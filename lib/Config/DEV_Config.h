/*****************************************************************************
* | File      	:   DEV_Config.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-03-12
* | Info        :   Basic version
*
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#ifdef USE_BCM2835_LIB
	#include <bcm2835.h>
#elif USE_WIRINGPI_LIB
	#include <wiringPi.h>
	#include <wiringPiI2C.h>
	#include <wiringPiSPI.h>
#elif USE_DEV_LIB
	#include "sysfs_gpio.h"
	#include "dev_hardware_I2C.h"
	#include "dev_hardware_SPI.h"
#endif

#include <stdint.h>
#include "Debug.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DEV_SPI 1
#define DEV_I2C 1

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/*------------------------------------------------------------------------------------------------------*/
UBYTE DEV_ModuleInit(void);
void DEV_ModuleExit(void);

void DEV_Delay_ms(UDOUBLE xms);

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);
void DEV_Digital_Write(UWORD Pin, UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);

void DEV_I2C_Init(void);
void DEV_I2C_WriteByte(uint8_t Addr, UBYTE Cmd, UBYTE value);
int DEV_I2C_ReadByte(uint8_t Addr, UBYTE Cmd);
int DEV_I2C_ReadWord(uint8_t Addr, UBYTE Cmd);

void DEV_SPI_Init(void);
void DEV_SPI_WriteByte(UBYTE Value);
void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len);

#endif
