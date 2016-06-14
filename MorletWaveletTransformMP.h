//
// Created by m on 6/10/16.
//
#pragma once
//#ifndef MORLETPYTHON_MORLETWAVELETTRANSFORMMP_H
//#define MORLETPYTHON_MORLETWAVELETTRANSFORMMP_H

#include <vector>
#include <memory>
#include <mutex>


class MorletWaveletTransform;

class MorletWaveletTransformMP {

    std::mutex mut;

public:
    unsigned int cpus = 1;
    unsigned int num_freq = -1;

    std::vector<std::shared_ptr<MorletWaveletTransform>> mwt_vec;

    size_t signal_len=-1;
    size_t num_signals=-1;

    double *signal_array=nullptr;
    double *wavelet_pow_array=nullptr;
    double *wavelet_phase_array=nullptr;
    double *freqs=nullptr;
    double sample_freq = -1.0;
    size_t  width = -1;



    MorletWaveletTransformMP(unsigned int cpus = 1);


    void set_signal_array(double *signal_array, size_t num_signals, size_t signal_len){
        this->signal_len = signal_len;
        this->num_signals = num_signals;
        this->signal_array = signal_array;

    }

    void set_wavelet_pow_array(double *wavelet_pow_array, size_t num_wavelets, size_t signal_len){
        this->wavelet_pow_array = wavelet_pow_array;
    }

    void set_wavelet_phase_array(double *wavelet_phase_array, size_t num_wavelets, size_t signal_len){
        this->wavelet_phase_array = wavelet_phase_array;
    }



//    void initialize_arrays(double *signal_array, size_t num_signals, size_t signal_len,
//                           double *wavelet_pow_array, size_t num_wavelets, size_t signal_len_pow
//    )
//    {
//        this->signal_len = signal_len;
//        this->num_signals = num_signals;
//
//        this->signal_array = signal_array;
//
//        this->wavelet_pow_array = wavelet_pow_array;
//
//    }


    void initialize_signal_props(double sample_freq){
        this->sample_freq=sample_freq;
    }

    void initialize_wavelet_props(size_t width, double *freqs, size_t nf){
        this->freqs = freqs;
        this->num_freq = nf;
        this->width = width;
    }


    void prepare_run();

    void process_wavelets_out_real(unsigned int thread_no);


    void compute();

    void computeMP(int cpu);

    void threads();

    void process_wavelets(double *signal_array, size_t num_signals, size_t signal_len);


    void set_num_freq(unsigned int num_freq) {
        this->num_freq = num_freq;
    }

    void process_wavelets_out();

    void process_wavelets_out_threads(unsigned int thread_no);

    void threads_process_wavelets();


    inline size_t index(size_t i, size_t j, size_t stride) {

        return i * stride + j;
    }

    void compute_sq(double *signal_array, size_t num_signals, size_t signal_len,size_t start, size_t stop);

private:
    std::vector<double> array;
};


//#endif //MORLETPYTHON_MORLETWAVELETTRANSFORMMP_H
