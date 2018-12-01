#include <wiringPi.h>
#include <iostream>
#include <vector>

namespace jtag_au {
    class JTAGPort {
        public:
            JTAGPort(int, int, int, int);
            ~JTAGPort();
            void clockFall();
            void clockRaise();
            void setTMS(unsigned char);
            void setTDI(unsigned char);
            unsigned char getTDO();
        private:
            int TCK, TMS, TDI, TDO;
            std::vector<unsigned char *> * instructionPipe;
    };
}