#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"


#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

//UART defines
#define RX 0 
#define TX 1

#define uart0   ((uart_inst_t *)uart0_hw)

//CSV COLUMNS: Time (s),Altitude (ft),Vertical velocity (ft/s),Vertical acceleration (G),Position East of launch (ft),Position North of launch (ft)
int main()
{
    stdio_init_all();

    uart_init(uart0, 9600);

    std::ifstream simData("simdata/sim.csv"); 
    
    if(!simData.is_open()) throw std::runtime_error("Could not open file");
    // Read the column names

    std::string line, colname;

    while(simData.good())
    {
        // Extract the first line in the file
        std::getline(simData, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname,',')){ // Processing each line 
            float f = std::stof(colname);

            uint8_t *array;
            array = reinterpret_cast<uint8_t*>(&f); 

            uart_write_blocking(uart0, array, 4); 
        }
    }
    return 0;
}
