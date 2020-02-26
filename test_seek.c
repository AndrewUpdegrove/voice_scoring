#include "aubio/aubio.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h" //included for rounding of midi value
int main (int argc, char **argv)
{
  if (argc < 2){
    printf("Not enough arguments\n");
  }
  uint_t samplerate = 0;
  uint_t hop_size = 1024;
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

  aubio_source_seek(src, 223314);
  aubio_source_do(src, hopper, &frames_read);
  aubio_pitch_do(o, hopper, pitch);
  smpl_t freq = fvec_get_sample(pitch,0);
  printf("Pitch is: %f\n", freq);

}
