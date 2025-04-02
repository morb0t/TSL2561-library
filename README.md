# TSL2561 STM32 Library

## Overview
This library provides an interface for the TSL2561 ambient light sensor using an STM32 microcontroller. It facilitates I2C communication to configure the sensor and retrieve light intensity values, including infrared (IR) and visible light levels.

## Features
- I2C communication with the TSL2561 sensor.
- Initialization with default or custom configuration.
- Reading visible and infrared light values.
- Computing luminosity in lux.
- UART output for debugging and data logging.


## Hardware Requirements
- STM32 Microcontroller
- TSL2561 Light Sensor
- I2C Bus Connection
- UART for debugging (optional)

## Software Requirements
- STM32 HAL or custom low-level drivers for I2C, UART, and Timers.
- Compatible with PlatformIO or STM32CubeIDE.

## Installation
1. Copy `tsl2561.h` and `tsl2561.c` into your project directory.
2. Include `#include "tsl2561.h"` in your main program.
3. Ensure I2C, UART, and Timer libraries are available in your project.

## API Reference
### Initialization
```c
int TSL2561_init();
```
Initializes the TSL2561 sensor with default settings.

```c
int TSL2561_configure(uint16_t mode);
```
Configures the TSL2561 sensor with a user-defined mode.

### Register Operations
```c
uint8_t readRegister(uint8_t slaveAddress, uint8_t registerAddress);
```
Reads a register from the sensor.

```c
int writeRegister(uint8_t slaveAdress, uint8_t registerAddress, uint8_t value);
```
Writes a value to a register of the sensor.

### Data Retrieval
```c
uint16_t readIR(int time);
```
Reads IR light intensity from the sensor.

```c
uint16_t readVisibleAndIR(int time);
```
Reads combined visible and IR light intensity from the sensor.

```c
unsigned int readVisibleLux(uint8_t gain, uint8_t integrationTime);
```
Computes the luminosity in lux based on sensor readings.

## Example Usage
```c
#include "tsl2561.h"

int main() {
    unsigned int lux;
    uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
    i2c_master_init(_I2C1);

    TSL2561_init();
    
    while (1) {
        lux = readVisibleLux(GAIN_x1, INTEGRATION_TIME_402ms);
        uart_printf(_USART2, "Luminosity is %d\r\n", lux);
        timer_wait_ms(_TIM2, 1000, NULL);
    }

    return 0;
}
```

## License
This project is open-source. Feel free to use, modify, and distribute it.


## Authors
- **Anouar ELHARDA**
- **Aya KADDAH**

## Contact
For any questions or contributions, feel free to reach out to the authors.

