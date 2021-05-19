#include "stm8s.h"

void i2c_start_()
{
	I2C_CR2 |= I2C_START;
	while (!(I2C_SR1 & I2C_SB)); 	// wait for START bit sent
}

void i2c_write_(unsigned char ch)
{
	I2C_DR = ch;
	while (!(I2C_SR1 & I2C_TXE)); 	// wait for TXE Data register empty
}

void i2c_addr_(unsigned char addr)
{
	i2c_write_(addr); 				// addres
	while (!(I2C_SR3 & 1)); 		// MSL. Check master mode 
}

void i2c_stop_()
{
	//while (!(I2C_SR1 & I2C_TXE)); // wait for TXE Data register empty 
	//while (!(I2C_SR1 & I2C_BTF)); // wait for BTF Byte transfer finished  
	I2C_CR2 |= I2C_STOP;
	while (I2C_SR3 & 1); 			// MSL. Check slave mode 
}

void delay(unsigned short ms)
{
	unsigned short d;
	for(ms; ms>0; ms--)
	{
		for(d=0; d<2286; d++);
	}
}

//-----main-----
int main() 
{
	//-----init-----
	CLK_CKDIVR = 0x00; 		// Set the frequency to 16 MHz, fCPU=fMASTER
	CLK_PCKENR1 |= 0x01; 	// Enable I2C clock
	I2C_FREQR = 0x10; 		// Set the I2C peripheral frequency to 16 MHz 
	I2C_CCRL = 0x50;		// CCR = 80 * (1/16MHz)   = 5000ns
	I2C_TRISER = 0x11;		// 
	I2C_CR1 |= I2C_PE;		// Enable I2C peripheral
	
	PB_DDR = 0b00110000; 	// SDA PB5, SCL PB4, True open drain pins
	//-----*init-----
	
	delay(100); //some displays needs this delay.
	
	////-----OLED init-----
	//i2c_start_();
	//i2c_addr_(0b01111000);	// OLED address 0x78
	//i2c_write_(0x00);			// OLED control byte, Co = 0, D/C = 0
	//i2c_write_(0xA8); 		// OLED command
	//i2c_write_(0x3F); 		// OLED value
	//i2c_write_(0xD3); 		// OLED command
	//i2c_write_(0x00); 		// OLED value
	//i2c_write_(0x40); 		// OLED command
	//i2c_write_(0xA0); 		// OLED command
	//i2c_write_(0xC0); 		// OLED command
	//i2c_write_(0xDA); 		// OLED command
	//i2c_write_(0x10); 		// OLED value
	//i2c_write_(0x81); 		// OLED command
	//i2c_write_(0x7F); 		// OLED value
	//i2c_write_(0xA4); 		// OLED command
	//i2c_write_(0xA6); 		// OLED command
	//i2c_write_(0xD5); 		// OLED command
	//i2c_write_(0x80); 		// OLED value
	//i2c_write_(0x8D); 		// OLED command
	//i2c_write_(0x14); 		// OLED value
	//i2c_write_(0xAF); 		// OLED command
	//i2c_stop_();
	////-----*OLED init-----

	//Move to page 0
	i2c_start_();
	i2c_addr_(0b01111000);	// OLED address 0x78
	i2c_write_(0x00);		// OLED control byte, Co = 0, D/C = 0
	i2c_write_(0xB0); 		// OLED command, Set Page Start Address for Page Addressing Mode (B0h~B7h)
	i2c_stop_();

	i2c_start_();
	i2c_addr_(0b01111000);	// OLED address 0x78
	i2c_write_(0x40);		// OLED control byte, Co = 0, D/C = 1
	
	unsigned short k;
	
	// Display checkered pattern
	for (k=0; k<29; k++) 	
	{
		i2c_write_(0xAA); 	// OLED data
		i2c_write_(0x55); 	// OLED data
	}
	for (k=0; k<4; k++) 	
	{
		i2c_write_(0x00); 	// OLED data
	}
	
	// Display letter "A"
	i2c_write_(0b00000000); 	// OLED data
	i2c_write_(0b01111110); 	// OLED data
	i2c_write_(0b00001001); 	// OLED data
	i2c_write_(0b00001001); 	// OLED data
	i2c_write_(0b00001001); 	// OLED data
	i2c_write_(0b01111110); 	// OLED data
	i2c_write_(0b00000000); 	// OLED data
	
	// Display letter "B"
	i2c_write_(0b00000000); 	// OLED data
	i2c_write_(0b01111111); 	// OLED data
	i2c_write_(0b01001001); 	// OLED data
	i2c_write_(0b01001001); 	// OLED data
	i2c_write_(0b01001001); 	// OLED data
	i2c_write_(0b00110110); 	// OLED data
	i2c_write_(0b00000000); 	// OLED data
	
	// Display stripe pattern
	for (k=0; k<4; k++) 	
	{
		i2c_write_(0x00); 	// OLED data
	}
	for (k=0; k<20; k++) 	
	{
		i2c_write_(0x55); 	// OLED data
	}
	for (k=0; k<4; k++) 	
	{
		i2c_write_(0x00); 	// OLED data
	}
	for (k=0; k<10; k++) 	
	{
		i2c_write_(0xFF); 	// OLED data
		i2c_write_(0x00); 	// OLED data
	}
	for (k=0; k<4; k++) 	
	{
		i2c_write_(0x00); 	// OLED data
	}
	
	i2c_stop_();
	
	//Move to page 7
	i2c_start_();
	i2c_addr_(0b01111000);	// OLED address 0x78
	i2c_write_(0x00);		// OLED control byte, Co = 0, D/C = 0
	i2c_write_(0xB7); 		// OLED command, Set Page Start Address for Page Addressing Mode (B0h~B7h)
	i2c_stop_();
	
	i2c_start_();
	i2c_addr_(0b01111000);	// OLED address 0x78
	i2c_write_(0x40);		// OLED control byte, Co = 0, D/C = 1
	
	unsigned short l;
	for (k=0; k<8; k++) 	
	{
		for (l=0; l<8; l++) 	
		{
			i2c_write_(0x00); 	// OLED data
		}
		for (l=0; l<8; l++) 	
		{
			i2c_write_(0xFF); 	// OLED data
		}
	}
	
    i2c_stop_();
	
	i2c_start_();
	i2c_addr_(0b01111000);	// OLED address 0x78
	i2c_write_(0x00);		// OLED control byte, Co = 0, D/C = 0
	i2c_write_(0xA4); 		// OLED command, Entire Display ON (A4h/A5h) 
	i2c_write_(0x81); 		// OLED command, Set Contrast Control for BANK0
	i2c_write_(0xEE); 		// OLED value
	i2c_write_(0xD5); 		// OLED command, Set Display Clock Divide Ratio/ Oscillator Frequency
	i2c_write_(0xF1); 		// OLED value 
	i2c_write_(0x8D); 		// OLED command, Enable charge pump during display on
	i2c_write_(0x14); 		// OLED value, Enable Charge Pump
	i2c_write_(0xAF); 		// OLED command, Set Display ON
	
	while (1)
	{
		i2c_write_(0xA6); 		// OLED command, Set Normal/Inverse Display (A6h/A7h)
		delay(1500);
		i2c_write_(0xA5); 		// OLED command, Entire Display ON (A4h/A5h)
		delay(1500);
		i2c_write_(0xA4); 		// OLED command, Entire Display ON (A4h/A5h)
		delay(1500);
		i2c_write_(0xA7); 		// OLED command, Set Normal/Inverse Display (A6h/A7h)
		delay(1500);
	}
}
