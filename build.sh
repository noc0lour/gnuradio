#!/bin/bash

set -e
# Disable the GUI
cmakeopts=(
    '-DENABLE_GRC=OFF'
    '-DENABLE_GR_QTGUI=OFF'
    '-DENABLE_GR_WXGUI=OFF'
)

# Disable the Testsuite
cmakeopts+=(
    '-DENABLE_TESTING=OFF'
)

# Disable these components because the build system is missing dependencies or we don't need them
cmakeopts+=(
    '-DENABLE_PERFORMANCE_COUNTERS=OFF'
    '-DENABLE_DOXYGEN=OFF'
    '-DENABLE_SPHINX=OFF'
    '-DENABLE_GR_COMEDI=OFF'
    '-DENABLE_GR_UHD=OFF'
    '-DENABLE_GR_FCD=OFF'
    '-DENABLE_GR_ZEROMQ=OFF'
    '-DENABLE_GR_ATSC=OFF'
    '-DENABLE_GR_AUDIO=OFF'
    '-DENABLE_GR_CHANNELS=OFF'
    '-DENABLE_GR_NOAA=OFF'
    '-DENABLE_GR_PAGER=OFF'
    '-DENABLE_GR_TRELLIS=OFF'
    '-DENABLE_GR_VOCODER=OFF'
    '-DENABLE_GR_WAVELET=OFF'
    '-DENABLE_GR_CTRLPORT=OFF'
)

export CXXFLAGS="-march=skylake -mtune=skylake"

cmake ${cmakeopts[@]} $SRC

