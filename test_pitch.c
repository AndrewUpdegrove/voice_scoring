#include "aubio/aubio.h"
#include "stdio.h"
int main (int argc, char **argv)
{
  // 1. allocate some memory
  uint_t n = 0; // frame counter
  uint_t win_s = 1024; // window size
  uint_t hop_s = win_s / 4; // hop size
  uint_t samplerate = 0; // samplerate
  uint_t n_frames = 0, read = 0;

  uint_t err = 0;
  // create some vectors
  fvec_t *input = new_fvec (hop_s); // input buffer


  //declare new source
  char_t *source_path = argv[1];
  aubio_source_t* s = new_aubio_source(source_path, samplerate, hop_s);
  if(!s){err = 1; goto beach;}
  if (samplerate == 0) samplerate = aubio_source_get_samplerate(s);

  //hardcoding the size of the ouput vector, later we'll use a dynamically growing one?
  fvec_t *out = new_fvec(8000); // output candidates
  printf("%u\n", samplerate);
  printf("%u\n", out->length);
  fvec_t *out_2 = new_fvec(1);
  // create pitch object
  aubio_pitch_t *o = new_aubio_pitch ("default", win_s, hop_s, samplerate);

  aubio_source_do(s, input, &read);
  //fvec_print (input);
  n_frames += read;
  uint_t out_count = 0;
  while(read == hop_s){
    aubio_pitch_do(o, input, out_2);
    out[out_count] = out_2[0];
    out_count++;
    printf("%u\n", out_count);
    aubio_source_do(s, input, &read);
    //fvec_print (input);
    n_frames += read;
  }

  printf("read %d frames at %dHz (%d blocks) from %s\n", n_frames, samplerate, n_frames / hop_s, source_path);
  fvec_print(out);

  del_aubio_pitch (o);
  del_aubio_source(s);
  del_fvec (out);
  del_fvec (input);
  aubio_cleanup ();
  beach:
    return err;
}
