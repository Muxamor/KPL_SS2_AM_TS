/**
  ******************************************************************************
  * @author  Ivan Neskorodev
  * @Emil    ivan.neskorodev@gmail.com
  * @version V0.0
  * @date    09.07.2018
  * @brief   Function for work with IC on the SS board
  *
  *
  ******************************************************************************
*/

#include "stm32l4xx.h"
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
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_usart.h"
#include "SetupPeriph.h"

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

  	//Enable HSE
  	LL_RCC_HSE_EnableBypass();
  	LL_RCC_HSE_Enable();
   	// Wait till HSE is ready
  	while(LL_RCC_HSE_IsReady() != 1);

	/*
	//Enable HSI
	LL_RCC_HSI_Enable();
	//Wait till HSI is ready
	while(LL_RCC_HSI_IsReady() != 1);
	//LL_RCC_HSI_SetCalibTrimming(16);//not need to do this
	*/

	// Enable LSI
	LL_RCC_LSI_Enable();

	/* Wait till LSI is ready */
	while(LL_RCC_LSI_IsReady() != 1);

	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2); // LL_RCC_PLLSOURCE_HSE
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

  /* Setup clock source for USART1 and I2C */
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
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
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
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);


	/* Configure pins out for control preamplifier K1. PA3=10 PA4=100 PA5=1000 for AG1411 ON=0 OFF=1    */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*off K1 */
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5);

	/*For setup K1 need use:   		                */
	/*PA3_10_Set()      PA3_10_Reset()            	*/
	/*PA4_100_Set()     PA4_100_Reset()          	*/
	/*PA5_1000_Set()    PA5_1000_Reset()            */
	/*Define in SetupPeriph.h                       */



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
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8|LL_GPIO_PIN_9);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);

	/*For SA0, SA1, SA2 need use:   		        */
	/*F_SA0_Set()       F_SA0_Reset()            	*/
	/*F_SA1_Set()       F_SA1_Reset()          		*/
	/*F_SA2_Set()       F_SA2_Reset()               */
	/*Define in SetupPeriph.h                       */

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
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*For setup K2 need use:   		                */
	/*PB0_G0_Set()      PB0_G0_Reset()            	*/
	/*PB1_G1_Set()      PB1_G1_Reset()          	*/
	/*PB2_G2_Set()      PB2_G2_Reset()              */
	/*PB10_G3_Set()     PB10_G3_Reset()             */
	/*PB11_G4_Set()     PB11_G4_Reset()             */
	/*Define in SetupPeriph.h                       */

	/*Config input pin to start manual settup */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* Configure pins STOP-ADC out: PB14= STOP-ADC*/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*For setup STOP-ADC need use:                */
  /*PB14_STOP_ADC_Set()                         */
  /*PB14_STOP_ADC_Reset()                       */
  /*Define in SetupPeriph.h                     */



	//удалить только для отладки на NUcleo board
//	GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
//	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);


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
  	USART_InitStruct.DataWidth =  LL_USART_DATAWIDTH_9B;
  	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  	LL_USART_Init(USART1, &USART_InitStruct);

  	LL_USART_SetTXRXSwap(USART1, LL_USART_TXRX_SWAPPED);

  	LL_USART_ConfigAsyncMode(USART1); 
  	LL_USART_Enable(USART1);

    /* Configure pins RE and TE to control transfer data throughISO3086DW, PA11=RE PA12=DE */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_11|LL_GPIO_PIN_12;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /**/
    //RE -Enable when low
    //TE - Enbale when high
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11); //Enable receive data
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12); //Enable transmit data

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
  	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2; /// Скорость обмена нужно уточнить у Саши
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
void I2C1_Init(void){

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
  	I2C_InitStruct.Timing = 0x10909CEC;//  - Frequenci I2C=100 kHz //0x00702991; - Frequenci I2C=400 kHz
  	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  	I2C_InitStruct.DigitalFilter = 0;
  	I2C_InitStruct.OwnAddress1 = 0;
  	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK; //LL_I2C_NACK 
  	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  	LL_I2C_Init(I2C1, &I2C_InitStruct);

  	LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);

  	LL_I2C_Enable(I2C1);
}

/**
  * @brief  Setup PWM Timer2 or Timer 3.
  * @param  None
  * @retval None
*/
void PWM_Init(enum PWR_TIMx timer_number){

    LL_TIM_InitTypeDef TIM_InitStruct;
    LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    if (timer_number == PWM_TIM2_CH2_PA1){

    	//Setup GPIO TIM2_CH2 ------> PA1
    	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    	/* TIM2 clock enable */
    	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    	LL_TIM_StructInit(&TIM_InitStruct);
    	TIM_InitStruct.Prescaler = 0;//(SystemCoreClock/1000000)-1;
    	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    	TIM_InitStruct.Autoreload = 61538-1;
    	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    	LL_TIM_Init(TIM2, &TIM_InitStruct);

    	LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
    	LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH2);

    	LL_TIM_OC_StructInit(&TIM_OC_InitStruct);
    	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
    	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
    	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
    	TIM_OC_InitStruct.CompareValue = 30769-1;
    	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);

    	LL_TIM_EnableCounter(TIM2);

    } else if (timer_number == PWM_TIM3_CH1_PA6){

    	//Setup GPIO TIM3_CH1 ------> PA6
    	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
    	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    	/* TIM2 clock enable */
    	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

    	LL_TIM_StructInit(&TIM_InitStruct);
    	TIM_InitStruct.Prescaler = 0;//(SystemCoreClock/1000000)-1;;
    	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    	TIM_InitStruct.Autoreload = 61538-1;
    	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    	LL_TIM_Init(TIM3, &TIM_InitStruct);

        LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
        LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);

        LL_TIM_OC_StructInit(&TIM_OC_InitStruct);
        TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
        TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
        TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
        TIM_OC_InitStruct.CompareValue = 30769-1;
        TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
        LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);

        LL_TIM_EnableCounter(TIM3);
    }

}

/**
  * @brief  This function setup interrupts for all ports and inside event .
  * @param  None
  * @retval None
*/
void SetupInterrupt(void){
	
    LL_EXTI_InitTypeDef EXTI_InitStruct;

  	/* Setup USART1 interrupt Init */
  	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0)); //Set priority №1 from 0..15
 	  LL_USART_EnableIT_RXNE(USART1); //Enable RX no empty Interrupt
  	//LL_USART_DisableIT_RXNE(USART1);
  	NVIC_EnableIRQ(USART1_IRQn);
  	/**********************************************/

  	/*Setup interrupt PB15 ADC-DRDY*/
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE15);

    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_15;
    EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);

    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_INPUT);


    /*For enable/desible interrupt ADC_DRDY need use:                                          */
    /*INTERRUPT_ADC_DRDY_Enable()       LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15)                 */
    /*INTERRUPT_ADC_DRDY_Disable()      LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_15)                */
    /*Define in SetupPeriph.h                                                                  */

    // EXTI interrupt init
    NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0)); //Set priority №2 from 0..15
    NVIC_DisableIRQ(EXTI15_10_IRQn);
    //NVIC_EnableIRQ(EXTI15_10_IRQn);
    /**********************************************/
 	  


    /*Setup interrupt PC5 DRDY_GOOD*/
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE5);

    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_5;
    EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
    LL_EXTI_Init(&EXTI_InitStruct);

    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_5, LL_GPIO_MODE_INPUT);

    /*For enable/desible interrupt DRDY_GOOD need use:                                        */
    /*INTERRUPT_DRDY_GOOD_Enable()      LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_5)                 */
    /*INTERRUPT_DRDY_GOOD_Disable()     LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_5)                */
    /*Define in SetupPeriph.h                                                                 */



    /*Setup interrupt PB8 PULSE*/
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE8);

    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8;
    EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);

    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_INPUT);

    /*For enable/desible interrupt PULSE need use:                  */
    /*INTERRUPT_PULSE_Enable()           LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8)                 */
    /*INTERRUPT_PULSE_Disable()          LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8)                */
    /*Define in SetupPeriph.h                          */

    // EXTI interrupt init
    NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0)); //Set priority №3 from 0..15
    NVIC_DisableIRQ(EXTI9_5_IRQn);
    //NVIC_EnableIRQ(EXTI9_5_IRQn);

    /***************************************************/

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

