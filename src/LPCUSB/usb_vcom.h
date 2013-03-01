/*
 * usb_vcom.h
 *
 *  Created on: Feb 12, 2013
 *      Author: armin
 */

#ifndef USB_VCOM_H_
#define USB_VCOM_H_

#include <stdio.h>

int VCOM_putchar(int c);
int VCOM_getchar(void);
int VCOM_puts(const char *s);
char *VCOM_fgets(char *s, int size);


#endif /* USB_VCOM_H_ */
