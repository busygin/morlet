//
// Created by busygin on 1/11/16.
//

#ifndef MORLET_MORLET_H
#define MORLET_MORLET_H

#include <complex.h>
#include <fftw3.h>

class MorletWaveFFT {
public:
   size_t len;
   fftw_complex *fft;

   size_t len0;

   MorletWaveFFT(): len(0), fft(NULL), len0(0) {}
   ~MorletWaveFFT() { if(fft) fftw_free(fft); }

   void init(size_t width, double freq, size_t win_size);
};

class MorletWaveletTransform {
public:
   size_t n_freqs;
   MorletWaveFFT *morlet_wave_ffts;

   MorletWaveletTransform(): n_freqs(0), morlet_wave_ffts(NULL) { }
   ~MorletWaveletTransform() { if (morlet_wave_ffts) delete[] morlet_wave_ffts; }

   void init(size_t width, double low_freq, double high_freq, size_t nf, size_t win_size);
};

#endif //MORLET_MORLET_H
