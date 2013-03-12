/*
 * controltask.c
 *
 *  Created on: Mar 12, 2013
 *      Author: armin
 */


void vControlTask( void *pvParameters )
{
	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;

	while (1)
	{
		// Read input from sensors
		// Determine error
		// Decide control action
		// Send output to actuators

		vTaskDelay(15);
	}

}
