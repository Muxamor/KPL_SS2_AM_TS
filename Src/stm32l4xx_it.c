/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_it.h"

#include "SetupPeriph.h"
#include "conf_a_module.h"
#include "spi_adc.h"
#include "uart_comm.h"


#include "global_variables.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void){

    uint16_t input_data9b = 0;
    uint8_t input_data8b = 0;
   
    if(LL_USART_IsActiveFlag_RXNE(USART1)){

        input_data9b = LL_USART_ReceiveData9(USART1);

        if( (input_data9b & 0x0100) != 0 ){ //check that we got byte with address 

            input_data8b = (uint8_t)input_data9b;

            if( input_data8b == CONF_MOD_ptr->addr_module_req_data_adc ){ // check got request ADC data 

                UART1_BUF_ptr->ADC_data_request_flag = 1;
                UART1_BUF_ptr->received_command_flag = 1;

            } else if( (input_data8b == 0x00) || ((input_data8b>>3) == CONF_MOD_ptr->addr_module) ){ // chaeck we got broadcast message or me address module

                  if( UART1_BUF_ptr->received_command_flag == 1){ //Protection if you do not have time to process previous massage.
                    UART1_BUF_ptr->received_command_flag = 0;//Clear previous message
                  }
                  
                  UART1_BUF_ptr->UART_Recive_Buf[0] = input_data8b;
                  UART1_BUF_ptr->UART_rec_buf_len = 1;
                  UART1_BUF_ptr->recive_data_permit_flag = 1;
            }

        } else if( UART1_BUF_ptr->recive_data_permit_flag == 1 ){ // getting data if we have permit to recive byte with data 

            UART1_BUF_ptr->UART_Recive_Buf[ UART1_BUF_ptr->UART_rec_buf_len ] = (uint8_t)input_data9b;
            UART1_BUF_ptr->UART_rec_buf_len++;

            if( UART1_BUF_ptr->UART_rec_buf_len == 4 ){ // finish getting command

                UART1_BUF_ptr->received_command_flag = 1;
                UART1_BUF_ptr->recive_data_permit_flag = 0;
            }
        }

    }

  // LL_USART_RequestRxDataFlush(USART1); -- на слуяай если не вычитываем пришедшие данные

  	/*
  	LL_USART_IsActiveFlag_RXNE(USART_TypeDef *USARTx) 
  	RXNE
  	: Read data register not empty
  	This bit is set by hardware when the content
  	of the RDR shift register has been transferred
  	to the USART_RDR register. It is cleared by
  	 a read to the USART_RDR register. The RXNE
  	flag can also be cleared by writing 1 to the RXFRQ in the USART_RQR register.
  	An interrupt is generated if RXNEIE=1 in the USART_CR1 register.
  	0: data is not received
  	1: Received data is ready to be read.*/
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void){

	//interrupt PB15 ADC-DRDY  FALLING edge
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET){
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);

    ADC_PARAM_ptr->ADC_DRDY_flag = 1;
    ADC_PARAM_ptr->DRDY_GOOD_flag = 0;
    ADC_PARAM_ptr->PULSE_flag = 0;

    INTERRUPT_DRDY_GOOD_Enable();
    INTERRUPT_PULSE_Enable();
	}

}

void EXTI9_5_IRQHandler(void){

  //interrupt PC5 DRDY_GOOD  RISING edge
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_5) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);

    ADC_PARAM_ptr->DRDY_GOOD_flag = 1;

    INTERRUPT_DRDY_GOOD_Disable();
    INTERRUPT_PULSE_Disable();
    ADC_PARAM_ptr->PULSE_flag = 0;
  }

  //interrupt PB8 PULSE FALLING edge
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);

    ADC_PARAM_ptr->PULSE_flag = 1;
    INTERRUPT_PULSE_Disable();
    INTERRUPT_DRDY_GOOD_Disable();
  }

}
  
void EXTI2_IRQHandler(void){

  //interrupt PB15 ADC-DRDY  FALLING edge
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET){
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);

   //LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15)
   LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_2); 
  }

}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
