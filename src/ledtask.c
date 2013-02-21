/*
 * ledtask.c
 *
 *  Created on: Feb 2, 2013
 *      Author: armin
 */

#include "led.h"

void vLEDTask( void *pvParameters )
{
	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;
	//DBG("Starting LED task\n");

	while (1) {
		led_toggle((1<<LED0) | (1<<LED1) | (1<<LED2));
		vTaskDelay(300);
	}
}
