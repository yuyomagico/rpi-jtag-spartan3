#include <iostream>
#include <vector>
#include <map>
#include "jtag_port.h"
#define TDI_BITS_NOTIFY 15000

//using namespace std;
namespace jtag_au {
    class TapController {
        public:
            TapController(JTAGPort*);
            ~TapController();
            enum States {TEST_LOGIC_RESET, RUN_TEST_IDLE,
                        SELECT_DR_SCAN, CAPTURE_DR, SHIFT_DR, EXIT1_DR, PAUSE_DR, EXIT2_DR, UPDATE_DR,
                        SELECT_IR_SCAN, CAPTURE_IR, SHIFT_IR, EXIT1_IR, PAUSE_IR, EXIT2_IR, UPDATE_IR
                        };
            enum OutputBuffers {TMS_BUFFER, TDI_BUFFER};
            void ResetTAP();
            void ResetTAPandGoIdle();
            void GoToCaptureIR();
            void GoToCaptureDR();
            void UpdateAndGoIdle();
            void UpdateAndGoCaptureDR();
            void UpdateAndGoCaptureIR();

            void ShiftTMSBuffer(unsigned char *, int);
            void LoadTDIBuffer(std::vector<unsigned char>);
            void ShiftTDIBuffer();
            void ReadTDOBuffer(int); //reads and also works for shifting dr
            void ReadTDOBuffer(int, int);
            
            void PrintTDOBuffer();
        private:
            std::vector<std::vector<unsigned char>> tdiBuffer;
            std::vector<unsigned char> tmsBuffer;
            std::vector<unsigned char> tdoBuffer;
            JTAGPort* jtagPort;
            States currentState;
            std::map<States, std::string> stateNames = {
                {States::TEST_LOGIC_RESET , "TEST_LOGIC_RESET"},
                {States::RUN_TEST_IDLE , "RUN_TEST_IDLE"},
                {States::SELECT_DR_SCAN , "SELECT_DR_SCAN"},
                {States::CAPTURE_DR , "CAPTURE_DR"},
                {States::SHIFT_DR , "SHIFT_DR"},
                {States::EXIT1_DR , "EXIT1_DR"},
                {States::PAUSE_DR , "PAUSE_DR"},
                {States::EXIT2_DR , "EXIT2_DR"},
                {States::UPDATE_DR , "UPDATE_DR"},
                {States::SELECT_IR_SCAN , "SELECT_IR_SCAN"},
                {States::CAPTURE_IR , "CAPTURE_IR"},
                {States::SHIFT_IR , "SHIFT_IR"},
                {States::EXIT1_IR , "EXIT1_IR"},
                {States::PAUSE_IR , "PAUSE_IR"},
                {States::EXIT2_IR , "EXIT2_IR"},
                {States::UPDATE_IR , "UPDATE_IR"}
            };
    };
}