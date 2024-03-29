/****************************************************************************
 *   $Id:: adc.h 6089 2011-01-06 04:38:09Z nxp12832                         $
 *   Project: NXP LPC17xx ADC example
 *
 *   Description:
 *     This file contains ADC code header definition.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#ifndef __ADC_H 
#define __ADC_H

#include <string.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


/* In DMA mode, BURST mode and ADC_INTERRUPT flag need to be set. */
/* In BURST mode, ADC_INTERRUPT need to be set. */
#define ADC_INTERRUPT_FLAG	1	/* 1 is interrupt driven, 0 is polling */
#define ADC_DEBUG			1

#define ADC_OFFSET          0x10
#define ADC_INDEX           4

#define ADC_DONE            0x80000000
#define ADC_OVERRUN         0x40000000
#define ADC_ADINT           0x00010000

#define ADC_NUM			4		/* for LPCxxxx */
#define ADC_CLK			1000000		/* set to 1Mhz */

#define ADCQ_BUFFER_LEN		( ADC_NUM * 4 * sizeof(ADCMeasurementItem_t) )
#define ADC_REC_TICKS_TO_WAIT     ((portTickType) 10)
#define ADC_READ_WAIT_LONG_TIME_FOR_MUTEX 0xffff

typedef struct {
	uint16_t m;
	uint8_t c;
	portTickType t;
} ADCMeasurementItem_t;

extern void ADC_IRQHandler( void );
extern void ADCInit( uint32_t ADC_Clk );
extern uint32_t ADCRead( uint8_t channelNum );
extern ADCMeasurementItem_t *ADCReceiveQueue(ADCMeasurementItem_t *m);

extern volatile uint32_t ADCIntDone;
extern volatile uint32_t ADCValue[];
extern volatile xSemaphoreHandle xAdcSem;

#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
