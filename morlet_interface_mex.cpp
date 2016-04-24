#include "mex.h"
#include "matrix.h"
#include "class_handle.hpp"
#include "morlet.h"

#include <cstdlib>
//#include <cstdio>
#include <cstring>


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{	
    // Get the command string
    char cmd[64];
	if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
		mexErrMsgTxt("First input should be a command string less than 64 characters long.");
        
    // New
    if (!strcmp("new", cmd)) {
        // Check parameters
        if (nlhs != 1)
            mexErrMsgTxt("New: One output expected.");
        // Return a handle to a new C++ instance
        plhs[0] = convertPtr2Mat<MorletWaveletTransform>(new MorletWaveletTransform);
        return;
    }
    
    // Check there is a second input, which should be the class instance handle
    if (nrhs < 2)
		mexErrMsgTxt("Second input should be a class instance handle.");
    
    // Delete
    if (!strcmp("delete", cmd)) {
        // Destroy the C++ object
        destroyObject<MorletWaveletTransform>(prhs[1]);
        // Warn if other commands were ignored
        if (nlhs != 0 || nrhs != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }
    
    // Get the class instance pointer from the second input
    MorletWaveletTransform *wavelet_transformer = convertMat2Ptr<MorletWaveletTransform>(prhs[1]);
    
    // Call the various class methods
    // init
    if (!strcmp("init", cmd)) {
        // Check parameters
        if (nlhs < 0 || nrhs < 8)
            mexErrMsgTxt("init: Unexpected arguments.");
        // Call the method
        size_t width = size_t(mxGetScalar(prhs[2]));
        double min_freq = mxGetScalar(prhs[3]);
        double max_freq = mxGetScalar(prhs[4]);
        size_t n_freqs = size_t(mxGetScalar(prhs[5]));
        double samplerate = mxGetScalar(prhs[6]);
        size_t winsize = size_t(mxGetScalar(prhs[7]));
        //printf("width=%ld, min_freq=%lg, max_freq=%lg, n_freqs=%ld, samplerate=%lg, winsize=%ld\n", width, min_freq, max_freq, n_freqs, samplerate, winsize);
        wavelet_transformer->init(width, min_freq, max_freq, n_freqs, samplerate, winsize);
        return;
    }
    // multiphasevec
    else if (!strcmp("multiphasevec", cmd)) {
        // Check parameters
        if (nlhs < 0 || nrhs < 4)
            mexErrMsgTxt("multiphasevec: Unexpected arguments.");

        // Call the method
        double* signal = mxGetPr(prhs[2]);
        double* powers = mxGetPr(prhs[3]);
        if (nrhs == 4)
            wavelet_transformer->multiphasevec_powers(signal, powers);
        else {
            double* phases = mxGetPr(prhs[4]);
            wavelet_transformer->multiphasevec_powers_and_phases(signal, powers, phases);
        }
        return;
    }
    
    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
