/******************************************/
/*********@author: ELHARDA Anouar**********/
/*********@author: KADDAH Aya    **********/
/*********Project: TSL2561 sensor**********/
/******************************************/

#ifndef _TSL2561_H_
#define _TSL2561_H_

#include "lib/i2c.h"
#include "lib/timer.h"
#include "lib/uart.h"


/********Constante values/Adresses*********/
#define TSL2561_ADDRESS             (0x29)
#define ControlReg_ADDRESS          (0x80)
#define TimingReg_ADDRESS           (0x81)
#define TRESHLOWLOW_ADDRESS         (0x82)
#define TRESHLOWHIGH_ADDRESS        (0x83)
#define TRESHHIGHLOW_ADDRESS        (0x84)
#define TRESHHIGHHIGH_ADDRESS       (0x85)
#define InterruptC_ADDRESS          (0x86)
#define ID_REG_ADDRESS              (0x8A)
#define CHANNEL0_DATA_LOW           (0x8C)
#define CHANNEL0_DATA_HIGH          (0x8D)
#define CHANNEL1_DATA_LOW           (0x8E)
#define CHANNEL1_DATA_HIGH          (0x8F)

/***********INTEGRATION TIME***********/
#define INTEGRATION_TIME_13ms       (0x00)
#define INTEGRATION_TIME_101ms      (0x01)
#define INTEGRATION_TIME_402ms      (0x02)

/*****************GAIN***************/
#define GAIN_x1                     (0x00)
#define GAIN_x16                    (0x01)

/*****INTERRUPT CONTROL(INTER)*********/
#define INTERRUPT_DISABLE           (0x00)
#define LEVEL_INTERRUPT             (0x01)
#define SMB_ALERT                   (0x02)
#define TEST_MODE                   (0x03)

/******PERSIST INTERRUPT FUNCTION*******/
#define EVERY_ADC_CYCLE             (0x00)
#define OUT_OF_TRESH_RANGE          (0x01)
#define TWO_INTEG_OUT_RANGE         (0x02)
#define THREE_INTEG_OUT_RANGE       (0x03)
#define FOUR_INTEG_OUT_RANGE        (0x04)
#define FIVE_INTEG_OUT_RANGE        (0x05)
#define SIX_INTEG_OUT_RANGE         (0x06)
#define SEVEN_INTEG_OUT_RANGE       (0x07)
#define EIGHT_INTEG_OUT_RANGE       (0x08)
#define NINE_INTEG_OUT_RANGE        (0x09)
#define TEN_INTEG_OUT_RANGE         (0x0A)
#define ELEVEN_INTEG_OUT_RANGE      (0x0B)
#define TWELVE_INTEG_OUT_RANGE      (0x0C)
#define THIRTEEN_INTEG_OUT_RANGE    (0x0D)
#define FOURTEEN_INTEG_OUT_RANGE    (0x0E)
#define FIFTEEN_INTEG_OUT_RANGE     (0x0F) 
/******************************************/

/*****************CONSTANT*****************/
#define LUX_SCALE 14 // scale by 2^14
#define RATIO_SCALE 9 // scale ratio by 2^9

#define CH_SCALE 10 // scale channel values by 2^10
#define CHSCALE_TINT0 0x7517 // 322/11 * 2^CH_SCALE
#define CHSCALE_TINT1 0x0fe7 // 322/81 * 2^CH_SCALE

#define K1T 0x0040 // 0.125 * 2^RATIO_SCALE
#define B1T 0x01f2 // 0.0304 * 2^LUX_SCALE
#define M1T 0x01be // 0.0272 * 2^LUX_SCALE
#define K2T 0x0080 // 0.250 * 2^RATIO_SCALE
#define B2T 0x0214 // 0.0325 * 2^LUX_SCALE
#define M2T 0x02d1 // 0.0440 * 2^LUX_SCALE
#define K3T 0x00c0 // 0.375 * 2^RATIO_SCALE
#define B3T 0x023f // 0.0351 * 2^LUX_SCALE
#define M3T 0x037b // 0.0544 * 2^LUX_SCALE
#define K4T 0x0100 // 0.50 * 2^RATIO_SCALE
#define B4T 0x0270 // 0.0381 * 2^LUX_SCALE
#define M4T 0x03fe // 0.0624 * 2^LUX_SCALE
#define K5T 0x0138 // 0.61 * 2^RATIO_SCALE
#define B5T 0x016f // 0.0224 * 2^LUX_SCALE
#define M5T 0x01fc // 0.0310 * 2^LUX_SCALE
#define K6T 0x019a // 0.80 * 2^RATIO_SCALE
#define B6T 0x00d2 // 0.0128 * 2^LUX_SCALE
#define M6T 0x00fb // 0.0153 * 2^LUX_SCALE
#define K7T 0x029a // 1.3 * 2^RATIO_SCALE
#define B7T 0x0018 // 0.00146 * 2^LUX_SCALE
#define M7T 0x0012 // 0.00112 * 2^LUX_SCALE
#define K8T 0x029a // 1.3 * 2^RATIO_SCALE
#define B8T 0x0000 // 0.000 * 2^LUX_SCALE
#define M8T 0x0000 // 0.000 * 2^LUX_SCALE
/***********Functions prototype************/
int TSL2561_init();
int TSL2561_configure(uint16_t mode); // mode = INTR<<7 | PERSIST<<4 | INTEG<<1 | GAIN
uint8_t readRegister(uint8_t slaveAddress, uint8_t registerAddress);
int writeRegister(uint8_t slaveAdress, uint8_t registerAddress, uint8_t value);
uint16_t readIR();
uint16_t readVisibleAndIR(); 
unsigned int readVisibleLux(uint8_t gain, uint8_t integrationTime);
/******************************************/

#endif
