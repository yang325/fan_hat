/*****************************************************************************
* | File      	:	OLED_Driver.h
* | Author      :   Waveshare team
* | Function    :	OLED define
* | Info        :	
*----------------
* |	This version:   V1.0
* | Date        :   2020-04-01
* | Info        :   Basic version
******************************************************************************/
#ifndef __OLED_H
#define __OLED_H		

#include "SSD1306.h"

#define OLED_Init() SSD1306_Init()
#define OLED_Clear() SSD1306_Clear()
#define OLED_Display(Buf) SSD1306_Display(Buf)

#endif  
	 