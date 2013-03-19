/*
 * sensortask.c
 *
 *  Created on: Mar 12, 2013
 *      Author: armin
 */

#include "FreeRTOS.h"
#include "task.h"
#include "adc.h"
#include "gpio.h"
#include "semphr.h"

#define LINE_LEN 8
#define LINE_CHAN 0
#define BW_THRESHOLD 2100

int line[LINE_LEN];

void vSensorTask( void *pvParameters )
{
	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;
	int i;
	ADCMeasurementItem_t m;

	memset(&line, 0, sizeof(line));

	while (1)
	{
		// Read sensor on ADC channel
		ADCRead(LINE_CHAN);

		//Wait until ADC is ready
		if( ADCReceiveQueue(&m)!=NULL )
		{
			printf("adc0=%d\r\n", m.m);
			if(m.m>BW_THRESHOLD)
				line[i]=1;
			else
				line[i]=0;
		}


/*
		// Scan line
		for(i=0;i<LINE_LEN;i++)
		{
		// - select sensor
			gpio_select(i);
		// - query sensor
			//ADCRead( LINE_CHAN );
			//TODO wait -- or busy wait by polling adcwait
		}
		// - preprocess result
		for(i=0;i<LINE_LEN;i++)
		{		// - store result
			//ADCReceiveQueue(&m);
			if(m.m[LINE_CHAN]>BW_THRESHOLD)
				line[i]=1;
			else
				line[i]=0;
		}
*/
		vTaskDelay(500);
	}

}

