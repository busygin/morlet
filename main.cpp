//
// Created by Stas Busygin on 1/12/16.
//

#include <cstdio>
#include <cmath>
#include <ctime>
#include "morlet.h"

#include "MorletWaveletTransformMP.h"


int main() {

   unsigned int cpus = 2;
   MorletWaveletTransformMP m(cpus=cpus);
//   m.compute();
//
//   for (int i = 0  ; i < cpus ; ++i){
//      m.computeMP(i);
//
//
//   }
//
//    m.threads();


//   MorletWaveletTransform morlet;
//   morlet.init(5, 3.0, 180.0, 8, 1000, 4096);
//
//   double* signal = (double*)fftw_malloc(4096*sizeof(double));
//   double* powers = (double*)fftw_malloc(8*4096*sizeof(double));
//
//   // srand(time(NULL));
//
//   for(size_t i=0; i<4096; ++i)
//      signal[i] = sin(i*M_PI/317.0);
//
//   /*time_t timer_beg, timer_end;
//   time(&timer_beg);
//   for(size_t i=0; i<100000; ++i)*/
//   morlet.multiphasevec_powers(signal, powers);
//   for(size_t i=0; i<8*4096; ++i)
//      printf("%lg ", powers[i]);
//   /*time(&timer_end);
//   double seconds = difftime(timer_end, timer_beg);
//   printf("%g secs spent on 100000 multiphasevec calls\n", seconds);*/
//
//   fftw_free(powers);
//   fftw_free(signal);

   return 0;
}