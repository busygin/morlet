//
// Created by busygin on 1/11/16.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include "morlet.h"


void MorletWave::init(double width, double freq) {
   double st = 1.0/(2.0*M_PI*(freq/width));

   t=-3.5*st:dt:3.5*st;
   curWave=morlet(this.freqs(i),t,5);

   this.Lys(i)=this.total_winsize+length(curWave)-1;
   this.Ly2s(i)=pow2(nextpow2(this.Lys(i)));

   curWaveFFT=fft(curWave,this.Ly2s(i));

}