# Raspberry PI JTAG for Spartan3
This project was created to be able to download the bitstream to a FPGA board with 2 chips in the JTAG chain (XFC01S PROM and XC3S200 FPGA). Like this one:  

![FPGA](https://www.picclickimg.com/d/l400/pict/202418248319_/XILINX-SPARTAN-3-XC3S200-FPGA-module-FPGA-kit-Development.jpg).

In this project I provide 2 utilites, the firstone **idcode** is to test reading the IDCODES in a JTAG chain, and the 2nd one **fpga_program** to load the bitstream into the fpga. Is super simple to extend the functionalities. I also implemented the JTAG TAP Controller to improve the control of the JTAG Chain.

Raspberry PI JTAG programmer for Spartan3 FPGA. This project is written from scratch in C/C++ mix for a RPI v1. So **Choose proper JTAG pins wirings for your RPI version in main cpp file. Pin numbers use wiringPI numeration** 

## Check Video
  <a href="http://www.youtube.com/watch?feature=player_embedded&v=aABMkHJeyaw" target="_blank"><img src="http://img.youtube.com/vi/aABMkHJeyaw/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

# Programming the FPGA
You can easily program your FPGA following the steps described below. If you have a different fpga or more devices in the chain, you can just add the required registers in the **xilinx_devices.h** file or a new one if you want. Remember to always chech the manufacturer guides to get the Registers right.

## Prerequisites
* wiringPI library

## Usage
1. [Install wiringPI library](http://wiringpi.com/download-and-install/)
2. Clone repository `git clone https://github.com/yuyomagico/rpi-jtag-spartan3`
3. **Check your RPI Version and check pin numbers for JTAG port in "idcode.cpp"**
4. `./idcode_build.sh` to build the **idcode** and `./build.sh` to build **fpga_program**.
5. Outputs will be written into `bin/`
6. Usage: `./bin/idcode` and `./bin/fpga_program <filename>`
