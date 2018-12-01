#include <iostream>
#include <vector>

namespace jtag_au {
    class XC3S200 {
        public:
            // Reference https://www.xilinx.com/support/documentation/user_guides/ug332.pdf
            // Page 207
            std::vector<unsigned char> 
                EXTEST = {0,0,0,0,0,0},
                IDCODE = {1,0,0,1,0,0},//REMEMBER TO FLIP BITS SHOWN IN DATASHEET.
                BYPASS = {1,1,1,1,1,1},
                CFG_IN = {1,0,1,0,0,0},
                JSTART = {0,0,1,1,0,0},
                JPROGRAM = {1,1,0,1,0,0};              
    };
    class XFC01S {
            // Reference https://www.xilinx.com/support/documentation/data_sheets/ds123.pdf
            // Page 5
        public:
            std::vector<unsigned char> 
                EXTEST = {0,0,0,0,0,0,0,0},
                IDCODE = {0,1,1,1,1,1,1,1},
                CONFIG = {0,1,1,1,0,1,1,1},
                BYPASS = {1,1,1,1,1,1,1,1};        
    };
}