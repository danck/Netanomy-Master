#!/bin/bash

# configure Travis CI's build server for a C++11 build with GCC

sudo add-apt-repository --yes ppa:ubuntu-toolchain-r/test
sudo apt-get --yes update
sudo apt-get --yes install g++-4.7 c++-4.7
export CC=/usr/bin/gcc-4.7
export CXX=/usr/bin/g++-4.7
