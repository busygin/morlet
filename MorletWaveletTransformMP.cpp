//
// Created by m on 6/10/16.
//
#include <iostream>
#include "MorletWaveletTransformMP.h"

#include <thread>
#include <list>
#include <cmath>
#include "morlet.h"

using namespace std;

MorletWaveletTransformMP::MorletWaveletTransformMP(unsigned int cpus) : cpus(cpus) {

    array.assign(1000000, 0.0);

//    mwt_vec.assign(cpus,std::shared_ptr<MorletWaveletTransform>());
//    for (unsigned int i = 0 ; i< cpus ; ++i){
//
//
//    }
}

void MorletWaveletTransformMP::prepare_run() {

    for (unsigned int i = 0 ; i< cpus ; ++i){
        mwt_vec.push_back(shared_ptr<MorletWaveletTransform>(new MorletWaveletTransform));
        auto & mwt_ptr = mwt_vec[i];
        mwt_ptr->init_flex(width, freqs, num_freq, sample_freq, signal_len);


    }

    cerr<<"num_freq="<<num_freq<<endl;
    cerr<<"signal_len="<<signal_len<<endl;
    cerr<<"samplfreq="<<sample_freq<<endl;
    cerr<<"num_signals="<<num_signals<<endl;
}

void MorletWaveletTransformMP::process_wavelets_out_real(unsigned int thread_no) {

    cerr<<"\n\n\n INSIDE process_wavelets_out_real"<<endl;
    auto & mwt = mwt_vec[thread_no];

    std::vector<double> powers(signal_len*num_freq,0.0);



    size_t chunk = num_signals / cpus;

    size_t idx_start = thread_no * chunk;
    size_t idx_stop = thread_no * chunk + chunk ;

    if (thread_no == cpus - 1) {
        idx_stop = num_signals ;
    }



    cerr << "thread num = " << thread_no << endl;
    cerr << "idx_start=" << idx_start << " idx_stop=" << idx_stop << endl;


    for (size_t sig_num = idx_start; sig_num < idx_stop; ++sig_num) {
        size_t idx_signal = index(sig_num, 0, signal_len);
        auto signal = signal_array + idx_signal;
//        mwt->multiphasevec_powers(signal, &powers[0]);

        size_t idx_out = index(num_freq * sig_num, 0, signal_len);
        mwt->multiphasevec_powers(signal, wavelet_pow_array + idx_out);



//        cerr<<"idx_out="<<idx_out<<endl;
        //copying content of powers vec
//        copy(powers.begin(),powers.end(),wavelet_pow_array+idx_out);



        }







}



void MorletWaveletTransformMP::compute() {

    int c = 0;
    for (auto &a : array) {

        a = c * c;
        ++c;
    }

    for (int i = 0; i < 20; ++i) {
        cerr << "a[" << i << "]=" << array[i] << endl;

    }

}

void MorletWaveletTransformMP::process_wavelets(double *signal_array, size_t num_signals, size_t signal_len) {

    cerr << "num_signals=" << num_signals << endl;
    cerr << "signal_len=" << signal_len << endl;
    cerr << "s[0,0]=" << signal_array << endl;


    int i = 2, j = 123;

    cerr << "a[2,123]=" << signal_array[i * signal_len + j];
    i = 123;
    j = 2;
    cerr << "a[123,2]=" << signal_array[i * signal_len + j];

}






void MorletWaveletTransformMP::process_wavelets_out() {

    cerr << "num_signals=" << num_signals << endl;
    cerr << "signal_len=" << signal_len << endl;
    cerr << "s[0,0]=" << signal_array << endl;

    size_t stop = num_signals;

    size_t start=0;


    for (size_t sig_num = start; sig_num < stop; ++sig_num ) {
        for (size_t i = 0; i < signal_len; ++i) {

            size_t idx_in = index(sig_num, i, signal_len);

            double a = signal_array[idx_in];

            size_t idx_out = index(num_freq*sig_num, i, signal_len);
            wavelet_pow_array[idx_out] = a;

            idx_out = index(num_freq*sig_num+1, i, signal_len);
            wavelet_pow_array[idx_out] = sin(a);

            idx_out = index(num_freq*sig_num+2, i, signal_len);
            wavelet_pow_array[idx_out] = log10(a+1);

        }

    }

}


void MorletWaveletTransformMP::process_wavelets_out_threads(unsigned int thread_no) {

    cerr << "num_signals=" << num_signals << endl;
    cerr << "signal_len=" << signal_len << endl;
    cerr << "s[0,0]=" << signal_array << endl;



    size_t start=0;

    size_t stop = num_signals;


    size_t chunk = num_signals / cpus;

    size_t idx_start = thread_no * chunk;
    size_t idx_stop = thread_no * chunk + chunk - 1;

    if (thread_no == cpus - 1) {
        idx_stop = num_signals - 1;
    }




    for (size_t sig_num = start; sig_num < stop; ++sig_num ) {
        for (size_t i = 0; i < signal_len; ++i) {

            size_t idx_in = index(sig_num, i, signal_len);


            double a = signal_array[idx_in];

            size_t idx_out = index(num_freq*sig_num, i, signal_len);
            wavelet_pow_array[idx_out] = a;

            idx_out = index(num_freq*sig_num+1, i, signal_len);
            wavelet_pow_array[idx_out] = sin(a);

            idx_out = index(num_freq*sig_num+2, i, signal_len);
            wavelet_pow_array[idx_out] = log10(a+1);



//            signal_array[idx] = a * a;

        }

    }


}




void MorletWaveletTransformMP::threads_process_wavelets() {

    cerr << "THREADED FUNCTION" << endl;

    std::list<std::thread> thread_list;
    for (unsigned int i = 0; i < cpus; ++i) {
//        thread_list.push_back(std::thread([=] { process_wavelets_out_threads(i); }));
        thread_list.push_back(std::thread([=] { process_wavelets_out_real(i); }));


    }

    for (auto &t: thread_list) {

        t.join();

    }

}



void MorletWaveletTransformMP::compute_sq(double *signal_array, size_t num_signals, size_t signal_len, size_t start,
                                          size_t stop) {

    for (size_t sig_num = start; sig_num < stop; ++sig_num) {
        for (size_t i = 0; i < signal_len; ++i) {
            size_t idx = index(sig_num, i, signal_len);
            double a = signal_array[idx];
            signal_array[idx] = a * a;

        }

    }
}

void MorletWaveletTransformMP::computeMP(int cpu) {

    size_t s = array.size();

    size_t chunk = s / cpus;

    size_t idx_start = cpu * chunk;
    size_t idx_stop = cpu * chunk + chunk - 1;

    if (cpu == cpus - 1) {
        idx_stop = s - 1;
    }

    cerr << "CPU=" << cpu << endl;
    cerr << "chunk=" << chunk << endl;


    cerr << "start=" << idx_start << endl;
    cerr << "stop=" << idx_stop << endl;
}


void MorletWaveletTransformMP::threads() {

    cerr << "THREADED FUNCTION" << endl;

    std::list<std::thread> thread_list;
    for (unsigned int i = 0; i < cpus; ++i) {
        thread_list.push_back(std::thread([=] { computeMP(i); }));

    }

    for (auto &t: thread_list) {

        t.join();

    }


//    std::thread t = std::thread([this] { this->computeMP(0); }  );
//    t.join();

//    std::list<std::thread> thread_list;
//    for (int i = 0  ; i < cpus ; ++i){
//
//        thread_list.push_back(std::thread(MorletWaveletTransformMP::computeMP,i));
//
//
//    }
//
//    for (auto &t: thread_list){
//
//        t.join();
//
//    }

}