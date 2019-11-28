#include "mbed.h"

#define GPIOEX_ADDRESS 0x46
#define GPIOEX_NCS D10
#define OPP_GPIOMODE 0x00
#define OPP_GPIOWRITE 0x09

SPI device(SPI_MOSI, SPI_MISO, SPI_SCK);
DigitalOut ncs(GPIOEX_NCS);


void writeData(char opperation, char data) {
    ncs = 0;
    device.write(GPIOEX_ADDRESS);
    device.write(opperation);
    device.write(data);
    ncs = 1;
} 

int main() {
    ncs = 1;
    wait_ms(50);
    
    writeData(OPP_GPIOMODE, 0xC0);
    wait_ms(50);
    
    while(1) {
        writeData(OPP_GPIOWRITE, 0x3F);
        wait(1);
        
        writeData(OPP_GPIOWRITE, 0x00);
        wait(1);
    }
}
 
