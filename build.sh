#!/bin/sh

rm -f config.h
rm -f *.rej
make clean
make
sudo make install
