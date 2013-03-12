/*
 * gpio.c
 *
 *  Created on: Mar 12, 2013
 *      Author: armin
 */

#include "gpio.h"


/*
 * Pin Function Select register 0 (PINSEL0 - 0x4002 C000)
 * 15:14 P0.7 GPIO Port 0.7 I2STX_CLK SCK1 MAT2.1  00
 * 17:16 P0.8 GPIO Port 0.8 I2STX_WS  MISO1 MAT2.2 00
 * 19:18 P0.9 GPIO Port 0.9 I2STX_SDA MOSI1 MAT2.3 00
*/


inline void gpio_init(void)
{
	LPC_SC->PCONP |= (1 << 15);				/* Enable GPIO */
	LPC_GPIO0->FIODIR |= 0x00000380;		/* P0.7 + P0.8 + P0.9 defined as Outputs */
	LPC_GPIO0->FIOSET &= ~(0x00000380);		/* turn off all the LEDs */
}


inline void gpio_set(int pin)
{
	if((pin<PIN0) || (pin>PIN2))
		return ;

	LPC_GPIO0->FIOSET |= ((1<<pin) << 7);
}


inline void gpio_clear(int pin)
{
	if((pin<PIN0) || (pin>PIN2))
		return ;

	LPC_GPIO0->FIOCLR |= ((1<<pin) << 7);
}

inline void gpio_xselect(int pin)
{
	if((pin<PIN0) || (pin>PIN2))
		return ;

	LPC_GPIO0->FIOCLR |= ((~(1<<pin) << 6) & 0x00000380);
	LPC_GPIO0->FIOSET |= ((1<<pin) << 6);

}
