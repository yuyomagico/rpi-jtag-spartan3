#include "jtag_port.h"
using namespace jtag_au;
JTAGPort::JTAGPort(int TCK, int TMS, int TDI, int TDO){
    this->TCK = TCK;
    this->TMS = TMS;
    this->TDI = TDI;
    this->TDO = TDO;
    if (wiringPiSetup () == -1){
        std::cout << "Error: WiringPI failed to Initialize" << std::endl;
    }

    pinMode (TCK, OUTPUT) ;         // aka BCM_GPIO pin 17
    pinMode (TDI, OUTPUT);
    pinMode (TDO, INPUT);
    pinMode (TMS, OUTPUT);
    std::cout << "JTAG Port Initialized" << std::endl;
}

JTAGPort::~JTAGPort(){
    std::cout << "JTAG Port Destroyed" << std::endl;
}

void JTAGPort::clockFall(){
	//delay(0.001);
	digitalWrite(this->TCK,0);
}

void JTAGPort::clockRaise(){
	//delay(0.001);
	digitalWrite(this->TCK, 1);
}

void JTAGPort::setTMS(unsigned char value){
	digitalWrite(this->TMS, value);
}

void JTAGPort::setTDI(unsigned char value){
	digitalWrite(this->TDI, value);
}

unsigned char JTAGPort::getTDO(){
	return digitalRead(this->TDO);
}