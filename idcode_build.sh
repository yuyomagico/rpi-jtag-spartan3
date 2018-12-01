#!/bin/bash
mkdir -p bin
g++ -o bin/idcode idcode.cpp tap_controller.cpp jtag_port.cpp -lwiringPi
echo "output: bin/idcode"
