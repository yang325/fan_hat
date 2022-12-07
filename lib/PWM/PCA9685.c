#include "PCA9685.h"

/******************************************************************************
function:	
	Set duty cycle
parameter	:
	channel : Channel number
	on      : Drive output turn-on time 
	off     : PWM duty cycle 
******************************************************************************/
static void PCA9685_SetPwmDutyCycle(UBYTE channel, UWORD on, UWORD off)
{	
    DEV_I2C_WriteByte(PCA9685_I2C_Addr, LED0_ON_L + 4 * channel, on & 0xFF);
    DEV_I2C_WriteByte(PCA9685_I2C_Addr, LED0_ON_H + 4 * channel, on >> 8);
    DEV_I2C_WriteByte(PCA9685_I2C_Addr, LED0_OFF_L + 4 * channel, off & 0xFF);
    DEV_I2C_WriteByte(PCA9685_I2C_Addr, LED0_OFF_H + 4 * channel, off >> 8);
}

/******************************************************************************
function:	
	Set duty cycle
parameter	:
	channel : Channel number
	value   : Set value 0 - 100 
******************************************************************************/
void PCA9685_setPWM(UBYTE channel, UWORD value)
{
	// value = value * 4096 / 100;
	UWORD x = value * 4095 / 100;
    PCA9685_SetPwmDutyCycle(channel, 0, x);
}

/******************************************************************************
function:	
	PCA9685 initialization 
******************************************************************************/
void PCA9685_Init(UWORD pwmhz)
{
	UDOUBLE prescaleval, oldmode;
	
	DEV_I2C_WriteByte(PCA9685_I2C_Addr, MODE1, 0x00);
	prescaleval = 25000000;   
	prescaleval /= 4096;      
	prescaleval /= pwmhz;
	prescaleval -= 1.0;
	
	prescaleval=prescaleval+3;

	oldmode = DEV_I2C_ReadByte(PCA9685_I2C_Addr, MODE1);
	DEV_I2C_WriteByte(PCA9685_I2C_Addr, MODE1,  (oldmode & 0x7F) | 0x10);
	DEV_I2C_WriteByte(PCA9685_I2C_Addr, PRESCALE, prescaleval);
	DEV_I2C_WriteByte(PCA9685_I2C_Addr, MODE1, oldmode);
	DEV_Delay_ms(200);

	DEV_I2C_WriteByte(PCA9685_I2C_Addr, MODE1, oldmode | 0x80);
    DEV_I2C_WriteByte(PCA9685_I2C_Addr, MODE2, 0x04);
}

