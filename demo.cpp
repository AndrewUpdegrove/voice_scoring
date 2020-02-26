#include "aubio/aubio.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <iomanip> //for specify float output
#include "math.h" //included for rounding of midi value
#include <vector>

std::string convert_midi(int midi){
  std::string out = "";
  switch(midi)
  {
    case 67: out = "G4";
      break;
    case 64: out = "E4";
      break;
    case 66: out = "F#4";
      break;
    case 60: out = "C4";
      break;
    case 72: out = "C5";
      break;
    case 76: out = "E5";
      break;
    case 74: out = "D5";
      break;
    default: out = "Unknown";
      break;
  }
  return out;
}

int roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}


int main (int argc, char **argv)
{
  if (argc < 2){
    printf("Not enough arguments\n");
  }

  uint_t samplerate = 0;
  uint_t hop_size = 256;
  uint_t win_size = hop_size * 4;
  uint_t total_frames = 0, frames_read = 0;
  fvec_t *hopper = new_fvec(hop_size);

  char_t *source_path = argv[1];
  aubio_source_t* src  = new_aubio_source(source_path, samplerate, hop_size);
  samplerate = aubio_source_get_samplerate(src);

  aubio_pitch_t *o = new_aubio_pitch("default", win_size, hop_size, samplerate);
  aubio_pitch_set_unit(o, "midi");
  fvec_t *out_buffer = new_fvec(hop_size);
  fvec_t *pitch = new_fvec(1);

  aubio_onset_t *start = new_aubio_onset("energy", win_size, hop_size, samplerate);
  aubio_onset_set_threshold(start, 0);
  aubio_onset_set_silence(start, -40);
  fvec_t *onset = new_fvec(1);

  std::vector<float> notes;
  uint_t dur = aubio_source_get_duration(src);

  do {
    aubio_source_do(src, hopper, &frames_read);
    aubio_onset_do(start, hopper, onset);

    smpl_t is_onset = aubio_onset_get_last(start);
    if(notes.empty()){
      if(is_onset <= dur){
        notes.push_back(is_onset);
      }
    } else if(is_onset <= dur && notes.back() < is_onset){
      notes.push_back(is_onset);
    }
    //printf("Onset is: %f\n", is_onset);
    //fvec_print(hopper);
    total_frames += frames_read;
  } while (frames_read == hop_size);

  //printf("read %d frames at %dHz (%d blocks) from %s\n", total_frames, samplerate, total_frames / hop_size, source_path);

  std::cout << std::setprecision(2) << std::fixed;
  std::cout << notes.size() << " notes were detected.\nThey are:\n";
  std::vector<int> pitches(notes.size());
  for(int i = 0; i < notes.size(); i++){
    aubio_source_seek(src, notes[i]);
    aubio_source_do(src, hopper, &frames_read);
    aubio_pitch_do(o, hopper, pitch);
    pitches[i] = (int) round(fvec_get_sample(pitch, 0));
    std::cout << "Pitch " << pitches[i] << " played at " << notes[i] << "\n";
  }



  del_fvec(hopper);
  del_fvec(pitch);
  del_fvec(out_buffer);
  del_fvec(onset);

  del_aubio_source(src);
  del_aubio_pitch(o);
  del_aubio_onset(start);

}
