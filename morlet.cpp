//
// Created by busygin on 1/11/16.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstring>
#include "morlet.h"
#include "log_space.h"


size_t nextpow2(size_t v) {
   --v;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   v |= v >> 32;
   return ++v;
}


void MorletWaveFFT::init(size_t width, double freq, size_t win_size, double sample_freq) {
   double dt = 1.0 / sample_freq;
   double sf = freq / width;
   double st = 1.0 / (2.0*M_PI*sf);
   double a = 1 / sqrt(st*sqrt(M_PI));
   double omega = 2.0 * M_PI * freq;

   nt = size_t(7.0*st/dt)+1;
   len0 = win_size + nt - 1;
   len = nextpow2(len0);
   fft = (fftw_complex*)fftw_malloc(len*sizeof(fftw_complex));

   fftw_complex* cur_wave = (fftw_complex*)fftw_malloc(len*sizeof(fftw_complex));

   double t = -3.5*st;
   for(size_t i=0; i<nt; ++i) {
      double c = a * exp(-t*t/(2.0*st*st));
      double omega_t = omega * t;
      cur_wave[i][0] = c * cos(omega_t);
      cur_wave[i][1] = c * sin(omega_t);
      t += dt;
   }
   for(size_t i=nt; i<len;++i)
      cur_wave[i][0] = cur_wave[i][1] = 0.0;

   fftw_plan plan = fftw_plan_dft_1d(len, cur_wave, fft, FFTW_FORWARD, FFTW_ESTIMATE);
   fftw_execute(plan);

   fftw_destroy_plan(plan);
   fftw_free(cur_wave);
}

void MorletWaveletTransform::init(size_t width, double low_freq, double high_freq, size_t nf, double sample_freq, size_t signal_len) {
   signal_len_ = signal_len;
   n_freqs = nf;
   std::vector<double> freqs = logspace(log10(low_freq), log10(high_freq), nf);
   morlet_wave_ffts = new MorletWaveFFT[nf];
   for(size_t i=0; i<nf; ++i)
      morlet_wave_ffts[i].init(width, freqs[i], signal_len, sample_freq);

   fft_len = morlet_wave_ffts[0].len;
   prod_buf = (fftw_complex*)fftw_malloc(fft_len*sizeof(fftw_complex));
   result_buf = (fftw_complex*)fftw_malloc(fft_len*sizeof(fftw_complex));
   signal_buf = (double*)fftw_malloc(fft_len*sizeof(double));
   fft_buf = (fftw_complex*)fftw_malloc((fft_len/2+1)*sizeof(fftw_complex));

   plan_for_signal = fftw_plan_dft_r2c_1d(fft_len, signal_buf, fft_buf, FFTW_PATIENT);
   plan_for_inverse_transform = fftw_plan_dft_1d(fft_len, prod_buf, result_buf, FFTW_BACKWARD, FFTW_PATIENT);

   memset(signal_buf, 0, fft_len*sizeof(double));
}

void product_with_herm_fft(size_t len, fftw_complex* fft1, fftw_complex* fft_herm, fftw_complex* result) {
   // fft1 and result have length len
   // but fft_herm has length len/2+1
   result[0][0] = fft1[0][0]*fft_herm[0][0] - fft1[0][1]*fft_herm[0][1];
   result[0][1] = fft1[0][0]*fft_herm[0][1] + fft1[0][1]*fft_herm[0][0];

   size_t i;
   for(i=1; i<len/2; ++i) {
      result[i][0] = fft1[i][0]*fft_herm[i][0] - fft1[i][1]*fft_herm[i][1];
      result[i][1] = fft1[i][0]*fft_herm[i][1] + fft1[i][1]*fft_herm[i][0];
   }

   result[i][0] = fft1[i][0]*fft_herm[i][0] - fft1[i][1]*fft_herm[i][1];
   result[i][1] = fft1[i][0]*fft_herm[i][1] + fft1[i][1]*fft_herm[i][0];

   size_t j = len/2 - 1;
   for(++i; i<len; ++i, --j) {
      result[i][0] = fft1[i][0]*fft_herm[j][0] + fft1[i][1]*fft_herm[j][1];
      result[i][1] = fft1[i][1]*fft_herm[j][0] - fft1[i][0]*fft_herm[j][1];
   }
}

void MorletWaveletTransform::multiphasevec_powers(double* signal, double* powers) {
   memcpy(signal_buf, signal, signal_len_*sizeof(double));
   fftw_execute(plan_for_signal);
   for (MorletWaveFFT* wavelet=morlet_wave_ffts; wavelet<morlet_wave_ffts+n_freqs; ++wavelet) {
      // construct product
      product_with_herm_fft(fft_len, wavelet->fft, fft_buf, prod_buf);

      // inverse fft
      fftw_execute(plan_for_inverse_transform);

      // retrieve powers
      size_t first_idx = (wavelet->nt-1) / 2;
      for(size_t i=first_idx; i<first_idx+signal_len_; ++i) {
         result_buf[i][0] /= fft_len; result_buf[i][1] /= fft_len;
         *(powers++) = result_buf[i][0] * result_buf[i][0] + result_buf[i][1] * result_buf[i][1];
      }
   }
}
