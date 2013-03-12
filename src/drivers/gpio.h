/*
 * gpio.h
 *
 *  Created on: Mar 12, 2013
 *      Author: armin
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "FreeRTOS.h"
#include "task.h"

#include "LPC17xx.h"


#define PIN0 0
#define PIN1 1
#define PIN2 2


inline void gpio_init(void);
inline void gpio_set(int pin);
inline void gpio_clear(int pin);
inline void gpio_xselect(int pin);


#endif /* GPIO_H_ */
