/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-03-12
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"
#include <unistd.h>
#include <fcntl.h>

uint32_t fd;
/******************************************************************************
function:	GPIO Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
	/*
	    0:  INPT
	    1:  OUTP
	*/
#ifdef USE_BCM2835_LIB
	if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT) {
		bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
	} else {
		bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
	}
#elif USE_WIRINGPI_LIB
	if(Mode == 0 || Mode == INPUT) {
		pinMode(Pin, INPUT);
		pullUpDnControl(Pin, PUD_UP);
	} else {
		pinMode(Pin, OUTPUT);
	}
#elif USE_DEV_LIB
	SYSFS_GPIO_Export(Pin);
	if(Mode == 0 || Mode == SYSFS_GPIO_IN) {
		SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_IN);
	} else {
		SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_OUT);
	}
#endif
}

void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
#ifdef USE_BCM2835_LIB
	bcm2835_gpio_write(Pin, Value);

#elif USE_WIRINGPI_LIB
	digitalWrite(Pin, Value);

#elif USE_DEV_LIB
	SYSFS_GPIO_Write(Pin, Value);

#endif
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
	UBYTE Read_value = 0;
#ifdef USE_BCM2835_LIB
	Read_value = bcm2835_gpio_lev(Pin);

#elif USE_WIRINGPI_LIB
	Read_value = digitalRead(Pin);

#elif USE_DEV_LIB
	Read_value = SYSFS_GPIO_Read(Pin);
#endif
	return Read_value;
}


/******************************************************************************
function:	SPI Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_SPI_Init(void)
{
#if DEV_SPI
#ifdef USE_BCM2835_LIB
	printf("BCM2835 SPI Device\r\n");
	bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //spi mode 0
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);  //Frequency
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //set CE0
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //enable cs0

#elif USE_WIRINGPI_LIB
	printf("WIRINGPI SPI Device\r\n");
	wiringPiSPISetup(0,9000000);
	// wiringPiSPISetupMode(0, 9000000, 0);

#elif USE_DEV_LIB
	printf("DEV SPI Device\r\n");
	DEV_HARDWARE_SPI_begin("/dev/spidev0.0");
#endif
#endif
}

void DEV_SPI_WriteByte(uint8_t Value)
{
#if DEV_SPI
#ifdef USE_BCM2835_LIB
	bcm2835_spi_transfer(Value);

#elif USE_WIRINGPI_LIB
	wiringPiSPIDataRW(0,&Value,1);

#elif USE_DEV_LIB
	DEV_HARDWARE_SPI_TransferByte(Value);
#endif
#endif
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
#if DEV_SPI
#ifdef USE_BCM2835_LIB
	uint8_t rData[Len];
	bcm2835_spi_transfernb((char *)pData, (char *)rData, Len);

#elif USE_WIRINGPI_LIB
	wiringPiSPIDataRW(0, pData, Len);

#elif USE_DEV_LIB
	DEV_HARDWARE_SPI_Transfer(pData, Len);

#endif
#endif
}


/******************************************************************************
function:	I2C Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_I2C_Init(void)
{
#if DEV_I2C
#ifdef USE_BCM2835_LIB
	printf("BCM2835 I2C Device\r\n");
	bcm2835_i2c_begin();

#elif USE_WIRINGPI_LIB
	printf("WIRINGPI I2C Device\r\n");

#elif USE_DEV_LIB
	printf("DEV I2C Device\r\n");
	DEV_HARDWARE_I2C_begin("/dev/i2c-1");
#endif
#endif
}

void DEV_I2C_WriteByte(uint8_t Addr, uint8_t Cmd, uint8_t value)
{
#if DEV_I2C
#ifdef USE_BCM2835_LIB
	bcm2835_i2c_setSlaveAddress(Addr);
	char wbuf[2]= {Cmd, value};
	bcm2835_i2c_write(wbuf, 2);
#elif USE_WIRINGPI_LIB
	fd = wiringPiI2CSetup(Addr);
	int ref;
	ref = wiringPiI2CWriteReg8(fd, (int)Cmd, (int)value);
	while(ref != 0) {
		ref = wiringPiI2CWriteReg8 (fd, (int)Cmd, (int)value);
		if(ref == 0)
			break;
	}
#elif USE_DEV_LIB
	DEV_HARDWARE_I2C_setSlaveAddress(Addr);
	char wbuf[2]= {Cmd, value};
	DEV_HARDWARE_I2C_write(wbuf, 2);

#endif
#endif
}

int DEV_I2C_ReadByte(uint8_t Addr, uint8_t Cmd)
{
	int ref = 0;
#if DEV_I2C
#ifdef USE_BCM2835_LIB
	bcm2835_i2c_setSlaveAddress(Addr);
	char rbuf[2]= {0};
	bcm2835_i2c_read_register_rs((char *)&Cmd, rbuf, 1);
	ref = rbuf[0];

#elif USE_WIRINGPI_LIB
	fd = wiringPiI2CSetup(Addr);
	ref = wiringPiI2CReadReg8 (fd, (int)Cmd);

#elif USE_DEV_LIB
	DEV_HARDWARE_I2C_setSlaveAddress(Addr);
	char rbuf[2]= {0};
	DEV_HARDWARE_I2C_read(Cmd, rbuf, 1);
	ref = rbuf[0];
#endif
#endif
	return ref;
}

int DEV_I2C_ReadWord(uint8_t Addr, uint8_t Cmd)
{
	int ref = 0;
#if DEV_I2C
#ifdef USE_BCM2835_LIB
	bcm2835_i2c_setSlaveAddress(Addr);
	char rbuf[2] = {0};
	bcm2835_i2c_read_register_rs((char *)&Cmd, rbuf, 2);
	ref = rbuf[1]<<8 | rbuf[0];

#elif USE_WIRINGPI_LIB
	fd = wiringPiI2CSetup(Addr);
	ref = wiringPiI2CReadReg16 (fd, (int)Cmd);

#elif USE_DEV_LIB
	DEV_HARDWARE_I2C_setSlaveAddress(Addr);
	char rbuf[2] = {0};
	DEV_HARDWARE_I2C_read(Cmd, rbuf, 2);
	ref = rbuf[1]<<8 | rbuf[0];
#endif
#endif
	return ref;
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_ModuleInit(void)
{
#ifdef USE_BCM2835_LIB
	if(!bcm2835_init()) {
		printf("bcm2835 init failed  !!! \r\n");
		return 1;
	} else {
		printf("bcm2835 init success !!! \r\n");
	}

#elif USE_WIRINGPI_LIB
	//if(wiringPiSetup() < 0)//use wiringpi Pin number table
	if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
		printf("set wiringPi lib failed	!!! \r\n");
		return 1;
	} else {
		printf("set wiringPi lib success  !!! \r\n");
	}

#elif USE_DEV_LIB
	printf("USE_DEV_LIB \r\n");
#endif
	return 0;
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_ModuleExit(void)
{
#ifdef USE_BCM2835_LIB
#if DEV_I2C
	bcm2835_i2c_end();
#endif
#if DEV_SPI
	bcm2835_spi_end();
#endif
	bcm2835_close();

#elif USE_WIRINGPI_LIB

#elif USE_DEV_LIB
#if DEV_I2C
	DEV_HARDWARE_I2C_end();
#endif
#if DEV_SPI
	DEV_HARDWARE_SPI_end();
#endif
#endif
}


/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
#ifdef USE_BCM2835_LIB
	bcm2835_delay(xms);
#elif USE_WIRINGPI_LIB
	delay(xms);
#elif USE_DEV_LIB
	UDOUBLE i;
	for(i=0; i < xms; i++) {
		usleep(1000);
	}
#endif
}

