#include "aubio/aubio.h"
#include "stdio.h"
int main ()
{

  printf("Hello /\n");
  // 1. allocate some memory
  uint_t n = 0; // frame counter
  uint_t win_s = 1024; // window size
  uint_t hop_s = win_s / 4; // hop size
  uint_t samplerate = 44100; // samplerate

  fvec_t *input = new_fvec(hop_s); //input buffer
  fvec_t *out = nev_fvec(1);
