#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aubio/aubio.h>
//#include "utils_tests.h"
int main (int argc, char **argv)
{
  uint_t err = 0;
  if (argc < 2) {
    err = 2;
    printf("not enough arguments\n");
    printf("read a wave file as a mono vector\n");
    printf("usage: %s <source_path> [samplerate] [hop_size]\n", argv[0]);
    printf("examples:\n");
    printf(" - read file.wav at original samplerate\n");
    printf("       %s file.wav\n", argv[0]);
    printf(" - read file.wav at 32000Hz\n");
    printf("       %s file.aif 32000\n", argv[0]);
    printf(" - read file.wav at original samplerate with 4096 blocks\n");
    printf("       %s file.wav 0 4096 \n", argv[0]);
    return err;
  }
  uint_t samplerate = 0;
  uint_t hop_size = 256;
  uint_t n_frames = 0, read = 0;
  if ( argc == 3 ) samplerate = atoi(argv[2]);
  if ( argc == 4 ) hop_size = atoi(argv[3]);
  char_t *source_path = argv[1];
  aubio_source_t* s =
    new_aubio_source(source_path, samplerate, hop_size);
  if (!s) { err = 1; goto beach; }
  fvec_t *vec = new_fvec(hop_size);
  if (samplerate == 0 ) samplerate = aubio_source_get_samplerate(s);
  do {
    aubio_source_do(s, vec, &read);
    fvec_print (vec);
    n_frames += read;
  } while ( read == hop_size );
  printf("read %d frames at %dHz (%d blocks) from %s\n", n_frames, samplerate,
    n_frames / hop_size, source_path);
  del_fvec (vec);
  del_aubio_source (s);
beach:
  return err;
}
