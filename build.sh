#!/bin/bash
mkdir -p bin
g++ -o bin/fpga_program fpga_program.cpp tap_controller.cpp jtag_port.cpp -lwiringPi
echo "output: bin/fpga_program"
