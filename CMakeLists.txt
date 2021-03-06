CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MorletPython)



# SET(FFTW_LIB_DIR  "/Users/m/src-install/fftw-3.3.4/lib" CACHE PATH "path to directory with libfft library")
# SET(FFTW_INCLUDE_DIR "/Users/m/src-install/fftw-3.3.4/include" CACHE PATH "path to directory with libfft headers")




# SET(FFTW_LIB "fftw3" CACHE STRING "name of the fftw library")

SET(PYTHON_INSTALL_DIR "/Users/m/miniconda/envs/ram/" CACHE PATH "path to Python installation")


SET(PYTHON_INCLUDE_DIR ${PYTHON_INSTALL_DIR}/include/python2.7 CACHE PATH "path to Python include dir")
SET(PYTHON_LIBRARY ${PYTHON_INSTALL_DIR}/lib/libpython2.7.dylib CACHE PATH "path to Python library")
SET(NUMPY_INCLUDE_DIR ${PYTHON_INSTALL_DIR}/lib/python2.7/site-packages/numpy/core/include CACHE PATH "path to directory with numpy headers")
SET(PYTHON_EXECUTABLE ${PYTHON_INSTALL_DIR}/bin/python CACHE PATH "path to Python executable")

SET(FFTW_LIB_DIR  "/Users/m/src-install/fftw-3.3.4/lib" CACHE PATH "path to directory with libfft library")
SET(FFTW_LIB "fftw3" CACHE STRING "name of the fftw library")
# SET(FFTW_LIB "/Users/m/src-install/fftw-3.3.4/lib/libfftw3.la" CACHE PATH "path to fftw library")
SET(FFTW_INCLUDE_DIR "/Users/m/src-install/fftw-3.3.4/include" CACHE PATH "path to directory with libfft headers")


link_directories(${FFTW_LIB_DIR})
include_directories(${FFTW_INCLUDE_DIR})

# --------- morlet library
SET(SRCS morlet.cpp resample.cpp)
ADD_LIBRARY(morlet STATIC ${SRCS})
TARGET_LINK_LIBRARIES(morlet ${FFTW_LIB})

# --------- morlet executable
ADD_EXECUTABLE(morlet_exe  main.cpp)
TARGET_LINK_LIBRARIES(morlet_exe morlet)
install(TARGETS morlet_exe  DESTINATION ${CMAKE_INSTALL_PREFIX})



FIND_PACKAGE(SWIG REQUIRED)

# INCLUDE(${SWIG_USE_FILE})
INCLUDE(UseSWIG)

FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(PythonInterp)


SET(LIBS
   morlet
   ${PYTHON_LIBRARIES}
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${NUMPY_INCLUDE_DIR}
)

SET_SOURCE_FILES_PROPERTIES(morlet.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_MODULE(morlet python morlet.i)

SWIG_LINK_LIBRARIES(morlet ${LIBS})

install(TARGETS _morlet    DESTINATION  ${CMAKE_INSTALL_PREFIX})
# install(TARGETS _CC3DXML    DESTINATION lib/pythonOld )

set(python_files_path ${CMAKE_BINARY_DIR})

INSTALL(FILES 
    ${python_files_path}/morlet.py
	DESTINATION ${CMAKE_INSTALL_PREFIX}
	)



