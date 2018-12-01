#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tap_controller.h"
#include "xilinx_devices.h"

using namespace std;
using namespace jtag_au;

#define TCK 7
#define TMS 0
#define TDI 2
#define TDO 3

XC3S200 xcs200;
XFC01S xfc01s;

int main (int argc, char **argv)
{
	char * filename;
	printf ("Xilinx Spartan3 FPGA_PROGRAMMER.\n") ;
	if(argc < 2){
		cout << "Usage ./fpga_program <bitstream name>" << endl;
		return 1;
	}else{
		filename = argv[1];
	}
	vector<unsigned char> buffer;

	ifstream f(filename, ios::binary | ios::in);
	char c;
	long int j = 0;
	while (f.get(c))
	{
		for (int i = 7; i >= 0; i--){ // or (int i = 0; i < 8; i++)  if you want reverse bit order in bytes
			buffer.push_back((unsigned char)((c >> i) & 1));
			j++;
		}
	}
	cout << "Buffer Size " << buffer.size() << endl;

	//Validate first 32 bits 
	for(int i=0; i<32; i++){
		cout << (int)buffer[i];
	}
	cout << endl;

	JTAGPort* jtagPort = new JTAGPort(TCK, TMS, TDI, TDO);
	TapController tapcontroller(jtagPort);

	// GET IDCODES
	tapcontroller.ResetTAPandGoIdle();
	tapcontroller.GoToCaptureIR();
	tapcontroller.LoadTDIBuffer(xcs200.IDCODE);//Last device Instruction in chain goes first
	tapcontroller.LoadTDIBuffer(xfc01s.IDCODE);//First device Instruction in chain goes last
	tapcontroller.ShiftTDIBuffer();
	tapcontroller.UpdateAndGoIdle();
	tapcontroller.GoToCaptureDR();
	tapcontroller.ReadTDOBuffer(64);//2x 32-bit responses expected
	tapcontroller.UpdateAndGoIdle();

	tapcontroller.PrintTDOBuffer();

	// SET XFC BYPASS AND GET IDCODE OF XC3S200
	tapcontroller.GoToCaptureIR();
	tapcontroller.LoadTDIBuffer(xcs200.IDCODE);
	tapcontroller.LoadTDIBuffer(xfc01s.BYPASS);
	tapcontroller.ShiftTDIBuffer();
	tapcontroller.UpdateAndGoIdle();
	tapcontroller.GoToCaptureDR();
	tapcontroller.ReadTDOBuffer(32);//1x 32-bit responses expected
	tapcontroller.UpdateAndGoIdle();

	tapcontroller.PrintTDOBuffer();

	// LOAD JPROGRAM TO FPGA
	vector<unsigned char> bypassextrabit = {0};
	//EXTRA BIT SHIFTED ACCORDING TO https://www.xilinx.com/support/documentation/application_notes/xapp188.pdf
	//PAGE 12 

	tapcontroller.GoToCaptureIR();
	tapcontroller.LoadTDIBuffer(xcs200.CFG_IN);
	tapcontroller.LoadTDIBuffer(xfc01s.BYPASS);
	tapcontroller.ShiftTDIBuffer();

	tapcontroller.UpdateAndGoCaptureDR();
	tapcontroller.LoadTDIBuffer(bypassextrabit);
	tapcontroller.LoadTDIBuffer(buffer);
	tapcontroller.ShiftTDIBuffer();

	tapcontroller.UpdateAndGoCaptureIR();
	tapcontroller.LoadTDIBuffer(xcs200.JSTART);
	tapcontroller.LoadTDIBuffer(xfc01s.BYPASS);
	tapcontroller.ShiftTDIBuffer();

	tapcontroller.UpdateAndGoCaptureDR();
	tapcontroller.ReadTDOBuffer(14);
	tapcontroller.UpdateAndGoIdle(); 
	tapcontroller.PrintTDOBuffer();

	//tapcontroller.ResetTAP();

	cout << "Outputting TCK Cycles until DONE Light" << endl;
	for(int i=0; i<100;i++){
		jtagPort->clockFall();
		jtagPort->clockRaise();
	}
	return 0 ;
}
