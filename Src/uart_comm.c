/*
 * uart_comm.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"
#include "SetupPeriph.h"
#include "spi_adc.h"
#include "conf_a_module.h"
#include "uart_comm.h"


//#include "global_variables.h"

#include  <stdio.h>


ErrorStatus Data_transmite_UART_9B (uint16_t mass[], USART_TypeDef *USARTx){

	uint32_t counter=0;

	for ( uint8_t i=0 ; i<4; i++ ){

		counter=0;
		while( LL_USART_IsActiveFlag_TXE(USARTx) == RESET ){
			counter++;
			if(counter==100000000){
				Error_Handler();
				goto exit_error;
			}
		}

		if (i==0){
			LL_USART_TransmitData9( USARTx, 0x1FF & mass[i] );

		}else{
			LL_USART_TransmitData9( USARTx, mass[i] );

		}
	}

	counter=0;
	while( LL_USART_IsActiveFlag_TC( USARTx ) == RESET ){
		counter++;
		if(counter==100000000){
			Error_Handler();
			goto exit_error;
		}
	}

	return SUCCESS;

	exit_error:

	#ifdef DEBUGprintf
			printf("ERROR transmitting data through UART1");
	#endif

	return ERROR;
}




/**
  * @brief  Parser command from master module
  * @param  
  * @retval void
  */

void Parser_command ( _UART_BUF uart_receive_buffer, _SETTINGS_MODULE *module_settings, _ADC_PARAMETERS *adc_parametrs ,enum PWR_TIMx timer_numberr,  USART_TypeDef *USARTx){

	uint8_t number_command = 0xFF;
	uint8_t ack_transmite_buf[4];
	uint8_t transmite_data_flag=0; // =0 - NO ANSWER, =1 - answer at request  
	ErrorStatus retval;

	ack_transmite_buf[0] = uart_receive_buffer.UART_Recive_Buf[0];
	
	ack_transmite_buf[2] = 0x00;
	ack_transmite_buf[3] = 0x00;

	number_command = 0x07 & uart_receive_buffer.UART_Recive_Buf[0];

	if( number_command == 0x0 ){ // Get command Start or Stop
		if( uart_receive_buffer.UART_Recive_Buf[1] == 0xFF ){ //Satart command
			module_settings->start_stop_ADC = 0x02; //Start ADC
			NVIC_EnableIRQ(EXTI15_10_IRQn);
			NVIC_EnableIRQ(EXTI9_5_IRQn);
			adc_parametrs->ADC_DRDY_flag=0; 

		} else if( uart_receive_buffer.UART_Recive_Buf[1] == 0x00 ){
			module_settings->start_stop_ADC = 0x00; ///stop without stop ADC
			NVIC_DisableIRQ(EXTI15_10_IRQn);
			NVIC_DisableIRQ(EXTI9_5_IRQn);

		} else if( uart_receive_buffer.UART_Recive_Buf[1] == 0x01 ){
			module_settings->start_stop_ADC = 0x01; //stop with stop ADC 
			NVIC_DisableIRQ(EXTI15_10_IRQn);
			NVIC_DisableIRQ(EXTI9_5_IRQn);

			PB14_STOP_ADC_Reset();
			LL_mDelay(1);
			PB14_STOP_ADC_Set();   

		} else{
			module_settings->status_module = 0x02; // not understand command
		}

		transmite_data_flag = 0;

	} else if( number_command == 0x02 ){ // Get command amplifier factor K1
		if( uart_receive_buffer.UART_Recive_Buf[1] == 0x80){ // request of value K1
			ack_transmite_buf[1] = module_settings->amp_factor_K1;
			transmite_data_flag = 1;

		} else { // amplifier factor K1
			/*
			if( VALUE_COMP1() == SET || VALUE_COMP2() == SET ){
				while( VALUE_COMP3() != RESET || i != 1000000 ){
					i++;
				}			
			}*/
			retval = Set_Amp_Factor_K1( uart_receive_buffer.UART_Recive_Buf[1] & 0X03 ); 

			if(retval == SUCCESS ){
				module_settings->amp_factor_K1 = uart_receive_buffer.UART_Recive_Buf[1] & 0X03;
				ack_transmite_buf[1] = 0x01;

			}else{
				ack_transmite_buf[1] = 0x00;
				ack_transmite_buf[3] = 0xFF;
			}

			transmite_data_flag = 1;
		}

	} else if( number_command == 0x03){ // Get command amplifier factor K2
		if( uart_receive_buffer.UART_Recive_Buf[1] == 0x80){ // request of value K1
			ack_transmite_buf[1] = module_settings->amp_factor_K2;
			transmite_data_flag = 1;

		} else { 
			retval = Set_Amp_Factor_K2( uart_receive_buffer.UART_Recive_Buf[1] & 0X0F );

			if(retval == SUCCESS ){
				module_settings->amp_factor_K2 = uart_receive_buffer.UART_Recive_Buf[1] & 0X0F ;
				ack_transmite_buf[1] = 0x01;

			}else{
				ack_transmite_buf[1] = 0x00;
				ack_transmite_buf[3] = 0xFF;
			}

			transmite_data_flag = 1;
		}

	} else if( number_command == 0x04 ){ // Get command cutof frequency 

		if( uart_receive_buffer.UART_Recive_Buf[1] == 0x80){ // request of value K1
			ack_transmite_buf[1] = module_settings->Fcut_value;
			transmite_data_flag = 1;

		} else { 

			retval = Set_Ficlk_and_F_SAx( uart_receive_buffer.UART_Recive_Buf[1] & 0X7F , timer_numberr );

			if(retval == SUCCESS ){
				module_settings->Fcut_value = uart_receive_buffer.UART_Recive_Buf[1] & 0X7F ;
				ack_transmite_buf[1] = 0x01;

			}else{
				ack_transmite_buf[1] = 0x00;
				ack_transmite_buf[3] = 0xFF;
			}

			transmite_data_flag = 1;
		}

	} else if( number_command == 0x05 ){ // Get command status command
		ack_transmite_buf[1] = module_settings->status_module;
		transmite_data_flag = 1;

	} else{ // Error in command 
		ack_transmite_buf[1] = 0x00;
		ack_transmite_buf[3] = 0xFF;
		transmite_data_flag = 1;
	}

	if( transmite_data_flag == 1 ){
		Data_transmite_UART_9B ((uint16_t*) ack_transmite_buf, USARTx);
	}

}






