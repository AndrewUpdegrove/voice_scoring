#include "aubio/aubio.h"
#include "stdio.h"
int main (int argc, char **argv)
{
  if (argc < 2){
    printf("Not enough arguments\n");
  }
  uint_t samplerate = 0;
  uint_t hop_size = 256;
  uint_t win_size = 1024;
  uint_t total_frames = 0, frames_read = 0;
  fvec_t *hopper = new_fvec(hop_size);

  char_t *source_path = argv[1];
  aubio_source_t* src  = new_aubio_source(source_path, samplerate, hop_size);
  samplerate = aubio_source_get_samplerate(src);

  aubio_pitch_t *o = new_aubio_pitch("default", win_size, hop_size, samplerate);
  //aubio_pitch_set_silence(o, .5);
  fvec_t *out_buffer = new_fvec(hop_size);
  fvec_t *pitch = new_fvec(1);

  aubio_wavetable_t *wavetable = new_aubio_wavetable(samplerate, hop_size);
  uint_t succ = aubio_wavetable_play(wavetable);
  printf("Success? %d\n", succ);


  do {
    aubio_source_do(src, hopper, &frames_read);
    aubio_pitch_do(o, hopper, pitch);
    smpl_t freq =fvec_get_sample(pitch, 0);
    printf("Pitch is: %f\n", pitch->data[0]);
    aubio_wavetable_set_amp(wavetable, aubio_level_lin(hopper));
    aubio_wavetable_set_freq(wavetable,freq);
    aubio_wavetable_do(wavetable, hopper, out_buffer);
    //fvec_print(hopper);
    total_frames += frames_read;
  } while (frames_read == hop_size);
  printf("read %d frames at %dHz (%d blocks) from %s\n", total_frames, samplerate, total_frames / hop_size, source_path);

  del_fvec(hopper);
  del_fvec(pitch);
  del_fvec(out_buffer);

  del_aubio_source(src);
  del_aubio_pitch(o);
  del_aubio_wavetable(wavetable);


}
