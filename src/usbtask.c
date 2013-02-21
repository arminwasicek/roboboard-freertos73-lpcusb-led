/*
 * usbtask.c
 *
 *  Created on: Feb 12, 2013
 *      Author: armin
 */

#include "usb_vcom.h"

#define INCREMENT_ECHO_BY 2


void usbsubtask(void)
{
	int c;
	// echo any character received (do USB stuff in interrupt)
	for( ;; )
	{
		c = VCOM_getchar();
		if (c != EOF)
		{
			// Echo character back with INCREMENT_ECHO_BY offset, so for example if
			// INCREMENT_ECHO_BY is 1 and 'A' is received, 'B' will be echoed back.
			VCOM_putchar(c + INCREMENT_ECHO_BY );
		}
	}
}

