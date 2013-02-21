/*
 * led.h
 *
 *  Created on: Jan 31, 2013
 *      Author: armin
 */

#ifndef LED_H_
#define LED_H_

#include "FreeRTOS.h"
#include "task.h"

#include "LPC17xx.h"

extern volatile int _led_state;

#define LED0 0
#define LED1 1
#define LED2 2

#define LEDS_NUM 3

inline void led_init(void);
inline void led_on(int led);
inline void led_off(int led);
inline void led_toggle(int ledmask);

void led_test(void);

#endif /* LED_H_ */
