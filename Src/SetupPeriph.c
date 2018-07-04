/*
 * SetupPeriph.c
 *
 *  Created on: Jun 30, 2018
 *      Author: muxamor
 */

#include "stm32l4xx.h"
#include "SetupPeriph.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_iwdg.h"
#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"

#include  <stdio.h>

//#include "stm32l4xx_ll_dma.h"



void LL_Init(void){

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/
	/* MemoryManagement_IRQn interrupt configuration */
	NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* BusFault_IRQn interrupt configuration */
	NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* UsageFault_IRQn interrupt configuration */
	NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* SVCall_IRQn interrupt configuration */
	NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* DebugMonitor_IRQn interrupt configuration */
	NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* PendSV_IRQn interrupt configuration */
	NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* SysTick_IRQn interrupt configuration */
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void){

	LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

	if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4){
		Error_Handler();
	}

	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	/*
  	//Enable HSE
  	  LL_RCC_HSE_EnableBypass();
  	  LL_RCC_HSE_Enable();
   	   // Wait till HSE is ready
  	  while(LL_RCC_HSE_IsReady() != 1);*/

	//Enable HSI
	LL_RCC_HSI_Enable();
	//Wait till HSI is ready
	while(LL_RCC_HSI_IsReady() != 1);
	//LL_RCC_HSI_SetCalibTrimming(16);//not need to do this

	// Enable LSI
	LL_RCC_LSI_Enable();

	/* Wait till LSI is ready */
	while(LL_RCC_LSI_IsReady() != 1);

	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2);
	LL_RCC_PLL_Enable();
	LL_RCC_PLL_EnableDomain_SYS();

	/* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1);

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_Init1msTick(80000000);
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	LL_SetSystemCoreClock(80000000);

	LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_SYSCLK);
	LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK);

	/* SysTick_IRQn interrupt configuration */
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
}


/**
  * @brief  This function setup settings for Led on the KTIVT_SS_board.
  * @retval None
*/
void SetupLED(void){

	LL_GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

	 /*Configure LED_Yellow_HL1 (PC10), LED_Green_HL2 (PC11), LED_Green_HL3 (PC12) as output Push-Pull      */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; // try set - LL_GPIO_OUTPUT_OPENDRAIN on real board
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* Diode is off */
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12);

	/*For ON/OFF LED need to use function:                            */
	  /*LED_Yellow_HL1_ON()       LED_Yellow_HL1_OFF()                */
	  /*LED_Green_HL2_ON()        LED_Green_HL2_OFF()                 */
	  /*LED_Green_HL3_ON()        LED_Green_HL3_OFF()                 */
	  /*Define in SetupPeriph.h                                       */
}




/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  *
  *
  *
  */
void _Error_Handler(char *file, int line)
{

	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 //while(1)
  //{
 // }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

