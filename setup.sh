#!/bin/bash

sudo apt update && sudo apt upgrade -y
sudo apt install $(cat dependencies.txt) -y

git clone https://github.com/raysan5/raylib.git ${HOME}/raylib
cd ${HOME}/raylib
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make -j$(($(nproc) + 1))
sudo make install
ldconfig
