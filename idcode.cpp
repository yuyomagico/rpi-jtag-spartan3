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

int main (void)
{
	printf ("JTAG IDCODE.\n") ;
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
	return 0 ;
}
