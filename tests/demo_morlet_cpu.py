import morlet
import numpy as np
import sys


import matplotlib
matplotlib.use('Qt4Agg')
import numpy as np
import matplotlib.pyplot as plt

m=1000
n=100
fq = 3
a = np.arange(m*n,dtype=np.float).reshape(m,n)

print 'a[2,123]=',a[2,23]

print 'a[123,2]=',a[23,2]



# morlet_transform = morlet.MorletWaveletTransformMP(10)
# # morlet_transform.process_wavelets(a)
# morlet_transform.compute_sq(a,0,10)
#
# print a[0,3]



wv = np.empty(m * n * fq, dtype=np.float).reshape(m*fq, n)

mt = morlet.MorletWaveletTransformMP(10)

mt.set_signal_array(a)
mt.set_wavelet_array(wv)
mt.set_num_freq(3)


# mt.process_wavelets_out()
mt.threads_process_wavelets()


print wv[0,:]

print wv[1,:]

print wv[2,:]


m = 1000
n = 100

fq = 3

a_test = np.arange(m * n, dtype=np.float).reshape(m, n)

# print 'a[2,123]=', a[2, 123]

# print 'a[123,2]=', a[123, 2]

wv0_test = a_test
wv1_test = np.sin(a_test)
wv2_test = np.log10(a_test+1)

wv_test = np.empty(m * n * fq, dtype=np.float).reshape(m*fq, n)

# wv[:,:,0::3] = wv0
# wv[:,:,1::3] = wv1
# wv[:,:,2::3] = wv2

wv_test[0::3,:] = wv0_test
wv_test[1::3,:] = wv1_test
wv_test[2::3,:] = wv2_test

print np.array_equal(wv_test,wv)



#
#
# morlet_transform.process_wavelets_out()

# morlet_transform.compute_sq()



# num_freqs = 8

#
# # morlet_transform.threads()
#
# morlet_transform.process_wavelets(a)
#
# print morlet_transform
