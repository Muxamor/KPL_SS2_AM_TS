/*
 * uart_comm.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */

/* Includes ------------------------------------------------------------------*/

#include "uart_comm.h"

ErrorStatus parser_command ( _UART_BUF *uart_receive_buffer, _SETTINGS_MODULE * module_settings, enum PWR_TIMx timer_numberr){

	uint8_t number_command = 0xFF;
	uint8_t ack_transmite_buf[4]=0x00;
	uint8_t transmite_data_flag=0; // =0 - NO ANSWER, =1 - answer the request value
	ErrorStatus retval;

	ack_transmite_buf[0] = uart_receive_buffer->UART_Recive_Buf[0];
	
	ack_transmite_buf[2] = 0x00;
	ack_transmite_buf[3] = 0x00;

	number_command = 0x07 & uart_receive_buffer->UART_Recive_Buf[0];

	if( number_command == 0x0 ){ // Get command Start or Stop

		if( uart_receive_buffer->UART_Recive_Buf[1] == 0xFF ){ //Satart command
			module_settings->start_stop_ADC = 0x02; //Start ADC

		} else if( uart_receive_buffer->UART_Recive_Buf[1] == 0x00 ){
			module_settings->start_stop_ADC = 0x00; ///stop without stop ADC

		} else if( uart_receive_buffer->UART_Recive_Buf[1] == 0x01 ){
			module_settings->start_stop_ADC = 0x01; //stop with stop ADC 
			PB14_STOP_ADC_Reset();
			LL_mDelay(1);
			PB14_STOP_ADC_Set();   

		} else{
			module_settings->status_module = 0x02; // not understand command
		}

		transmite_data_flag = 0;

	} else if( number_command == 0x02 ){ // Get command amplifier factor K1

		if( uart_receive_buffer->UART_Recive_Buf[1] == 0x80){ // request of value K1
			ack_transmite_buf[1] = module_settings->amp_factor_K1;
			transmite_data_flag = 1;

		} else { // amplifier factor K1
			/*
			if( VALUE_COMP1() == SET || VALUE_COMP2() == SET ){
				while( VALUE_COMP3() != RESET || i != 1000000 ){
					i++;
				}			
			}*/
			retval = Set_Amp_Factor_K1( uart_receive_buffer->UART_Recive_Buf[1] & 0X03 ); 

			if(retval == SUCCESS ){
				module_settings->amp_factor_K1 = uart_receive_buffer->UART_Recive_Buf[1] & 0X03 ;
				ack_transmite_buf[1] = 0x01;

			}else{
				ack_transmite_buf[1] = 0x00;
				ack_transmite_buf[3] = 0xFF;
			}

			transmite_data_flag = 1;
		}

	} else if( number_command == 0x03){ // Get command amplifier factor K2

		if( uart_receive_buffer->UART_Recive_Buf[1] == 0x80){ // request of value K1
			ack_transmite_buf[1] = module_settings->amp_factor_K2;
			transmite_data_flag = 1;

		} else { 

			retval = Set_Amp_Factor_K2( uart_receive_buffer->UART_Recive_Buf[1] & 0X0F );

			if(retval == SUCCESS ){
				module_settings->amp_factor_K2 = uart_receive_buffer->UART_Recive_Buf[1] & 0X0F ;
				ack_transmite_buf[1] = 0x01;

			}else{
				ack_transmite_buf[1] = 0x00;
				ack_transmite_buf[3] = 0xFF;
			}

			transmite_data_flag = 1;
		}

	} else if( number_command == 0x04 ){ // Get command cutof frequency 
		

	} else if( number_command == 0x05 ){ // Get command status command


	} else{

		//Error
	}




}

 	uint8_t addr_module;
 	uint8_t addr_module_req_data_adc; //aaaa a001'B
 	uint8_t start_stop_ADC; // 0-Stop witout stop ADC,   1 - stop with stop ADC,  2 - Start ADC 
 	uint8_t amp_factor_K1;  // value of amplifier factor K1
 	uint8_t amp_factor_K2;  // value of amplifier factor K2
 	uint8_t Fcut_value; // Cutof frequency
 	uint8_t status_ready; /// Спросить как првильно  по битам или просто число 




 typedef struct{

	uint8_t UART_Recive_Buf[4];
	//uint8_t UART_Transmite_Buf[4]; // Подумать на счет удалить 

	uint8_t UART_rec_buf_len;
	uint8_t recive_data_permit_flag; //1=Yes 0=No
 	uint8_t received_command_flag; 
 	uint8_t ADC_data_request_flag; // if 1 = Need send data ADC

 }_UART_BUF;

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
