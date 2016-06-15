import morlet
import numpy as np

import matplotlib
matplotlib.use('Qt4Agg')
import numpy as np
import matplotlib.pyplot as plt

num_freqs = 8
morlet_transform = morlet.MorletWaveletTransform()
morlet_transform.init(5, 3.0, 180.0, num_freqs, 1000, 4096)


signal = np.arange(4096,dtype=np.float)

signal = np.sin(signal*np.pi/317.0)
powers=np.empty(shape=(signal.shape[0]*num_freqs,), dtype=np.float)

phases=np.empty(shape=(signal.shape[0]*num_freqs,), dtype=np.float)


num_of_iterations = 100
# for i in xrange(num_of_iterations):
#     morlet_transform.multiphasevec(signal,powers)


for i in xrange(num_of_iterations):
    morlet_transform.multiphasevec(signal,powers,phases)



print 'powers=',powers
print dir(morlet_transform)

i_min = 200
i_max = 2000

plt.get_current_fig_manager().window.raise_()
# plt.plot(np.arange(signal.shape[0]) [i_min:i_max] , signal[i_min:i_max])
# plt.plot(np.arange(signal.shape[0]) [i_min:i_max] , powers[i_min:i_max])

plt.plot(np.arange(signal.shape[0]) [i_min:i_max] , phases[i_min:i_max])

plt.show()