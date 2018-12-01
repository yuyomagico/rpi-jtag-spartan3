#include "tap_controller.h"
using namespace jtag_au;
TapController::TapController(JTAGPort* jtagPort){
    this->jtagPort = jtagPort;
    std::cout << "Tap Controller Initialized" << std::endl;
}

TapController::~TapController(){
    this->jtagPort->~JTAGPort();
    std::cout << "Tap Controller Destroyed" << std::endl;
}


void TapController::ResetTAP(){
    //std::cout << "Reseting TAP Controller" << std::endl;
    unsigned char a[] = {1,1,1,1,1};
    this->ShiftTMSBuffer(a,5);
}

void TapController::ResetTAPandGoIdle(){
    //std::cout << "Reseting TAP Controller" << std::endl;
    unsigned char a[] = {1,1,1,1,1,0};
    this->ShiftTMSBuffer(a,6);
}
void TapController::GoToCaptureIR(){
    //std::cout << "Going to Capture IR" << std::endl;
    unsigned char a[] = {1,1,0,0};
    this->ShiftTMSBuffer(a,4);
}
void TapController::GoToCaptureDR(){
    //std::cout << "Going to Capture DR" << std::endl;
    unsigned char a[] = {1,0,0};
    this->ShiftTMSBuffer(a,3);
}
void TapController::UpdateAndGoIdle(){
    //std::cout << "Updating IR/DR and going to Idle" << std::endl;
    unsigned char a[] = {1,0};
    this->ShiftTMSBuffer(a,2);
}
void TapController::UpdateAndGoCaptureDR(){
    //std::cout << "Updating IR/DR and going to Idle" << std::endl;
    unsigned char a[] = {1,1,0,0};
    this->ShiftTMSBuffer(a,4);
}
void TapController::UpdateAndGoCaptureIR(){
    //std::cout << "Updating IR/DR and going to Idle" << std::endl;
    unsigned char a[] = {1,1,1,0,0};
    this->ShiftTMSBuffer(a,5);
}


void TapController::ShiftTMSBuffer(unsigned char * newBuffer, int length){
    //std::cout << "Shifting TMS Buffer " << length << std::endl;
    for(int i = 0; i<length; i++){
        this->jtagPort->clockFall();
        this->jtagPort->setTMS(newBuffer[i]);
        this->jtagPort->clockRaise();
    }
}
void TapController::LoadTDIBuffer(std::vector<unsigned char> newBuffer){
    this->tdiBuffer.push_back(newBuffer);
}
void TapController::ShiftTDIBuffer(){
    std::cout << "Shifting TDI Buffer with " << this->tdiBuffer.size() << " bits" << std::endl;
    long int total_shifts = 0;
    long int max_shifts = 0;
    for(int i = 0; i<this->tdiBuffer.size(); i++){
        max_shifts += tdiBuffer[i].size();
    }

    for(int i = 0; i<this->tdiBuffer.size(); i++){
        for(int j=0; j<this->tdiBuffer[i].size(); j++){
            this->jtagPort->clockFall();
            this->jtagPort->setTDI(tdiBuffer[i][j]);
            if(i == tdiBuffer.size() -1 && j == tdiBuffer[i].size() -1){
                this->jtagPort->setTMS(1);
            }
            this->jtagPort->clockRaise();
            total_shifts++;
            if(total_shifts % TDI_BITS_NOTIFY == 0 || total_shifts >= max_shifts){
                std::cout << "Progress: " << (double)total_shifts/max_shifts*100 << '%' << std::endl;
            }
        }
    }
    this->tdiBuffer.clear();
}
void TapController::ReadTDOBuffer(int datalength){
    this->ReadTDOBuffer(datalength, 0);
}
void TapController::ReadTDOBuffer(int datalength, int offset){
    std::cout << "Reading TDO Buffer " << datalength << std::endl;
    this->tdoBuffer.clear();
    for(int i=0;i<datalength;i++){
        if(i>= offset){
            this->jtagPort->clockFall();
            this->tdoBuffer.push_back(this->jtagPort->getTDO());
            if(i == datalength -1){
                this->jtagPort->setTMS(1);
            }
            this->jtagPort->clockRaise();
        }
    }
}

void TapController::PrintTDOBuffer(){
    int bits = 0;
    int hexs = 0;
    
    for(int i=this->tdoBuffer.size()-1; i >= 0 ; i--){
	    if(bits==0){
            unsigned char value = this->tdoBuffer[i]*8 + this->tdoBuffer[i-1]*4 + 
			this->tdoBuffer[i-2]*2 + this->tdoBuffer[i-3];
            //std::cout << this->tdoBuffer->back();
            //this->tdoBuffer->pop();
            printf("%x",value);
            hexs++;
            if(hexs % 2 == 0)
                std::cout << " ";
        }
        bits=(bits+1)%4;
    }
	std::cout << std::endl;
}