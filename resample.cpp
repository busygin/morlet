//
// Created by busygin on 1/12/16.
//

#include "resample.h"

void Resampler::run(double* signal, double* downsampled_signal) {
   memcpy(signal_, signal, signal_len_*sizeof(double));
   fftw_execute(plan_for_signal);
   for(size_t i=resample_len_/2+1; i<resample_len_; ++i) {
      fft[i][0] = fft[resample_len_-i][0];
      fft[i][1] = -fft[resample_len_-i][1];
   }
   fftw_execute(plan_for_reduced_fft);
   for(size_t i=0; i<resample_len_; ++i)
      downsampled_signal[i] = downsampled_signal_[i][0];
}