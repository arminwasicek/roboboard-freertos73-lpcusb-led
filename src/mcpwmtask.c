/*
 * mcpwmtask.c
 *
 *  Created on: Feb 9, 2013
 *      Author: armin
 */

#include "mcpwm.h"

uint32_t f(uint32_t t)  {
	if((t%600)>=300)
		return (300 - (t%300)-1);
	else
		return (t%300);
}


void vMCPWMTask( void *pvParameters )
{
	MCPWM_CHANNEL_SETUP_T channelsetup[3];
	uint32_t t;

	/* Just to prevent compiler warnings about the unused parameter. */
	( void ) pvParameters;


	channelsetup[0].channelType = 0;
	channelsetup[0].channelPolarity = 0;
	channelsetup[0].channelDeadtimeEnable = 0;
	channelsetup[0].channelDeadtimeValue = 0;
	channelsetup[0].channelUpdateEnable = 1;
	channelsetup[0].channelTimercounterValue = 0;
	channelsetup[0].channelPeriodValue = 300;
	channelsetup[0].channelPulsewidthValue = 300;

	mcpwm_config(0, &channelsetup[0]);

	mcpwm_start(1,0,0);

	while (1)
	{
		channelsetup[0].channelPulsewidthValue = f(t++);
		mcpwm_writetoshadow(0, &channelsetup[0]);

		vTaskDelay(15);
	}
}
