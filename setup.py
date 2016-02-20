import sys
import distutils
from distutils.core import setup, Extension
from distutils.sysconfig import get_config_var
from distutils.extension import Extension
import distutils.ccompiler
import os
from os.path import *
import sys
import shutil
from subprocess import call

root_dir = dirname(abspath(__file__))
fftw_name = 'fftw-3.3.4'
fftw_tar = fftw_name+'.tar.gz'

third_party_build_dir = join(root_dir,'build/third_party_build')
third_party_install_dir = join(root_dir,'build/third_party_install')

python_executable = sys.executable

print 'python_executable=',python_executable

print 'PYTHON LIB=',  distutils.sysconfig.get_python_lib(standard_lib=True)
print distutils.sysconfig.get_python_inc()
print distutils.sysconfig.get_python_version()


def check_dependencies():
    try:
        import numpy
    except ImportError:
        print 'Numpy is required to build PTSA. Please install Numpy before proceeding'

        sys.exit(1)


    swig_executable = distutils.spawn.find_executable('swig')
    if not swig_executable:
        raise OSError('Missing cmake - please install swig to proceed (www.swig.org)')


    compiler=distutils.ccompiler.new_compiler()
    lib_dirs=[join(third_party_install_dir,'lib')]

    if compiler.find_library_file(lib_dirs,'fftw3'):
        print 'FOUND FFTW3 library'
    else:
        print 'FOUND FFTW3 library'
        build_third_party_libs()


def build_third_party_libs():
    try:
        shutil.rmtree(third_party_build_dir)
    except OSError:
        pass

    try:
        shutil.rmtree(third_party_install_dir)
    except OSError:
        pass

    try:
        os.makedirs(third_party_install_dir)
    except OSError:
        pass

    try:
        os.makedirs(third_party_build_dir)
    except OSError:
        pass


    call (['tar','-xzf',join(root_dir,'third_party',fftw_tar),'-C',third_party_build_dir])

    fftw_src_dir = join(third_party_build_dir,fftw_name)

    orig_dir = os.getcwd()

    os.chdir(fftw_src_dir)

    call(['./configure','--prefix='+join(third_party_install_dir)])
    call(['make'])
    call(['make', 'install'])
    os.chdir(orig_dir)

def swig_third_party():
    call(['swig','-python','-outdir', 'morlet', 'morlet.i'])

check_dependencies()
swig_third_party()

import numpy

ext_modules = []
morlet_dir = '/Users/m/src/morlet_git'
morlet_module = Extension('morlet._morlet',
                           sources=[join(morlet_dir,'morlet.cpp'),join(morlet_dir,'morlet.i')],
                           swig_opts=['-c++'],
                           include_dirs=[join(third_party_install_dir,'include'),numpy.get_include()],
                           library_dirs=[join(third_party_install_dir,'lib')],
                            libraries=['fftw3'],

                           )

ext_modules.append(morlet_module)


setup(name='morlet',
      ext_modules = ext_modules,
      packages = ['morlet'],
      py_modules = ["morlet.morlet"]
      )


#
#  sources=['src/ext_module1.cpp',
#                          'src/ext_module2.cpp',
#                          'src/swig_module.i'],
#                 swig_opts=['-c++', '-py3'],
#                 include_dirs=[...],
#                 runtime_library_dirs=[...],
#                 libraries=[...],
#                 extra_compile_args=['-Wno-write-strings']

# ext_modules = []
# edf_ext = Extension("ptsa.data.edf.edf",
#                     sources = ["ptsa/data/edf/edf.c",
#                                "ptsa/data/edf/edfwrap.c",
#                                "ptsa/data/edf/edflib.c"],
#                     include_dirs=[numpy.get_include()],
#                     define_macros = [('_LARGEFILE64_SOURCE', None),
#                                      ('_LARGEFILE_SOURCE', None)])
# ext_modules.append(edf_ext)




sys.exit()



# # get the version loaded as vstr
# execfile('ptsa/versionString.py')
#
# # set up extensions
# ext_modules = []
# edf_ext = Extension("ptsa.data.edf.edf",
#                     sources = ["ptsa/data/edf/edf.c",
#                                "ptsa/data/edf/edfwrap.c",
#                                "ptsa/data/edf/edflib.c"],
#                     include_dirs=[numpy.get_include()],
#                     define_macros = [('_LARGEFILE64_SOURCE', None),
#                                      ('_LARGEFILE_SOURCE', None)])
# ext_modules.append(edf_ext)
#
# # define the setup
# setup(name='ptsa',
#       version=vstr,
#       maintainer=['Per B. Sederberg'],
#       maintainer_email=['psederberg@gmail.com'],
#       url=['http://ptsa.sourceforge.net'],
#       packages=['ptsa','ptsa.tests','ptsa.data','ptsa.data.tests',
#                 'ptsa.data.edf','ptsa.plotting','ptsa.plotting.tests',
#                 'ptsa.stats',
#                 'dimarray','dimarray.tests'],
#       ext_modules = ext_modules
#       )
#
