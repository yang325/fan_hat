/*****************************************************************************
* | File      	:	SSD1306_Driver.h
* | Author      :   Waveshare team
* | Function    :	0.9inch OLED using SSD1306 control
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-04-01
* | Info        :   Basic version
******************************************************************************/
#ifndef __SSD1306_H
#define __SSD1306_H

#include "../Config/DEV_Config.h"

//Define the full screen height length of the display
#define SSD1306_I2C_ADDDR  0x3c

#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  32
#define SSD1306_Column	SSD1306_WIDTH
#define SSD1306_Page	(SSD1306_HEIGHT/8)

/*
end
******************************************************************************/
void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_Display(UBYTE *Buf);

#endif





