//
// Created by busygin on 1/11/16.
//

#ifndef MORLET_MORLET_H
#define MORLET_MORLET_H

#include <fftw3.h>

class MorletWaveFFT {
public:
   size_t len0;
   size_t len;
   fftw_complex* fft;
   size_t nt;

   MorletWaveFFT(): len0(0), len(0), fft(NULL) {}
   ~MorletWaveFFT() { if(fft) fftw_free(fft); }

   size_t init(size_t width, double freq, size_t win_size, double sample_freq);
};

class MorletWaveletTransform {
public:
   size_t n_freqs;
   MorletWaveFFT *morlet_wave_ffts;

   size_t signal_len_;

   double* signal_buf;
   fftw_complex* fft_buf;
   fftw_complex* prod_buf;
   fftw_complex* result_buf;

   size_t n_plans;
   fftw_plan* plan_for_signal;
   fftw_plan* plan_for_inverse_transform;

   MorletWaveletTransform(): n_freqs(0), morlet_wave_ffts(NULL), signal_buf(NULL), fft_buf(NULL), prod_buf(NULL), result_buf(NULL), n_plans(0), plan_for_signal(NULL), plan_for_inverse_transform(NULL) {}

   ~MorletWaveletTransform() {
      if (n_freqs) {
         delete[] morlet_wave_ffts;
         fftw_free(signal_buf);
         fftw_free(fft_buf);
         fftw_free(prod_buf);
         fftw_free(result_buf);
         for (size_t i=0; i<n_plans; ++i) {
            fftw_destroy_plan(plan_for_signal[i]);
            fftw_destroy_plan(plan_for_inverse_transform[i]);
         }
         delete[] plan_for_signal;
         delete[] plan_for_inverse_transform;
      }
   }

   void init(size_t width, double low_freq, double high_freq, size_t nf, double sample_freq, size_t signal_len);

   void multiphasevec_powers(double* signal, double* powers);  // input: signal, output: n_freqs*signal_len_ 1d array of powers

   // this is to make numpy interface possible
   void multiphasevec(double* signal, size_t signal_len, double* powers, size_t power_len);
};

#endif //MORLET_MORLET_H
