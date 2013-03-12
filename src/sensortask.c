/*
 * sensortask.c
 *
 *  Created on: Mar 12, 2013
 *      Author: armin
 */


void vSensorTask( void *pvParameters )
{
	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;

	while (1)
	{
		vTaskDelay(15);
	}

}

