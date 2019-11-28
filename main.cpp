#include "mbed.h"
#include "nRF24L01P.h"

#define GPIOEX_ADDRESS 0x46
#define GPIOEX_NCS D10
#define OPP_GPIOMODE 0x00
#define OPP_GPIOWRITE 0x09

#define NRF24_CSN D8
#define NRF24_CE D9
#define NRF24_IRQ D7

#define TRANSFER_SIZE 1
#define TRANSFER_DEVICE 1

Serial pc(USBTX, USBRX);

SPI gpioex(SPI_MOSI, SPI_MISO, SPI_SCK);
nRF24L01P nrf24(SPI_MOSI, SPI_MISO, SPI_SCK, NRF24_CSN, NRF24_CE, NRF24_IRQ);
//nRF24L01P my_nrf24l01p(p5, p6, p7, p8, p9, p10);    // mosi, miso, sck, csn, ce, irq
DigitalOut ncs(GPIOEX_NCS);

void gpioWriteData(char opperation, char data) {
    ncs = 0;
    gpioex.write(GPIOEX_ADDRESS);
    gpioex.write(opperation);
    gpioex.write(data);
    ncs = 1;
} 

void nrfWriteData(char data) {
    char data_arr[] = { data };
    nrf24.write(NRF24L01P_PIPE_P0, data_arr, 1);
}

int main() {
    printf("Power up nrf24\r\n");
    nrf24.powerUp();
    
    ncs = 1;
    wait_ms(50);
    
    gpioWriteData(OPP_GPIOMODE, 0xC0);
    wait_ms(50);
    
    printf("Configuring nrf24\r\n");
    nrf24.setTransferSize( TRANSFER_SIZE );
    nrf24.setRfFrequency(2480);
    if(TRANSFER_DEVICE) {
        printf("Starting in transfer mode\r\n");
        nrf24.setTransmitMode();
        nrf24.enable();
        
        while(1) {
            printf("Sending data\r\n");
            gpioWriteData(OPP_GPIOWRITE, 0x3F);
            nrfWriteData(0x3F);
            wait(1);
            
            gpioWriteData(OPP_GPIOWRITE, 0x00);
            nrfWriteData(0x00);
            wait(1);
        }
    } else {
        printf("Starting in receiver mode\r\n");
        nrf24.setReceiveMode();
        nrf24.enable();
        
        char data_arr[TRANSFER_SIZE]; 
        
        while(1) {
            if(nrf24.readable()) {
                nrf24.read( NRF24L01P_PIPE_P0, data_arr, TRANSFER_SIZE );
                gpioWriteData(OPP_GPIOWRITE, data_arr[0]);
            }
        } 
    }
}
