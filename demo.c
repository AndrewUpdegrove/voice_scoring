#include "aubio/aubio.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h" //included for rounding of midi value
#include <vector>


int main (int argc, char **argv)
{
  if (argc < 2){
    printf("Not enough arguments\n");
  }

  uint_t samplerate = 0;
  uint_t hop_size = 512;
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

  aubio_onset_t *start = new_aubio_onset("phase", win_size, hop_size, samplerate);
  aubio_onset_set_threshold(start, 5);
  aubio_onset_set_silence(start, -20);
  fvec_t *onset = new_fvec(1);

  vector<float> a(8);

  do {
    aubio_source_do(src, hopper, &frames_read);
    aubio_onset_do(start, hopper, onset);
    smpl_t freq = fvec_get_sample(pitch, 0);

    smpl_t is_onset = aubio_onset_get_last_ms(start);
    printf("Pitch is: %d\n", (int) round(freq));
    printf("Onset is: %f\n", is_onset);
    //fvec_print(hopper);
    total_frames += frames_read;
  } while (frames_read == hop_size);
  printf("read %d frames at %dHz (%d blocks) from %s\n", total_frames, samplerate, total_frames / hop_size, source_path);


  del_fvec(hopper);
  del_fvec(pitch);
  del_fvec(out_buffer);
  del_fvec(onset);

  del_aubio_source(src);
  del_aubio_pitch(o);
  del_aubio_onset(start);

}
