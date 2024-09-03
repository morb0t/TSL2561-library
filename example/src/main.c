#include "include/board.h"
#include "lib/io.h"
#include "lib/util.h"
#include "libshield/tsl2561.h"

#define delay(t) (timer_wait_ms(_TIM2, t, NULL))

volatile char cmd;
static void on_rx_cb(char c)
{
	cmd = c;
}


int main(){
    unsigned int lux;

    uart_init(_USART2, 115200, UART_8N1, on_rx_cb); // initialize UART
	i2c_master_init(_I2C1); // initialize I2C

    TSL2561_init();	// initialise TSL2561 with default configuration (no gain, no interrupt, integrationtime=402ms)
    /* use this block of code to specifiy the desired configuration
    mode = INTERRUPT_DISABLE<<7 | EVERY_ADC_CYCLE<<4 | INTEGRATION_TIME_402ms<<1 | GAIN_x1
    TSL2561_configure(mode);
    */
    while(1){

        lux=readVisibleLux(GAIN_x1, INTEGRATION_TIME_402ms);
        uart_printf(_USART2, "Luminosity is %d\r\n", lux);
        delay(1000);
    }

    return 0;
}