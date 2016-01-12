//
// Created by busygin on 1/11/16.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include "morlet.h"


size_t nextpow2(size_t v) {
   --v;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   return ++v;
}


void MorletWaveFFT::init(size_t width, double freq, size_t win_size) {
   double st = 1.0/(2.0*M_PI*(freq/width));

   t=-3.5*st:dt:3.5*st;
   cur_wave=morlet(freq,t,width);

   len0 = win_size + length(curWave) - 1;
   len = nextpow2(len0);
   fft = (fftw_complex*) fftw_malloc(len*sizeof(fftw_complex));

   fftw_plan plan = fftw_plan_dft_1d(len, cur_wave, fft, FFTW_FORWARD, FFTW_ESTIMATE);
   fftw_execute(plan);

   fftw_destroy_plan(plan);
   fftw_free(fft);
}
