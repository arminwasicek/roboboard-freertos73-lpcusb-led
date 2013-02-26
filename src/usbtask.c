/*
 * usbtask.c
 *
 *  Created on: Feb 12, 2013
 *      Author: armin
 */

#include "usb_vcom.h"
#include "roboshell.h"

#define INCREMENT_ECHO_BY 2
#define INBUFLEN  64

static int inbuf[INBUFLEN];

void usbsubtask(void)
{
	Scanner s;
	Token t;

	int c, n, i=0;

	s.curpos=inbuf;

	// echo any character received (do USB stuff in interrupt)
	for( ;; )
	{
		c = VCOM_getchar();

		if (c != EOF)
		{
			// add char to input buffer
			if(i>=INBUFLEN)
			{
				i--;
				continue ;
			}

			// echo character back
			VCOM_putchar(c);

			// check backspace
			if((c==0x8) && (i!=0))
			{
				VCOM_putchar(' ');
				VCOM_putchar('\b');
				i--;
				inbuf[i]='\0';
			}

			// save to input buffer
			inbuf[i]=c;
			i++;

			//
			if(c==0xd)               //return key
			{
				VCOM_putchar('\r');  //carriage return
				VCOM_putchar(0x0a);  //line feed
				memset(inbuf, 0, INBUFLEN);
				i=0;
				if( (n=scan(&s,&t))!=EOL )
				{

				}

			}
		}
	}
}

