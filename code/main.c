#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "CircularBuffer/circularBuffer.h"
#include "Display/display.h"

volatile circularBuffer ecgSignalBuffer;

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	//Stop watchdog timer
	
    /*
     * Setups
     */
    clocks_setup();								//System clocks configuration
    AFE_setup();								//AFE (ADS1291) port setup
    buzzer_setup();								//Buzzer PWM configuration
    display_setup();							//Display port setup
    circularBuffer_setup(&ecgSignalBuffer);		//Ecg signal storage buffer setup

    /*
     * MCU setup
     */
    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode
    __bis_SR_register(GIE);			//Enable global interrupts

    /*
     * Initializations
     */
    AFE_initialize();				//AFE communication start up and configuration commands
    display_initialize();			//Display communication start up and configuration commands

    /*
     * Sheits
     */
    volatile int data_read = 0;
	data_read = display_IO_read_reg(0x00);

	P9OUT &= ~BIT6;				//Turn screen on

//	uint8_t DA_MUDA_FUCKING_VARIABLE_0;
//	uint8_t DA_MUDA_FUCKING_VARIABLE_1;
//	uint8_t DA_MUDA_FUCKING_VARIABLE_2;
//
//	long i;
	while(1)
	{

//		for(i = 0 ; i < 60L * 320L ; i++)
//    	{
//
//			DA_MUDA_FUCKING_VARIABLE_0 = ecgSignalBuffer.ecgBuffer[i%256].signal[0];
//			DA_MUDA_FUCKING_VARIABLE_1 = ecgSignalBuffer.ecgBuffer[i%256].signal[1];
//			DA_MUDA_FUCKING_VARIABLE_2 = ecgSignalBuffer.ecgBuffer[i%256].signal[2];
//			display_IO_write_GRAM(DA_MUDA_FUCKING_VARIABLE_0, DA_MUDA_FUCKING_VARIABLE_1, DA_MUDA_FUCKING_VARIABLE_2);
//
//    	}
//    	//delay_ms(1000);
//
//    	for(i = 0 ; i < 120L * 320L ; i++)
//    	{
//			DA_MUDA_FUCKING_VARIABLE_0 = ecgSignalBuffer.ecgBuffer[i%256].signal[0];
//			DA_MUDA_FUCKING_VARIABLE_1 = ecgSignalBuffer.ecgBuffer[i%256].signal[1];
//			DA_MUDA_FUCKING_VARIABLE_2 = ecgSignalBuffer.ecgBuffer[i%256].signal[2];
//			display_IO_write_GRAM(DA_MUDA_FUCKING_VARIABLE_0, DA_MUDA_FUCKING_VARIABLE_1, DA_MUDA_FUCKING_VARIABLE_2);
//    	}
//    	//delay_ms(1000);
//    	for(i = 0 ; i < 60L * 320L ; i++)
//    	{
//			DA_MUDA_FUCKING_VARIABLE_0 = ecgSignalBuffer.ecgBuffer[i%256].signal[0];
//			DA_MUDA_FUCKING_VARIABLE_1 = ecgSignalBuffer.ecgBuffer[i%256].signal[1];
//			DA_MUDA_FUCKING_VARIABLE_2 = ecgSignalBuffer.ecgBuffer[i%256].signal[2];
//			display_IO_write_GRAM(DA_MUDA_FUCKING_VARIABLE_0, DA_MUDA_FUCKING_VARIABLE_1, DA_MUDA_FUCKING_VARIABLE_2);
//    	}
	}

}
