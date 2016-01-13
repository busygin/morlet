//
// Created by Stas Busygin on 1/12/16.
//

#include <cstdio>
#include <cmath>
#include <ctime>
#include "morlet.h"

int main() {
   MorletWaveletTransform morlet;
   morlet.init(5, 3.0, 180.0, 8, 1000, 4096);
   /*for(size_t i=0; i<8; ++i)
      printf("%ld ", morlet.morlet_wave_ffts[i].len0);*/

   double* signal = (double*)fftw_malloc(4096*sizeof(double));
   double* powers = (double*)fftw_malloc(8*4096*sizeof(double));

   // srand(time(NULL));

   for(size_t i=0; i<4096; ++i)
      signal[i] = sin(i*M_PI/317.0);

   time_t timer_beg, timer_end;
   time(&timer_beg);
   for(size_t i=0; i<100000; ++i)
      morlet.multiphasevec_powers(signal, powers);
   time(&timer_end);
   double seconds = difftime(timer_end, timer_beg);
   printf("%g secs spent on 100000 multiphasevec calls\n", seconds);

   fftw_free(powers);
   fftw_free(signal);
   return 0;
}