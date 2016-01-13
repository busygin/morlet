//
// Created by busygin on 1/12/16.
//

#ifndef MORLET_RESAMPLE_H
#define MORLET_RESAMPLE_H

#include <cstring>
// #include <complex.h>
#include <fftw3.h>


class Resampler {
public:
   size_t signal_len_;
   size_t resample_len_;

   double* signal_;
   fftw_complex* fft;
   fftw_complex* downsampled_signal_;

   fftw_plan plan_for_signal;
   fftw_plan plan_for_reduced_fft;

   Resampler(size_t signal_len, size_t resample_len): signal_len_(signal_len), resample_len_(resample_len),
      signal_((double*)fftw_malloc(signal_len*sizeof(double))),
      fft((fftw_complex*)fftw_malloc((signal_len/2+1)*sizeof(fftw_complex))),
      downsampled_signal_((fftw_complex*)fftw_malloc(resample_len*sizeof(fftw_complex))),
      plan_for_signal(fftw_plan_dft_r2c_1d(signal_len, signal_, fft, FFTW_PATIENT)),
      plan_for_reduced_fft(fftw_plan_dft_1d(resample_len, fft, downsampled_signal_, FFTW_BACKWARD, FFTW_PATIENT))
   {}

   ~Resampler() {
      if (signal_) fftw_free(signal_);
      if (fft) fftw_free(fft);
      if (downsampled_signal_) fftw_free(downsampled_signal_);
      fftw_destroy_plan(plan_for_signal);
      fftw_destroy_plan(plan_for_reduced_fft);
   }

   void run(double* signal, double* downsampled_signal);
};


#endif //MORLET_RESAMPLE_H
