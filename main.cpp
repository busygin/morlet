//
// Created by Stas Busygin on 1/12/16.
//

#include <cstdio>
#include <cmath>
#include "morlet.h"

int main() {
   MorletWaveletTransform morlet;
   morlet.init(5, 3.0, 180.0, 8, 4096, 1000);
   for(size_t i=0; i<8; ++i)
      printf("%ld ", morlet.morlet_wave_ffts[i].len0);
}