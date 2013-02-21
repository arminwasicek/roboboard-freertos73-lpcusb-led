/****************************************************************************
 *   $Id:: mcpwm.c 5747 2010-11-30 23:45:33Z usb00423                       $
 *   Project: NXP LPC17xx Motor Control PWM example
 *
 *   Description:
 *     This file contains MCPWM code example which include MCPWM 
 *     initialization, MCPWM interrupt handler, and APIs for MCPWM access.
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
#include "mcpwm.h"

/******************************************************************************
** Function name:		MCPWM_IRQHandler
**
** Descriptions:		MCPWM interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCPWM_IRQHandler(void)
{
  uint32_t regVal;
#if (CHANNEL_0_CAPTURE || CHANNEL_1_CAPTURE || CHANNEL_2_CAPTURE)
  uint32_t regCap0, regCap1, regCap2;
#endif

  regVal = LPC_MCPWM->MCINTFLAG;
#if (CHANNEL_0_CAPTURE || CHANNEL_1_CAPTURE || CHANNEL_2_CAPTURE)
  regCap0 = LPC_MCPWM->CAP0;
  regCap1 = LPC_MCPWM->CAP1;
  regCap2 = LPC_MCPWM->CAP2;
  LPC_MCPWM->CAP_CLR = 0x00000007;
  regCap0 = LPC_MCPWM->CAP0;
  regCap1 = LPC_MCPWM->CAP1;
  regCap2 = LPC_MCPWM->CAP2;
#endif
  LPC_MCPWM->MCINTFLAG_CLR = regVal;
  return;
}

/******************************************************************************
** Function name:		mcpwm_init
**
** Descriptions:		MCPWM initialization.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void mcpwm_init(void)
{

//	PINSEL3 name  00              01       10         11       Reset value
//	 7:6    P1.19 GPIO Port 1.19  MCOA0    USB_PPWR   CAP1.1   00
//	 9:8    P1.20 GPIO Port 1.20  MCI0     PWM1.2     SCK0     00
//	11:10   P1.21 GPIO Port 1.21  MCABORT  PWM1.3     SSEL0    00
//	13:12   P1.22 GPIO Port 1.22  MCOB0    USB_PWRD   MAT1.0   00
//	15:14   P1.23 GPIO Port 1.23  MCI1     PWM1.4     MISO0    00
//	17:16   P1.24 GPIO Port 1.24  MCI2     PWM1.5     MOSI0    00
//	19:18   P1.25 GPIO Port 1.25  MCOA1    Reserved   MAT1.1   00
//	21:20   P1.26 GPIO Port 1.26  MCOB1    PWM1.6     CAP0.0   00
//	23:22   P1.27 GPIO Port 1.27  CLKOUT   USB_OVRCR  CAP0.1   00
//	25:24   P1.28 GPIO Port 1.28  MCOA2    PCAP1.0    MAT0.0   00
//	27:26   P1.29 GPIO Port 1.29  MCOB2    PCAP1.1    MAT0.1   00

// Peripheral Clock Selection register 1 (PCLKSEL1 - address 0x400F C1AC) bit
// 31:30 PCLK_MC Peripheral clock selection for the Motor Control PWM. 00

//	PCLKSEL0 and PCLKSEL1     Function                          Reset value
//	individual peripheral’s
//	clock select options
//	       00                 PCLK_peripheral = CCLK/4           00
//	       01                 PCLK_peripheral = CCLK
//	       10                 PCLK_peripheral = CCLK/2
//	       11                 PCLK_peripheral = CCLK/8

//	Configure the following registers in other modules before using the Motor Control PWM:
//	1. Power: in the PCONP register (Table 46), set bit PCMCPWM.
//	Remark: On reset the MCPWM is disabled (PCMCPWM = 0).
//	2. Peripheral clock: in the PCLKSEL1 register (Table 40) select PCLK_MCPWM.
//	3. Pins: select MCPWM functions through the PINSEL registers. Select modes for these
//	pins through the PINMODE registers (Section 8.5).
//	4. Interrupts: See Section 25.7.3 for motor control PWM related interrupts. Interrupts can
//	be enabled in the NVIC using the appropriate Interrupt Set Enable register.




  LPC_SC->PCONP |= 0x00020000;		   /* Turn On MCPWM PCLK */

  /* P1.19~1.26, P1.28~29 as Motor Control PWM pins. */
  LPC_PINCON->PINSEL3 &= ~(0xFFFF<<6);
  LPC_PINCON->PINSEL3 |= (0x5555<<6);

  LPC_PINCON->PINSEL3 &= ~(0x0F<<24);
  LPC_PINCON->PINSEL3 |= (0x05<<24);
  return;
}

/******************************************************************************
** Function name:		mcpwm_config
**
** Descriptions:		MCPWM configuration
**
** parameters:			ChannelNum, setup structure
** Returned value:		true or fase.
** 
******************************************************************************/
uint32_t mcpwm_config(uint32_t channelNum, MCPWM_CHANNEL_SETUP_T * channelSetup)
{
  if (channelNum == 0)
  {
    LPC_MCPWM->MCTIM0 = channelSetup->channelTimercounterValue;
    LPC_MCPWM->MCPER0 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MCPW0 = channelSetup->channelPulsewidthValue;

	if (channelSetup->channelType)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 1;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 1;
	}

	if (channelSetup->channelPolarity)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 2;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 2;
	}

	if (channelSetup->channelDeadtimeEnable)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 3;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 3;
	}
	LPC_MCPWM->MCDEADTIME &= ~0x3FF;
	LPC_MCPWM->MCDEADTIME |= channelSetup->channelDeadtimeValue & 0x3FF;

	if (channelSetup->channelUpdateEnable)
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 4;
	}
	else
	{
	  LPC_MCPWM->MCCON_SET = 1 << 4;
	}

    NVIC_EnableIRQ(MCPWM_IRQn);

    LPC_MCPWM->MCINTEN_SET = 7;
  }
  else if (channelNum == 1)
  {
    LPC_MCPWM->MCTIM1 = channelSetup->channelTimercounterValue;
    LPC_MCPWM->MCPER1 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MCPW1 = channelSetup->channelPulsewidthValue;

	if (channelSetup->channelType)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 9;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 9;
	}

	if (channelSetup->channelPolarity)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 10;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 10;
	}

	if (channelSetup->channelDeadtimeEnable)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 11;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 11;
	}
	LPC_MCPWM->MCDEADTIME &= ~(0x3FF << 10);
	LPC_MCPWM->MCDEADTIME |= (channelSetup->channelDeadtimeValue & 0x3FF) << 10;

	if (channelSetup->channelUpdateEnable)
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 12;
	}
	else
	{
	  LPC_MCPWM->MCCON_SET = 1 << 12;
	}

    NVIC_EnableIRQ(MCPWM_IRQn);

    LPC_MCPWM->MCINTEN_SET = 7 << 4;
  }
  else if (channelNum == 2)
  {
    LPC_MCPWM->MCTIM2 = channelSetup->channelTimercounterValue;
    LPC_MCPWM->MCPER2 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MCPW2 = channelSetup->channelPulsewidthValue;

	if (channelSetup->channelType)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 17;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 17;
	}

	if (channelSetup->channelPolarity)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 18;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 18;
	}

	if (channelSetup->channelDeadtimeEnable)
	{
	  LPC_MCPWM->MCCON_SET = 1 << 19;
	}
	else
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 19;
	}
	LPC_MCPWM->MCDEADTIME &= ~(0x3FF << 20);
	LPC_MCPWM->MCDEADTIME |= (channelSetup->channelDeadtimeValue & 0x3FF) << 20;

	if (channelSetup->channelUpdateEnable)
	{
	  LPC_MCPWM->MCCON_CLR = 1 << 20;
	}
	else
	{
	  LPC_MCPWM->MCCON_SET = 1 << 20;
	}

    NVIC_EnableIRQ(MCPWM_IRQn);

    LPC_MCPWM->MCINTEN_SET = 7 << 8;
  }
  else
  {
	return ( FALSE );		/* Unknown channel number */
  }
	
  return (TRUE);
}

/******************************************************************************
** Function name:		MCPWM_WriteToShadow
**
** Descriptions:		Write to MCPWM shadow registers.
**
** parameters:			ChannelNum, setup structure
** Returned value:		true or fase.
** 
******************************************************************************/
uint32_t mcpwm_writetoshadow(uint32_t channelNum, MCPWM_CHANNEL_SETUP_T * channelSetup)
{
  if (channelNum == 0)
  {
    LPC_MCPWM->MCPER0 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MCPW0 = channelSetup->channelPulsewidthValue;
  }
  else if (channelNum == 1)
  {
    LPC_MCPWM->MCPER1 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MCPW1 = channelSetup->channelPulsewidthValue;
  }
  else if (channelNum == 2)
  {
    LPC_MCPWM->MCPER2 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MCPW2 = channelSetup->channelPulsewidthValue;
  }
  else
  {
	return ( FALSE );		/* Unknown channel number */
  }
	
  return (TRUE);
}

/******************************************************************************
** Function name:		MCPWM_CaptureEvent
**
** Descriptions:		MCPWM channel capture event setup
**
** parameters:			channel number, capture event input, timer reset, hnc enable
** Returned value:		None
** 
******************************************************************************/
void mcpwm_capture_event(uint32_t channelNum, uint32_t captureEvent, uint32_t timerReset, uint32_t hncEnable)
{
  if (channelNum == 0)
  {
    LPC_MCPWM->MCCAPCON_CLR = 0x0024003F;
    LPC_MCPWM->MCCAPCON_SET = captureEvent & 0x3F;
	if (timerReset)
	{
      LPC_MCPWM->MCCAPCON_SET = 0x00040000;
	}
	if (hncEnable)
	{
      LPC_MCPWM->MCCAPCON_SET = 0x00200000;
	}
  }
  else if (channelNum == 1)
  {
    LPC_MCPWM->MCCAPCON_CLR = 0x00480FC0;
    LPC_MCPWM->MCCAPCON_SET = (captureEvent & 0x3F) << 6;
	if (timerReset)
	{
      LPC_MCPWM->MCCAPCON_SET = 0x00080000;
	}
	if (hncEnable)
	{
      LPC_MCPWM->MCCAPCON_SET = 0x00400000;
	}
  }
  else if (channelNum == 2)
  {
    LPC_MCPWM->MCCAPCON_CLR = 0x0093F000;
    LPC_MCPWM->MCCAPCON_SET = (captureEvent & 0x3F) << 12;
	if (timerReset)
	{
      LPC_MCPWM->MCCAPCON_SET = 0x00100000;
	}
	if (hncEnable)
	{
      LPC_MCPWM->MCCAPCON_SET = 0x00800000;
	}
  }
	
  return;
}

/******************************************************************************
** Function name:		countControl
**
** Descriptions:		MCPWM count control
**
** parameters:			channel number, count mode, capture event input
** Returned value:		None
** 
******************************************************************************/
void mcpwm_count_control(uint32_t channelNum, uint32_t countMode, uint32_t captureEvent)
{
  if (channelNum == 0)
  {
	if (countMode)
	{
      LPC_MCPWM->MCCNTCON_SET = 0x20000000 | (captureEvent & 0x3F);
	}
	else
	{
      LPC_MCPWM->MCCNTCON_CLR = 0x2000003F;
	}
  }
  else if (channelNum == 1)
  {
	if (countMode)
	{
      LPC_MCPWM->MCCNTCON_SET = 0x40000000 | ((captureEvent & 0x3F) << 6);
	}
	else
	{
      LPC_MCPWM->MCCNTCON_CLR = 0x4000FC00;
	}
  }
  else if (channelNum == 2)
  {
	if (countMode)
	{
      LPC_MCPWM->MCCNTCON_SET = 0x80000000 | ((captureEvent & 0x3F) << 12);
	}
	else
	{
      LPC_MCPWM->MCCNTCON_CLR = 0x803F0000;
	}
  }
	
  return;
}

/******************************************************************************
** Function name:		EMPWM_Start
**
** Descriptions:		Stop MCPWM channels
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void mcpwm_start(uint32_t channel0, uint32_t channel1, uint32_t channel2)
{
  uint32_t regVal = 0;
  
  if (channel0 == 1)
  {
    regVal |= 1;
  }
  if (channel1 == 1)
  {
    regVal |= (1 << 8);
  }
  if (channel2 == 1)
  {
    regVal |= (1 << 16);
  }

  LPC_MCPWM->MCCON_SET = regVal;
  return;
}

/******************************************************************************
** Function name:		MCPWM_Stop
**
** Descriptions:		Stop MCPWM channels
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void mcpwm_stop(uint32_t channel0, uint32_t channel1, uint32_t channel2)
{
  uint32_t regVal = 0;

  if (channel0 == 1)
  {
    regVal |= 1;
  }
  if (channel1 == 1)
  {
    regVal |= (1 << 8);
  }
  if (channel2 == 1)
  {
    regVal |= (1 << 16);
  }

  LPC_MCPWM->MCCON_CLR = regVal;
  return;
}

/******************************************************************************
** Function name:		MCPWM_acMode
**
** Descriptions:		MCPWM ac Mode enable/disable
**
** parameters:			acMode
** Returned value:		None
** 
******************************************************************************/
void mcpwm_acmode(uint32_t acMode)
{
  if (acMode)
  {
    LPC_MCPWM->MCCON_SET = 0x40000000;
  }
  else
  {
    LPC_MCPWM->MCCON_CLR = 0x40000000;
  }

  return;
}

/******************************************************************************
** Function name:		MCPWM_dcMode
**
** Descriptions:		MCPWM ac Mode enable/disable, output invert/non-invert
**
** parameters:			dcMode, outputInverted, outputPattern
** Returned value:		None
** 
******************************************************************************/
void mcpwm_dcmode(uint32_t dcMode, uint32_t outputInvered, uint32_t outputPattern)
{
  if (dcMode)
  {
    LPC_MCPWM->MCCON_SET = 0x80000000;
  }
  else
  {
    LPC_MCPWM->MCCON_CLR = 0x80000000;
  }

  if (outputInvered)
  {
    LPC_MCPWM->MCCON_SET = 0x20000000;
  }
  else
  {
    LPC_MCPWM->MCCON_CLR = 0x20000000;
  }

  LPC_MCPWM->MCCCP = outputPattern;

  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
