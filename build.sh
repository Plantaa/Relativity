#!/bin/bash

gcc -Wall -Wpedantic -o window window.c `pkg-config --libs raylib`