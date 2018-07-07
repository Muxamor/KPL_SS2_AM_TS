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
	//NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
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
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
}


/**
  * @brief  This function setup  Led on the KTIVT_SS_board.
  * @retval None
*/
void SetupLED(void){

	LL_GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

	 /*Configure LED_Yellow_HL1 (PC10), LED_Green_HL2 (PC11), LED_Green_HL3 (PC12) as output Push-Pull      */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
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
  * @brief  This function setup GPIO to control IC on the KTIVT_SS_board.
  * @retval None
*/
void SetupGPIO(void){


	LL_GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);


	/* Configure pins out for control preamplifier K1. PA3=10 PA4=100 PA5=1000 for AG1411 ON=0 OFF=1    */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*off K1 */
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5);


	/* Configure pins in for control COMPorators COMP1=PC0, COMP2=PC1, COMP3=PA0*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	/* Configure pins ???? PC8=F_SA0 PC9=F_SA1 PA8=F_SA2 for AG1411 ON=0 OFF=1 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*off F_SA*/
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8|LL_GPIO_PIN_9);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);


	/* Configure pins for control main amplifier K2. out: PB0=G0 PB1=G1 PB2=G2 PB10=G3 PB11=G4  in: COMP4=PA2   */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_10|LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*off K2=1 */
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_10|LL_GPIO_PIN_11);
	/* Config COMP4*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	//GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	//GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void USART1_Init(void){
  	LL_USART_InitTypeDef USART_InitStruct;

  	LL_GPIO_InitTypeDef GPIO_InitStruct;

  	/* Peripheral clock enable */
  	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  
  	/**USART1 GPIO Configuration  
  	PA9   ------> USART1_TX
  	PA10   ------> USART1_RX 
  	*/
  	GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  	/* USART1 configured as follow:
            - BaudRate = 5000000
            - Word Length = 9 Bits
            - One Stop Bit
            - No parity
            - Hardware flow control disabled (RTS and CTS signals)
            - Receive and transmit enabled
      */
  	USART_InitStruct.BaudRate = 5000000;
  	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_9B;
  	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  	LL_USART_Init(USART1, &USART_InitStruct);

  	LL_USART_SetTXRXSwap(USART1, LL_USART_TXRX_SWAPPED);

  	LL_USART_ConfigAsyncMode(USART1); 
  	LL_USART_Enable(USART1);
   
}

/**
  * @brief  Setup I2C.
  * @param  None
  * @retval None
*/
void SPI2_Init(void){

	LL_SPI_Disable(SPI2); ///Возможно убрать после теста

  	LL_SPI_InitTypeDef SPI_InitStruct;

  	LL_GPIO_InitTypeDef GPIO_InitStruct;
  	/* Peripheral clock enable */
  	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  
  	/**SPI2 GPIO Configuration  
  	PC2   ------> SPI2_MISO
  	PC3   ------> SPI2_MOSI
  	PB12   ------> SPI2_NSS
  	PB13   ------> SPI2_SCK 
  	*/
  	GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  	GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  	/*Setup SPI2 */
  	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  	SPI_InitStruct.NSS = LL_SPI_NSS_HARD_OUTPUT;
  	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128; /// Скорость обмена нужно уточнить у Саши
  	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  	SPI_InitStruct.CRCPoly = 0;//7;
  	LL_SPI_Init(SPI2, &SPI_InitStruct);

  	LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);

  	LL_SPI_EnableNSSPulseMgt(SPI2);

  	LL_SPI_Enable(SPI2);// Добавлял сам проверить почему не сгенерилось автоматом

}

/**
  * @brief  Setup I2C.
  * @param  None
  * @retval None
*/
void Setup_I2C(void){

	LL_I2C_InitTypeDef I2C_InitStruct;

  	LL_GPIO_InitTypeDef GPIO_InitStruct;

  	/* Peripheral clock enable */
  	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  
  	/**I2C1 GPIO Configuration  
  	PB6   ------> I2C1_SCL
  	PB7   ------> I2C1_SDA 
  	*/
  	GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /**I2C Initialization 
    */
  	LL_I2C_EnableAutoEndMode(I2C1);
	LL_I2C_DisableOwnAddress2(I2C1);
  	LL_I2C_DisableGeneralCall(I2C1);
  	LL_I2C_EnableClockStretching(I2C1);

  	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  	I2C_InitStruct.Timing = 0x00702991;
  	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  	I2C_InitStruct.DigitalFilter = 0;
  	I2C_InitStruct.OwnAddress1 = 0;
  	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  	LL_I2C_Init(I2C1, &I2C_InitStruct);

  	LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
}


/**
  * @brief  This function setup interrupts for all ports and inside event .
  * @param  None
  * @retval None
*/
void SetupInterrupt(void){
	
  	/* Setup USART1 interrupt Init */
  	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0)); /*Set priority №1 from 0..15*/
  	LL_USART_EnableIT_RXNE(USART1); //Enable RX no empty Interrupt 
  	//LL_USART_DisableIT_RXNE(USART1);
  	NVIC_EnableIRQ(USART1_IRQn);



}



/**
  * @brief  This function setup Watch Dog.
  * @retval None
*/

void MX_IWDG_Init(void){

  	LL_IWDG_Enable(IWDG);
  	LL_IWDG_EnableWriteAccess(IWDG);
	LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32);
	LL_IWDG_SetWindow(IWDG, 4095);
	LL_IWDG_SetReloadCounter(IWDG, 4095);
	while (LL_IWDG_IsReady(IWDG) != 1);

	LL_IWDG_ReloadCounter(IWDG);
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

