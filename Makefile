DIR_BIN = ./_build
DIR_Config = ./lib/Config
DIR_FONTS = ./lib/Fonts
DIR_GUI = ./lib/GUI
DIR_LCD = ./lib/Device
DIR_PWM = ./lib/PWM

OBJ_C = $(wildcard ${DIR_Config}/*.c ${DIR_FONTS}/*.c ${DIR_GUI}/*.c ${DIR_LCD}/*.c ${DIR_PWM}/*.c ./*.c)
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

DEBUG = -D DEBUG

USE_RPI = USE_BCM2835_LIB
# USE_RPI = USE_WIRINGPI_LIB
# USE_RPI = USE_DEV_LIB

ifeq ($(USE_RPI), USE_BCM2835_LIB)
    LIB_RPI = -lbcm2835 -lm -lpthread
else ifeq ($(USE_RPI), USE_WIRINGPI_LIB)
    LIB_RPI = -lwiringPi -lm -lpthread 
else ifeq ($(USE_RPI), USE_DEV_LIB)
    LIB_RPI = -lm -lpthread
endif
DEBUG_RPI = -D $(USE_RPI)

TARGET = $(DIR_BIN)/fan
CC = gcc
MSG = -g -O0 -Wall
CFLAGS += $(MSG)

${TARGET}: ${DIR_BIN} ${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB_RPI) $(DEBUG_RPI) -I ${DIR_BIN}


${DIR_BIN}/%.o: $(DIR_FONTS)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ 
${DIR_BIN}/%.o: $(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ 
${DIR_BIN}/%.o: $(DIR_LCD)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ 
${DIR_BIN}/%.o: $(DIR_PWM)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ 
${DIR_BIN}/%.o: ./%.c
	$(CC) $(CFLAGS) -c  $< -o $@ 
${DIR_BIN}/%.o: $(DIR_Config)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB_RPI) $(DEBUG_RPI)

clean :
	rm -rf $(DIR_BIN) 

${DIR_BIN}:
	mkdir $@