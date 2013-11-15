#!/bin/sh
#
# Simple script to create a build/ subdirectory with Unix makefiles
# and install necessary third party libraries to
# build this Netanomy component for your platform
#

# check for required programs
if [ "`which cmake`" == "" ]; then
    echo "You must install CMake first - 'cmake' must be in your path"
    exit 1
fi

if [ "`which git`" == "" ]; then
    echo "You must install Git first"
    exit 1
fi


PROJECT_ROOT=`pwd`

#
# Install correct ZeroMQ library if necessary
#
if [ ! -d "lib/zeromq-4.0.1" ]; then
    echo "Installing ZeroMQ into project directory"
    mkdir lib
    cd lib
    wget http://download.zeromq.org/zeromq-4.0.1.tar.gz
    tar xzf zeromq-4.0.1.tar.gz
    rm zeromq-4.0.1.tar.gz
    cd zeromq-4.0.1
    ./configure --prefix=$PROJECT_ROOT/lib/zeromq

    ## Check for success
    make
    if [ `echo $?` != 0 ]; then
	echo "Building ZeroMQ failed. Exiting..."
    fi

    ## Check for success
    make install
    if [ `echo $?` != 0 ]; then
        echo "Building ZeroMQ failed. Exiting..."
        exit 1
    fi

    ## Get C++ binding
    cd $PROJECT_ROOT/lib/zeromq/include
    git clone https://github.com/zeromq/cppzmq.git

    ## Check for success
    if [ `echo $?` != 0 ]; then
        echo "Fetching C++ binding failed. Exiting..."
        exit 1
    fi

    ## Clean up
    cd $PROJECT_ROOT
    rm -rf $PROJECT_ROOT/lib/zeromq-4.0.1
fi

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug -DZMQ_INCLUDE=$PROJECT_ROOT/lib/zeromq/include -DZMQ_LIBRARY=$PROJECT_ROOT/lib/zeromq/lib/libzmq.so ..

echo "Creating convenience script for quick rebuilding: build/quickrebuild.sh"
echo "#!/bin/bash" > $PROJECT_ROOT/build/quickrebuild.sh

echo "rm CMakeCache.txt && make clean && cmake -DCMAKE_BUILD_TYPE=Debug -DZMQ_INCLUDE=$PROJECT_ROOT/lib/zeromq/include -DZMQ_LIBRARY=$PROJECT_ROOT/lib/zeromq/lib/libzmq.so .. && make" >> $PROJECT_ROOT/build/quickrebuild.sh

chmod u+x $PROJECT_ROOT/build/quickrebuild.sh

echo "## Enter 'make' in the 'build' subdirectory to build all examples. ##"
echo "Use build/quickrebuild.sh for complete instantanious rebuilds of the application core"
