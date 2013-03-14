/*
 * sensortask.c
 *
 *  Created on: Mar 12, 2013
 *      Author: armin
 */

#include "FreeRTOS.h"
#include "task.h"

void vSensorTask( void *pvParameters )
{
	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;

	while (1)
	{
		// Scan line
		// - select sensor
		// - query sensor
		// - preprocess result
		// - store result
		vTaskDelay(15);
	}

}

