#include "display.h"

// Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	TA1CTL = TASSEL__ACLK | MC_0  ;                      // hailts the timer
	__bic_SR_register_on_exit(LPM3_bits);
}

void delay_ms(int ms)
{
	TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	TA1CCR0 = ms*1000;						  // 1MHz
	TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS;   // ACLK, continuous mode

	__bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, enable interrupts
	__no_operation();                         // For debugger
}

void delay_ns(int ns)
{
	TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	TA1CCR0 = ns*1000;						  // 1MHz
	TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS;   // ACLK, continuous mode

	__bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, enable interrupts
	__no_operation();                         // For debugger
}

void LCD_set_reg(uint8_t b1, uint8_t reg)
{
	P6DIR |= 0x7F;					//Set P6.0 to P6.6 as output
	P6DIR |= BIT0;					//Set P7.0 as output

	P2OUT &= ~ BIT3;				//Write 0 to LCD_RS

	P6OUT &= 0xFF80;				//Clear P6.0 to P6.6
	P6OUT |= (b1 >> 1);				//Write data[0,6] to P6.0 to P6.6
	P7OUT &= ~ BIT0;				//Clear P7.0
	P6OUT |= (b1 & ~ BIT0);			//Write data(7) to P7.0
	P9OUT &= ~BIT4;					//Write 0 to LCD_WR
	delay_ns(200);
	P9OUT |= BIT4;					//Write 1 to LCD_WR
	delay_ns(500);

	P6OUT &= 0xFF80;				//Clear P6.0 to P6.6
	P6OUT |= (reg >> 1);			//Write data[0,6] to P6.0 to P6.6
	P7OUT &= ~ BIT0;				//Clear P7.0
	P6OUT |= (reg & ~ BIT0);		//Write data(7) to P7.0
	P9OUT &= ~BIT4;					//Write 0 to LCD_WR
	delay_ns(200);
	P9OUT |= BIT4;					//Write 1 to LCD_WR
	delay_ns(100);

	P2OUT |= BIT3;					//Write 1 to LCD_RS

}


void LCD_setup()
{
	P7DIR |= BIT1;					//Set LCD_RESET(P7.1) as output
	P9DIR |= BIT7|BIT5|BIT4;		//Set LCD_CS, LCD_WR and LCD_WR (P9.7, P9.5 and P9.4) as output
}

void LCD_initialize()
{
	P7OUT |= BIT1;					//Write 1 to LCD_RESET
	delay_ms(5);
	P7OUT &= ~BIT1;  ;				//Write 0 to LCD_RESET
	delay_ms(5);
	P7OUT |= BIT1;					//Write 1 to LCD_RESET
	delay_ms(5);
	P9OUT |= BIT7|BIT4;				//Write 1 to LCD_CS and LCD_WR
	delay_ms(5);
	P9OUT &= ~BIT7;					//Write 0 to LCD_CS

	LCD_set_reg(0x00,0xE5);
	LCD_write_data(0x78,0xF0); // set SRAM internal timing
	LCD_set_reg(0x00,0x01);
	LCD_write_data(0x01,0x00); // set SS and SM bit
	LCD_set_reg(0x00,0x02);
	LCD_write_data(0x07,0x00); // set 1 line inversion
	LCD_set_reg(0x00,0x03);
	LCD_write_data(0x10,0x30); // set GRAM write direction and BGR=1.
	LCD_set_reg(0x00,0x04);
	LCD_write_data(0x00,0x00); // Resize register
	LCD_set_reg(0x00,0x08);
	LCD_write_data(0x02,0x07); // set the back porch and front porch
	LCD_set_reg(0x00,0x09);
	LCD_write_data(0x00,0x00); // set non-display area refresh cycle ISC[3:0]
	LCD_set_reg(0x00,0x0A);
	LCD_write_data(0x00,0x00); // FMARK function
	LCD_set_reg(0x00,0x0C);
	LCD_write_data(0x00,0x00); // RGB interface setting
	LCD_set_reg(0x00,0x0D);
	LCD_write_data(0x00,0x00); // Frame marker Position
	LCD_set_reg(0x00,0x0F);
	LCD_write_data(0x00,0x00); // RGB interface polarity

	//*************Power On sequence ****************//

	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x00,0x00); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x00,0x07); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x00); // VREG1OUT voltage
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x00,0x00); // VDV[4:0] for VCOM amplitude
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x00,0x01);
	delay_ms(50); // Dis-charge capacitor power voltage
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x10,0x90); // 1490//SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x02,0x27); // DC1[2:0], DC0[2:0], VC[2:0]
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x1F); //001C// Internal reference voltage= Vci;
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x15,0x00); //0x1000//1400   Set VDV[4:0] for VCOM amplitude  1A00
	LCD_set_reg(0x00,0x29);
	LCD_write_data(0x00,0x27); //0x0012 //001a  Set VCM[5:0] for VCOMH  //0x0025  0034
	LCD_set_reg(0x00,0x2B);
	LCD_write_data(0x00,0x0D); // Set Frame Rate   000C
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x20);
	LCD_write_data(0x00,0x00); // GRAM horizontal Address
	LCD_set_reg(0x00,0x21);
	LCD_write_data(0x00,0x00); // GRAM Vertical Address

	// ----------- Adjust the Gamma Curve ----------//

	LCD_set_reg(0x00,0x30);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x31);
	LCD_write_data(0x07,0x07);
	LCD_set_reg(0x00,0x32);
	LCD_write_data(0x03,0x07);
	LCD_set_reg(0x00,0x35);
	LCD_write_data(0x02,0x00);
	LCD_set_reg(0x00,0x36);
	LCD_write_data(0x00,0x08);//0207
	LCD_set_reg(0x00,0x37);
	LCD_write_data(0x00,0x04);//0306
	LCD_set_reg(0x00,0x38);
	LCD_write_data(0x00,0x00);//0102
	LCD_set_reg(0x00,0x39);
	LCD_write_data(0x07,0x07);//0707
	LCD_set_reg(0x00,0x3C);
	LCD_write_data(0x00,0x02);//0702
	LCD_set_reg(0x00,0x3D);
	LCD_write_data(0x1D,0x04);//1604

	//------------------ Set GRAM area ---------------//

	LCD_set_reg(0x00,0x50);
	LCD_write_data(0x00,0x00); // Horizontal GRAM Start Address
	LCD_set_reg(0x00,0x51);
	LCD_write_data(0x00,0xEF); // Horizontal GRAM End Address
	LCD_set_reg(0x00,0x52);
	LCD_write_data(0x00,0x00); // Vertical GRAM Start Address
	LCD_set_reg(0x00,0x53);
	LCD_write_data(0x01,0x3F); // Vertical GRAM Start Address
	LCD_set_reg(0x00,0x60);
	LCD_write_data(0xA7,0x00); // Gate Scan Line
	LCD_set_reg(0x00,0x61);
	LCD_write_data(0x00,0x01); // NDL,VLE, REV
	LCD_set_reg(0x00,0x6A);
	LCD_write_data(0x00,0x00); // set scrolling line

	//-------------- Partial Display Control ---------//

	LCD_set_reg(0x00,0x80);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x81);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x82);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x83);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x84);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x85);
	LCD_write_data(0x00,0x00);

	//-------------- Panel Control -------------------//

	LCD_set_reg(0x00,0x90);
	LCD_write_data(0x00,0x10);
	LCD_set_reg(0x00,0x92);
	LCD_write_data(0x06,0x00);
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x01,0x33); // 262K color and display ON
}


void LCD_write_data(uint8_t dataH, uint8_t dataL)
{
	P6DIR |= 0x7F;					//Set P6.0 to P6.6 as output
	P6DIR |= BIT0;					//Set P7.0 as output

	P6OUT &= 0xFF80;				//Clear P6.0 to P6.6
	P6OUT |= (dataH >> 1);			//Write data[0,6] to P6.0 to P6.6
	P7OUT &= ~ BIT0;				//Clear P7.0
	P6OUT |= (dataH & ~ BIT0);		//Write data(7) to P7.0
	P9OUT &= ~BIT4;					//Write 0 to LCD_WR
	delay_ns(200);
	P9OUT |= BIT4;					//Write 1 to LCD_WR
	delay_ns(500);
	P6OUT &= 0xFF80;				//Clear P6.0 to P6.6
	P6OUT |= (dataL >> 1);			//Write data[0,6] to P6.0 to P6.6
	P7OUT &= ~ BIT0;				//Clear P7.0
	P6OUT |= (dataL & ~ BIT0);		//Write data(7) to P7.0
	P9OUT &= ~BIT4;					//Write 0 to LCD_WR
	delay_ns(200);
	P9OUT |= BIT4;					//Write 1 to LCD_WR
	delay_ns(100);

}

int LCD_read_data()
{
	uint8_t data_H = 0x00,data_L = 0x00;
	int data_out = 0x0000;
	P6DIR &= ~0x7F;					//Set P6.0 to P6.6 as input
	P6DIR &= ~BIT0;					//Set P7.0 as input

	P9OUT &= ~BIT5;					//Write 0 to LCD_RD
	data_H |= P6IN;
	data_H = data_H>>1;
	data_H |= (P7IN & BIT0);
	P9OUT |= BIT5;
	delay_ns(100);
	P9OUT &= ~BIT5;					//Write 0 to LCD_RD
	data_L |= P6IN;
	data_L = data_L>>1;
	data_L |= (P7IN & BIT0);
	P9OUT |= BIT5;
	data_out |= data_H;
	data_out = data_out<<8;
	data_out |= data_L;

	return data_out;
}
