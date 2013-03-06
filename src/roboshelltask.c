/*
 * roboshelltask.c
 *
 *  Created on: Feb 12, 2013
 *      Author: armin
 */

#include "usb_vcom.h"
#include "roboshell.h"

#define INCREMENT_ECHO_BY 2
#define INBUFLEN  64

static char inbuf[INBUFLEN];

static const char * const pcWelcomeMessage = ( char * ) "Welcome to FreeRTOS.\r\n";


void roboshelltask(void)
{
	int c, i=0;

	VCOM_puts(pcWelcomeMessage);

	// echo any character received (do USB stuff in interrupt)
	for( ;; )
	{
		c = VCOM_getchar();

		if (c != EOF)
		{
			// check end of input buffer
			if(i>=INBUFLEN)
			{
				i--;
				continue ;
			}

			// echo character back
			VCOM_putchar(c);

			// decide what to do
			switch(c)
			{
			case '\b' :  // backspace
				if((c==0x8) && (i!=0))
				{
					VCOM_putchar(' ');
					VCOM_putchar('\b');
					i--;
					inbuf[i]='\0';
				}
				break ;

			case 0xd  :  // return key
				VCOM_putchar('\r');  //carriage return
				VCOM_putchar(0x0a);  //line feed
				inbuf[i]='\r';
				inbuf[i+1]=0xa;
				VCOM_puts((char*)inbuf);
				i=0;
				//cli_process_command((char*)inbuf, i);
				memset(inbuf, 0, INBUFLEN);
				break;

			default:
				// save to input buffer
				inbuf[i]=c;
				i++;
			}//switch
		}//if
	}//for
}


