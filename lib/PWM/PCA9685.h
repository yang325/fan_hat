#ifndef _SERVO_H_
#define _SERVO_H_

#include "../Config/DEV_Config.h"

#define SUBADR1          0x02
#define SUBADR2          0x03
#define SUBADR3          0x04
#define MODE1            0x00
#define MODE2            0x01
#define PRESCALE         0xFE
#define LED0_ON_L        0x06
#define LED0_ON_H        0x07
#define LED0_OFF_L       0x08
#define LED0_OFF_H       0x09
#define ALLLED_ON_L      0xFA
#define ALLLED_ON_H      0xFB
#define ALLLED_OFF_L     0xFC
#define ALLLED_OFF_H     0xFD

#define PCA9685_I2C_Addr 0x40
#define PWM_MAX_Hz 	 	 1000 
#define PWM_MIN_Hz 	 	 40 

void PCA9685_setPWM(UBYTE channel, UWORD value);
void PCA9685_Init(UWORD pwmhz);
#endif
