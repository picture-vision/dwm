#!/bin/sh

rm -f config.h
rm -f *.orig
rm -f *.rej
make clean
make
sudo make install
[ -d /usr/share/xsessions ] && sudo cp ./resources/usr/share/xsessions/dwm.desktop /usr/share/xsessions/
# sudo cp -f ./scripts/* /usr/local/bin/
