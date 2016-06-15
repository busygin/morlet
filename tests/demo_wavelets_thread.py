import sys
# from os.path import *
# sys.path.append(join(expanduser('~'),'PTSA_NEW_GIT'))

from ptsa.data.readers import BaseEventReader
from ptsa.data.readers.TalReader import TalReader
from ptsa.data.readers import EEGReader
from ptsa.data.filters import MonopolarToBipolarMapper
from ptsa.data.filters import ButterworthFilter
from ptsa.data.filters.MorletWaveletFilterCpp import MorletWaveletFilterCpp
from ptsa.data.filters.MorletWaveletFilter import MorletWaveletFilter


import morlet
import numpy as np



import matplotlib
matplotlib.use('Qt4Agg')
import numpy as np
import matplotlib.pyplot as plt
import time

e_path = '/Users/m/data/events/RAM_FR1/R1111M_events.mat'
tal_path = '/Users/m/data/eeg/R1111M/tal/R1111M_talLocs_database_bipol.mat'
# ------------------- READING TAL STRUCTS
tal_reader = TalReader(filename=tal_path)
monopolar_channels = tal_reader.get_monopolar_channels()
bipolar_pairs = tal_reader.get_bipolar_pairs()
# ------------------- READING EVENTS
base_e_reader = BaseEventReader(filename=e_path, eliminate_events_with_no_eeg=True)
base_events = base_e_reader.read()
base_events = base_events[base_events.type == 'WORD']

base_events = base_events[:2]

monopolar_channels = monopolar_channels[:2]


freqs = np.logspace(np.log10(3), np.log10(180), 1)

s= time.time()
eeg_reader = EEGReader(events=base_events, channels=monopolar_channels,
                       start_time=0.0, end_time=1.6, buffer_time=1.0)


test_flag = False
# output = 'power'
# output = 'both'
# output = 'phase'
output = 'complex'
cpus = 1

base_eegs = eeg_reader.read()

num_chans = base_eegs.shape[0]
num_evs = base_eegs.shape[1]
num_time_points = base_eegs.shape[2]
num_freqs = freqs.shape[0]


# base_eegs_reshaped = base_eegs.data.reshape(num_chans*num_evs,num_time_points)
# base_eegs_un_reshaped = base_eegs_reshaped.reshape(num_chans,num_evs, num_time_points)
# print np.array_equal(base_eegs,base_eegs_un_reshaped)

print 'TOTAL READ TIME FOR EEGS = ',time.time()-s


from MorletWaveletFilterMP import MorletWaveletFilterMP

mmp = MorletWaveletFilterMP(time_series=base_eegs,freqs=freqs,output=output,cpus=cpus)


s = time.time()
pow_wavelet_new, phase_wavelet_new = mmp.filter()
print 'TOTAL CPP MP WAVELET TIME=', time.time()-s


if output =='complex':
    mmp_both = MorletWaveletFilterMP(time_series=base_eegs,freqs=freqs,output='both',cpus=cpus)
    pow_wavelet_new_1, phase_wavelet_new_1 = mmp_both.filter()
    phases_local = np.arctan2(pow_wavelet_new.imag,pow_wavelet_new.real)
    powers_local = pow_wavelet_new.imag**2+pow_wavelet_new.real**2

    print " CHECKING PHASES = ",np.array_equal(phases_local, phase_wavelet_new_1)
    print " CHECKING POWERS = ",np.array_equal(powers_local, pow_wavelet_new_1)

    mn = np.min((powers_local.data-pow_wavelet_new_1.data)/pow_wavelet_new_1.data)
    mx = np.max((powers_local.data-pow_wavelet_new_1.data)/pow_wavelet_new_1.data)

    if np.abs(mn)>1e-7 or np.abs(mx)>1e-7:
        print 'POWER ARRAYS DIFFER mn,mx = ', (mn,mx)
    else:
        print 'POWER ARRAYS AGREE ', (mn,mx)

    mn = np.min((phases_local.data-phase_wavelet_new_1.data)/phase_wavelet_new_1.data)
    mx = np.max((phases_local.data-phase_wavelet_new_1.data)/phase_wavelet_new_1.data)

    if np.abs(mn)>1e-7 or np.abs(mx)>1e-7:
        print 'PHASE ARRAYS DIFFER mn,mx = ', (mn,mx)
    else:
        print 'PHASE ARRAYS AGREE ', (mn,mx)





sys.exit()

wf = MorletWaveletFilterCpp(time_series=base_eegs,freqs=freqs,output=output)


s = time.time()
pow_wavelet, phase_wavelet = wf.filter()

# pow_wavelet = pow_wavelet.transpose('channels','events','frequency','time')
print 'TOTAL WAVELET TIME=', time.time()-s


num_chans = base_eegs.shape[0]
num_evs = base_eegs.shape[1]
num_time_points = base_eegs.shape[2]
num_freqs = freqs.shape[0]

# if test_flag:
#     if output=='power' or output=='both':
#         mn = np.min((pow_wavelet.data-pow_wavelet_new.data)/pow_wavelet_new.data)
#         mx = np.max((pow_wavelet.data-pow_wavelet_new.data)/pow_wavelet_new.data)
#
#         if np.abs(mn)>1e-7 or np.abs(mx)>1e-7:
#             print 'POWER ARRAYS DIFFER mn,mx = ', (mn,mx)
#         else:
#             print 'POWER ARRAYS AGREE ', (mn,mx)
#
#     if output=='phase' or output=='both':
#         mn = np.min((phase_wavelet.data-phase_wavelet_new.data)/phase_wavelet_new.data)
#         mx = np.max((phase_wavelet.data-phase_wavelet_new.data)/phase_wavelet_new.data)
#
#         if np.abs(mn)>1e-7 or np.abs(mx)>1e-7:
#             print 'PHASE ARRAYS DIFFER mn,mx = ', (mn,mx)
#         else:
#             print 'PHASE ARRAYS AGREE ', (mn,mx)

# for freq in range(num_freqs):
#     for chan in range(num_chans):
#         for ev in range(num_evs):
#
#             mn = np.min((pow_wavelet.data[freq,chan,ev,:]-pow_wavelet_new.data[freq,chan,ev,:])/pow_wavelet_new.data[freq,chan,ev,:])
#             mx = np.max((pow_wavelet.data[freq,chan,ev,:]-pow_wavelet_new.data[freq,chan,ev,:])/pow_wavelet_new.data[freq,chan,ev,:])
#
#             # if mx==np.inf or mn == np.inf:
#
#                 # print 'freq,chan,ev, min, max=',(freq,chan,ev,mn,mx)
#             if np.abs(mn)>1e-7 or np.abs(mx)>1e-7:
#                 print 'freq,chan,ev, min, max=',(freq,chan,ev,mn,mx)
#
# print

sys.exit()
# m=1000
# n=100
# fq = 3
# a = np.arange(m*n,dtype=np.float).reshape(m,n)
#
# print 'a[2,123]=',a[2,23]
#
# print 'a[123,2]=',a[23,2]



# #------------------------- DEMO ------------------------------
# freqs = np.logspace(np.log10(3), np.log10(180), 8)
#
# wv = np.empty(m * n * fq, dtype=np.float).reshape(m*fq, n)
#
# mt = morlet.MorletWaveletTransformMP(10)
# mt.initialize_arrays(a,wv)
# mt.initialize_signal_props(500.0)
#
# mt.initialize_wavelet_props(5,freqs)
# mt.prepare_run()
# #------------------------- DEMO ------------------------------



# wv = np.empty(m * n * fq, dtype=np.float).reshape(m*fq, n)



wavelets_reshaped = np.empty(shape=(base_eegs_reshaped.shape[0]*freqs.shape[0] , base_eegs_reshaped.shape[1]), dtype=np.float)


wavelets_final = wavelets_reshaped.reshape(num_chans,num_evs,num_freqs,num_time_points)


mt = morlet.MorletWaveletTransformMP(4)


mt.initialize_arrays(base_eegs_reshaped,wavelets_reshaped)
mt.initialize_signal_props(float(base_eegs['samplerate']))
mt.initialize_wavelet_props(5,freqs)
mt.prepare_run()

s = time.time()
mt.threads_process_wavelets()

print 'TOTAL_CPP_WAVELET_TIME=',time.time()-s





wf = MorletWaveletFilterCpp(time_series=base_eegs,
                         freqs=freqs,
                         output='power'
                         )

s = time.time()
pow_wavelet, phase_wavelet = wf.filter()

pow_wavelet = pow_wavelet.transpose('channels','events','frequency','time')
print 'TOTAL WAVELET TIME=', time.time()-s



sys.exit()



mt.set_signal_array(a)
mt.set_wavelet_array(wv)

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
