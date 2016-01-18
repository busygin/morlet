#include "mex.h"
#include "class_handle.hpp"
#include "morlet.h"


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
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("init: Unexpected arguments.");
        // Call the method
        wavelet_transformer->init(5, 3.0, 180.0, 8, 1000.0, 4096);
        return;
    }
    // multiphasevec
    if (!strcmp("multiphasevec", cmd)) {
        // Check parameters
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("multiphasevec: Unexpected arguments.");
        // Call the method
        wavelet_transformer->multiphasevec_powers();
        return;
    }
    
    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
