#!/bin/bash

gcc -o window window.c `pkg-config --libs raylib`