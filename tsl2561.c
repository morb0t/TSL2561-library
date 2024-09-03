#include "tsl2561.h"

/**********Define Functions************/
uint8_t readRegister(uint8_t slaveAddress, uint8_t registerAddress){

    //@param: salveAdress : sensor address
    //@param: registerAddress : the register which the data will be read
    //@return: buf : retrieved data from sensor

    uint8_t buf;
    buf=registerAddress;
    int status;
    status = i2c_write_read(_I2C1, slaveAddress, &buf, 1, 1);
    //uart_printf(_USART2, "status: %d, %x", status, status);
    if(status != I2C_OK){
        uart_printf(_USART2, "\r\nreading not");
        return -1;
    }
    return buf;

}

int writeRegister(uint8_t slaveAdress, uint8_t registerAddress, uint8_t value){

    //@param: slaveAdress : sensor address
    //@param: registerAddress : the register which the data will be written
    //@return: 0 or -1 based on the I2C write protocole if it was done successfully

    uint8_t buf[2];
    buf[0] = registerAddress;
    buf[1] = value;
    int status;

    status = i2c_write(_I2C1, slaveAdress, buf, 2);

    if(status != I2C_OK){
        return -1;
    }
    return 0;
}

int TSL2561_init(){
    
    //> This function initialize the sensor with default configuration

    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x03); // POWER UP
    writeRegister(TSL2561_ADDRESS, TimingReg_ADDRESS, (GAIN_x1<<4)|INTEGRATION_TIME_402ms); // no gain, integration time 400ms
    writeRegister(TSL2561_ADDRESS, InterruptC_ADDRESS, INTERRUPT_DISABLE); // no interrupt
    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x00); // power down
}

int TSL2561_configure(uint16_t mode){
    
    //@param: mode : variable that stores different configuration value of different register
    //> mode = INTR<<7 | PERSIST<<4 | INTEG<<1 | GAIN
    //> this function initialize the sensor with user's configuration

    int gain = mode & 0x1;
    int intgration_time = (mode>>1) & 0x3;
    int persiste = (mode >> 3) & 0xF;
    int interrupt = (mode>> 7) & 0x3;

    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x03); // POWER UP
    writeRegister(TSL2561_ADDRESS, TimingReg_ADDRESS, (gain << 4) | intgration_time);
    writeRegister(TSL2561_ADDRESS, InterruptC_ADDRESS, (interrupt<<4) | persiste);
    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x00); // POWER DOWN
}

uint16_t readIR(int time){

    //@return: channel1_raw : this variable stores a 16 bit value of channel1, containing the value of InfraRed

    uint8_t channel0_low, channel0_high, channel1_low, channel1_high;
    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x03);//POWER UP
    timer_wait_ms(_TIM2, time, NULL);
    channel0_low = readRegister(TSL2561_ADDRESS, CHANNEL0_DATA_LOW);
    channel0_high = readRegister(TSL2561_ADDRESS, CHANNEL0_DATA_HIGH);
    channel1_low = readRegister(TSL2561_ADDRESS, CHANNEL1_DATA_LOW);
    channel1_high = readRegister(TSL2561_ADDRESS, CHANNEL1_DATA_HIGH);
    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x00);//POWER DOWN

    uint16_t channel0_raw = channel0_high << 8 | channel0_low;
    uint16_t channel1_raw = channel1_high << 8 | channel1_low;

    if(channel1_raw == 0){
        return 0;
    }
    if((channel0_raw/channel1_raw)<2 && channel0_raw>4900){
        uart_printf(_USART2, "\n\rOut of range");
        return -1;
    }
    return channel1_raw;
}
uint16_t readVisibleAndIR(int time){

    //@return: channel0_raw : this variable stores a 16 bit value of channel0, containing the value of visible and InfraRed Light

    uint8_t channel1_low, channel1_high, channel0_low, channel0_high;
    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x03);//POWER UP
    timer_wait_ms(_TIM2, time, NULL);
    channel0_low = readRegister(TSL2561_ADDRESS, CHANNEL0_DATA_LOW);
    channel0_high = readRegister(TSL2561_ADDRESS, CHANNEL0_DATA_HIGH);
    channel1_low = readRegister(TSL2561_ADDRESS, CHANNEL1_DATA_LOW);
    channel1_high = readRegister(TSL2561_ADDRESS, CHANNEL1_DATA_HIGH);
    writeRegister(TSL2561_ADDRESS, ControlReg_ADDRESS, 0x00);//POWER DOWN

    uint16_t channel0_raw = channel0_high << 8 | channel0_low;
    uint16_t channel1_raw = channel1_high << 8 | channel1_low;

    if(channel1_raw == 0){
        return 0;
    }
    if((channel0_raw/channel1_raw)<2 && channel0_raw>4900){
        uart_printf(_USART2, "\n\rOut of range");
        return -1;
    }

    return channel0_raw;
}
unsigned int readVisibleLux(uint8_t gain, uint8_t integrationTime){
    unsigned long chScale;
    unsigned long channel1;
    unsigned long channel0;
    unsigned long ratio1 = 0;
    unsigned long ratio;
    unsigned int b, m;
    unsigned long temp;
    unsigned long lux;
    unsigned int time;

    // Scale the channel values depending on the gain and integration time
    switch (integrationTime) {
        case 0: // 13.7 msec
            chScale = CHSCALE_TINT0;
            time = 14;
            break;
        case 1: // 101 msec
            chScale = CHSCALE_TINT1;
            time = 102;
            break;
        default: // assume no scaling
            chScale = (1 << CH_SCALE);
            time = 403;
            break;
    }

    // Scale if gain is NOT 16X
    if (!gain) chScale = chScale << 4; // scale 1X to 16X

    // Scale the channel values
    channel0 = readVisibleAndIR(time);
    channel1 = readIR(time);
    channel0 = (channel0 * chScale) >> CH_SCALE;
    channel1 = (channel1 * chScale) >> CH_SCALE;

    // Find the ratio of the channel values (Channel1/Channel0)
    if (channel0 != 0) ratio1 = (channel1 << (RATIO_SCALE + 1)) / channel0;

    // Round the ratio value
    ratio = (ratio1 + 1) >> 1;

    // Determine b and m coefficients based on the ratio
    if ((ratio >= 0) && (ratio <= K1T)) {
        b = B1T;
        m = M1T;
    } else if (ratio <= K2T) {
        b = B2T;
        m = M2T;
    } else if (ratio <= K3T) {
        b = B3T;
        m = M3T;
    } else if (ratio <= K4T) {
        b = B4T;
        m = M4T;
    } else if (ratio <= K5T) {
        b = B5T;
        m = M5T;
    } else if (ratio <= K6T) {
        b = B6T;
        m = M6T;
    } else if (ratio <= K7T) {
        b = B7T;
        m = M7T;
    } else {
        b = B8T;
        m = M8T;
    }

    // Calculate the lux value
    temp = ((channel0 * b) - (channel1 * m));
    if (temp < 0) temp = 0;
    temp += (1 << (LUX_SCALE - 1));
    lux = temp >> LUX_SCALE;

    return lux;
}