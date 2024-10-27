#!/bin/bash

apt update && apt upgrade -y
apt install $(cat dependencies.txt) -y

git clone https://github.com/raysan5/raylib.git --depth=1 ${HOME}/raylib
cd ${HOME}/raylib
mkdir build && cd build
cmake -DEBUILD_SHARED_LIBS=ON ..
make
make install
ldconfig
