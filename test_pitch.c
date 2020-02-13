#include "aubio/aubio.h"
#include "stdio.h"
int main (int argc, char **argv)
{
  // 1. allocate some memory
  uint_t n = 0; // frame counter
  uint_t win_s = 1024; // window size
  uint_t hop_s = win_s / 4; // hop size
  uint_t samplerate = 0; // samplerate
  unint_t n_frames = 0, read = 0;

  unint_t err = 0;
  // create some vectors
  fvec_t *input = new_fvec (hop_s); // input buffer
  fvec_t *out = new_fvec (1); // output candidates

  //declare new source
  char_t *source_path = argv[1];
  aubio_source_t* s = new_aubio_soure(source_path_samplerate, hop_s);
  if(!s){err = 1; goto beach;}
  if (samplerate == 0) samplerate = aubio_source_get_samplerate(s);


  // create pitch object
  aubio_pitch_t *o = new_aubio_pitch ("default", win_s, hop_s, samplerate);


  while (n < 100) {
    // get `hop_s` new samples into `input`
    // ...
    // exectute pitch
    aubio_pitch_do (o, input, out);
    // do something with output candidates
    // ...
    n++;
  };
  // 3. clean up memory
  del_aubio_pitch (o);
  del_fvec (out);
  del_fvec (input);
  aubio_cleanup ();
  beach:
    return err;
}
