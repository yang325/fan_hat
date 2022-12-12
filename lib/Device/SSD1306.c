/*****************************************************************************
* | File      	:	SSD1306_Driver.c
* | Author      :   Waveshare team
* | Function    :	0.9inch OLED using SSD1306 control
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-04-01
* | Info        :   Basic version
******************************************************************************/
#include "SSD1306.h"
#include <stdio.h>

static void SSD1306_WriteReg(char Reg)
{
	DEV_I2C_WriteByte(SSD1306_I2C_ADDDR, 0X00, Reg);
}

static void SSD1306_WriteData(char Data)
{
	DEV_I2C_WriteByte(SSD1306_I2C_ADDDR, 0X40, Data);
}

void SSD1306_Init(void)
{
	SSD1306_WriteReg(0xAE);

	SSD1306_WriteReg(0x40);//---set low column address 64
	SSD1306_WriteReg(0xB0);//---set high column address 176

	SSD1306_WriteReg(0xC8);//-not offset

	SSD1306_WriteReg(0x81);
	SSD1306_WriteReg(0xff);

	SSD1306_WriteReg(0xa1);

	SSD1306_WriteReg(0xa6); // Set Normal/Inverse Display (A6h/A7h) 

	SSD1306_WriteReg(0xa8);
	SSD1306_WriteReg(0x1f);

	SSD1306_WriteReg(0xd3);
	SSD1306_WriteReg(0x00);

	SSD1306_WriteReg(0xd5);
	SSD1306_WriteReg(0xf0);

	SSD1306_WriteReg(0xd9);
	SSD1306_WriteReg(0x22);

	SSD1306_WriteReg(0xda);
	SSD1306_WriteReg(0x02);

	SSD1306_WriteReg(0xdb);
	SSD1306_WriteReg(0x49);

	SSD1306_WriteReg(0x8d);
	SSD1306_WriteReg(0x14);
	
	SSD1306_WriteReg(0xA1);
	SSD1306_WriteReg(0xC8);
	
	SSD1306_WriteReg(0xaf);
}

void SSD1306_Clear(void)
{
	char Column,Page;
	char B = 0x00;
	for(Page = 0; Page < SSD1306_Page; Page++) { // 4
		SSD1306_WriteReg(0xb0 + Page);    //Set page address
		SSD1306_WriteReg(0x00);           //Set display position - column low address
		SSD1306_WriteReg(0x10);           //Set display position - column high address
		for(Column = 0; Column < SSD1306_Column; Column++){ // 128
			SSD1306_WriteData(B);
		}
	} 
}

void SSD1306_Display(UBYTE *Buf)
{	
	UBYTE newBuf[128 * 4]; // 4 page
	UBYTE temp = 0x00;
	for (UWORD y = 0; y < 4; y++) {
		for (UWORD x = 0; x < 128; x++) {
			temp = 0x00;
			for(UBYTE j = 8; j > 0; j--){
				// printf("offset = %d\r\n", x + ((j-1)*128) + (y*8*128));
				if(Buf[x + ((j-1)*128) + (y*8*128)] == 0xff){
					temp |= (1 << (j - 1));
				}
			}
			newBuf[x + y * 128] = temp;
		}
	}	
		
	char Column,Page;
	UBYTE *ptr = newBuf;
	for(Page = 0; Page < SSD1306_Page; Page++) {
		SSD1306_WriteReg(0xb0 + Page);
		SSD1306_WriteReg(0x00);
		SSD1306_WriteReg(0x10);
		for(Column = 0; Column < SSD1306_Column; Column++) {
			SSD1306_WriteData(*ptr);
			ptr++;
		}
	}
}
