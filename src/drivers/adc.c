/****************************************************************************
 *   $Id:: adc.c 6089 2011-01-06 04:38:09Z nxp12832                         $
 *   Project: NXP LPC17xx ADC example
 *
 *   Description:
 *     This file contains ADC code example which include ADC 
 *     initialization, ADC interrupt handler, and APIs for ADC
 *     reading.
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
#include "LPC17xx.h"
#include "type.h"
#include "adc.h"

#ifndef _BV
#define _BV(_x_) (1UL << (_x_))
#endif

volatile uint32_t ADCValue[ADC_NUM];
volatile uint32_t ADCIntDone = 0;
volatile uint32_t OverRunCounter = 0;
volatile uint8_t ADCCurrentChannel = 0;

volatile xQueueHandle xAdcQueue = NULL;
volatile xSemaphoreHandle xAdcSem = NULL;

#if ADC_INTERRUPT_FLAG
/******************************************************************************
** Function name:		ADC_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_IRQHandler (void) 
{
  uint32_t regVal;
  volatile uint32_t dummy=dummy;
  int i;
  uint32_t *adcdata = &LPC_ADC->ADDR0;
  ADCMeasurementItem_t mrec;
  static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  
  regVal = LPC_ADC->ADSTAT;		/* Read ADC will clear the interrupt */
  if ( regVal & 0x0000FF00 )	/* check OVERRUN error first */
  {
	OverRunCounter++;
  	for ( i = 0; i < ADC_NUM; i++ )
  	{
  	  regVal = (regVal & 0x0000FF00) >> 0x08;
  	  /* if overrun, just read ADDR to clear */
  	  /* regVal variable has been reused. */
      if ( regVal & _BV(i) )
  	  {
        dummy = adcdata[i];
  	  }
  	}
	LPC_ADC->ADCR &= ~((0x7<<24)|(0x1<<16));	/* stop ADC now, turn off BURST bit. */
  	ADCIntDone = 1;
  	return;	
  }

  for ( i = 0; i < ADC_NUM; i++ )
  {
    if ( regVal & _BV(i) )
	{
	  ADCValue[i] = ( adcdata[i] >> 4 ) & 0xFFF;
	  if(ADCCurrentChannel==i)
		  mrec.m = ( adcdata[i] >> 4 ) & 0xFFF;
	}
  }
  LPC_ADC->ADCR &= ~(0x7<<24);	/* stop ADC now */

  mrec.c = ADCCurrentChannel;
  mrec.t = xTaskGetTickCountFromISR();
  xQueueSendToBackFromISR(xAdcQueue, &mrec, NULL);

  ADCIntDone = 1;
  xSemaphoreGiveFromISR( xAdcSem, &xHigherPriorityTaskWoken );

  return;
}
#endif

/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			ADC clock rate
** Returned value:		None
** 
*****************************************************************************/
void ADCInit( uint32_t ADC_Clk )
{
  uint32_t i, pclkdiv, pclk;

  /* Allocate a queue to store measurements */
  xAdcQueue = xQueueCreate( ADCQ_BUFFER_LEN, sizeof(ADCMeasurementItem_t) );

  /* Allocate a mutex semaphore to sync multiple adcread invokations   */
  xAdcSem = xSemaphoreCreateMutex();

  /* Enable CLOCK into ADC controller */
  LPC_SC->PCONP |= (1 << 12);

  for ( i = 0; i < ADC_NUM; i++ )
  {
	ADCValue[i] = 0x0;
  }

  /* all the related pins are set to ADC inputs, AD0.0~7 */
  //LPC_PINCON->PINSEL0 &= ~0x000000F0;	/* P0.2~3, A0.6~7, function 10 */
  //LPC_PINCON->PINSEL0 |= 0x000000A0;
  LPC_PINCON->PINSEL1 &= ~0x003FC000;	/* P0.23~26, A0.0~3, function 01 */
  LPC_PINCON->PINSEL1 |= 0x00154000;
  //LPC_PINCON->PINSEL3 |= 0xF0000000;	/* P1.30~31, A0.4~5, function 11 */
  /* No pull-up no pull-down (function 10) on these ADC pins. */
  //LPC_PINCON->PINMODE0 &= ~0x000000F0;
  //LPC_PINCON->PINMODE0 |= 0x000000A0;
  LPC_PINCON->PINMODE1 &= ~0x003FC000;
  LPC_PINCON->PINMODE1 |= 0x002A8000;
  //LPC_PINCON->PINMODE3 &= ~0xF0000000;
  //LPC_PINCON->PINMODE3 |= 0xA0000000;

  /* By default, the PCLKSELx value is zero, thus, the PCLK for
  all the peripherals is 1/4 of the SystemFrequency. */
  /* Bit 24~25 is for ADC */
  pclkdiv = (LPC_SC->PCLKSEL0 >> 24) & 0x03;
  switch ( pclkdiv )
  {
	case 0x00:
	default:
	  pclk = SystemCoreClock/4;
	break;
	case 0x01:
	  pclk = SystemCoreClock;
	break; 
	case 0x02:
	  pclk = SystemCoreClock/2;
	break; 
	case 0x03:
	  pclk = SystemCoreClock/8;
	break;
  }

  LPC_ADC->ADCR = ( 0x01 << 0 ) |  /* SEL=1,select channel 0~7 on ADC0 */
		( ( pclk  / ADC_Clk - 1 ) << 8 ) |  /* CLKDIV = Fpclk / ADC_Clk - 1 */ 
		( 0 << 16 ) | 		/* BURST = 0, no BURST, software controlled */
		( 0 << 17 ) |  		/* CLKS = 0, 11 clocks/10 bits */
		( 1 << 21 ) |  		/* PDN = 1, normal operation */
		( 0 << 24 ) |  		/* START = 0 A/D conversion stops */
		( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */ 

  /* If POLLING, no need to do the following */
#if ADC_INTERRUPT_FLAG
  NVIC_EnableIRQ(ADC_IRQn);
  LPC_ADC->ADINTEN = 0x1FF;		/* Enable all interrupts */
#endif
  return;
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead( uint8_t channelNum )
{

	if( xAdcSem != NULL )
	{
		if( xSemaphoreTake( xAdcSem, ADC_READ_WAIT_LONG_TIME_FOR_MUTEX ) == pdTRUE )
	    {
			  //const volatile uint32_t *adcdata = &LPC_ADC->ADDR0;
			  ADCCurrentChannel = channelNum;

			  /* channel number is 0 through 7 */
			  if ( channelNum >= ADC_NUM )
			  {
				channelNum = 0;		/* reset channel number to 0 */
			  }
			  LPC_ADC->ADCR &= 0xFFFFFF00;
			  LPC_ADC->ADCR |= (1 << 24) | (1 << channelNum);
							/* switch channel,start A/D convert */
	    }
	}

	return ( channelNum );	/* if it's interrupt driven, the ADC reading is
							done inside the handler. so, return channel number */
}

ADCMeasurementItem_t *ADCReceiveQueue(ADCMeasurementItem_t *m)
{
	if(xAdcQueue != NULL)
	{
		if( xQueueReceive( xAdcQueue, ( void * ) m, ADC_REC_TICKS_TO_WAIT) )
		{
			return m;
		}
	}

	return NULL;
}


/*********************************************************************************
**                            End Of File
*********************************************************************************/
