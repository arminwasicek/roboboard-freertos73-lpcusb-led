/*
 * led.c
 *
 *  Created on: Jan 31, 2013
 *      Author: armin
 */

#include "led.h"


inline void led_init(void)
{
	LPC_GPIO2->FIODIR |= 0x00003800;		/* P2.12 + P2.12 + P2.14 defined as Outputs */
	LPC_GPIO2->FIOSET &= ~(0x00003800);		/* turn off all the LEDs */
}

inline void led_on(int led)
{
	if((led<LED0) || (led>LED2))
		return ;

	LPC_GPIO2->FIOCLR |= ((1<<led) << 11);
}


inline void led_off(int led)
{

	if((led<LED0) || (led>LED2))
		return ;

	LPC_GPIO2->FIOSET |= ((1<<led) << 11);
}


inline void led_toggle(int ledmask)
{
	int state=0;
	// sanitize input
	ledmask = ledmask & 0x7;
	//read current state from gpio
	state = LPC_GPIO2->FIOPIN & (ledmask<<11);
	// clear all high state pins
	LPC_GPIO2->FIOCLR |= state;
	// reverse state; transition from low to high
	state = ~state & (ledmask<<11);
	// set all prev. low pins
	LPC_GPIO2->FIOSET |= state;
}



void led_test(void)
{
	led_off(LED0);
	led_off(LED1);
	led_off(LED2);

	led_on(LED0);
	led_on(LED1);
	led_on(LED2);

	led_toggle( (1<<LED0) | (1<<LED2)  );
	led_toggle( (1<<LED1) | (1<<LED2)  );
	led_toggle( (1<<LED0) | (1<<LED1) | (1<<LED2)  );
	led_toggle( (1<<LED0) | (1<<LED1) | (1<<LED2)  );

//	  LPC_GPIO2->FIODIR = 0x00003FFF;		/* P2.xx defined as Outputs (J6-42 - J6-53) */
//	  LPC_GPIO2->FIOCLR = 0x00003FFF;		/* turn off all the LEDs */
//
//	  while(1)
//	  {
//		for(i = 0; i < 14; i++)
//		{
//		  LPC_GPIO2->FIOSET = 1 << i;
//		  for(j = 1000000; j > 0; j--);
//		}
//		LPC_GPIO2->FIOCLR = 0x00003FFF;
//		for(j = 1000000; j > 0; j--);
//	  }
}
/* EOF */
