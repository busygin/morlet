//
// Created by busygin on 1/11/16.
//

#ifndef MORLET_MORLET_H
#define MORLET_MORLET_H

#include <complex.h>
#include <fftw3.h>

class MorletWave {
public:
   size_t len;
   double *fft;

   MorletWave(): len(0), fft(NULL) {}
   ~MorletWave() { if(fft) fftw_free(fft); }

   void init(double width, double freq);
};

#endif //MORLET_MORLET_H
