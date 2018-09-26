
/* Includes ------------------------------------------------------------------*/



#include "main.h"
#include "SetupPeriph.h"
#include "conf_a_module.h"
#include "spi_adc.h"
#include "i2c.h"
#include "uart_comm.h"

#include  <stdio.h>


/****************************TODO*************************

проверить на плате  настройку открытый коллектор для PB14_STOP_ADC

1. в conf_a_module выяснить как работать со статусом модуля побитно или по числу status_ready;

3. Доделать проверки ошибоу в I2C.c (возможно и не делать так как используется только при включении)

6. Проверить 			while( VALUE_COMP3() != RESET || i != 1000000 ){
				i++;
			}	

			 сколько i != 1000000  по времени

7. module_settings->status_module = 0x02; // или писать 0x00? 

**********************************************************/
//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release


_SETTINGS_MODULE config_mod, *CONF_MOD_ptr=&config_mod;

_UART_BUF uart1_buf, *UART1_BUF_ptr=&uart1_buf;

_ADC_PARAMETERS adc_param, *ADC_PARAM_ptr=&adc_param;


int main(void){


	uint8_t ADC_data_transmit[4];
	int16_t RAW_DATA_16_ADC = 0;
	int32_t RAW_DATA_24_ADC = 0;

	LL_Init();
	SystemClock_Config(); //Setup system clock at 80 MHz
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	printf("Hello human. I am Analog module.\r\n");
	printf("Start setup periphery STM32L452.....\r\n");
	SetupLED();
	SetupGPIO();
	USART1_Init();
	SPI2_Init();
	I2C1_Init();
	PWM_Init(PWM_TIM2_CH2_PA1); //PWM_TIM3_CH1_PA6
	SetupInterrupt();
	//MX_IWDG_Init();
	printf("Finish setup periphery. Success! \r\n");
	
	PB14_STOP_ADC_Set(); 
	PC7_MCLK_Reset();
    ADC_PARAM_ptr->ADC_DRDY_flag = 0;
    ADC_PARAM_ptr->DRDY_GOOD_flag = 0;
    ADC_PARAM_ptr->PULSE_flag = 0;
    ADC_PARAM_ptr->Count_MCLK = 0;
    printf("Default settings. Success! \r\n");


	LED_Yellow_HL1_ON();
	LED_Green_HL2_ON();

	CONF_MOD_ptr->addr_module =I2C_Read_addr_a_module(I2C1, ADDR_I2C_TCA9554PWR);	
	
	// Manual settings if jumper is set
	if( LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_2) == RESET ){ //check jumper
		Manual_settings(CONF_MOD_ptr->addr_module, PWM_TIM2_CH2_PA1);
		CONF_MOD_ptr->addr_module = 0x01;
	} 
	
	CONF_MOD_ptr-> addr_module_req_data_adc = (CONF_MOD_ptr->addr_module << 3)| 0x01;

	CONF_MOD_ptr->status_module = 0x01;

	while (1){

		LED_Green_HL3_ON();

		//Read ADC if we got ADC interrupt 
		if(CONF_MOD_ptr->start_stop_ADC == 0x02 && ADC_PARAM_ptr->ADC_DRDY_flag==1 ){

			ADC_PARAM_ptr->ADC_DRDY_flag=0;
			RAW_DATA_24_ADC = 0;

			RAW_DATA_24_ADC = SPI_Get_RAW_data_ADC7767( SPI2 ); 

			if(RAW_DATA_24_ADC == -1){ //SPI hardware problem

				RAW_DATA_16_ADC = 0;
				ADC_data_transmit[0] = ADC_data_transmit[0] & 0xF8;
				CONF_MOD_ptr->status_module = 0x05;

			}else{ //No Error

				RAW_DATA_16_ADC = convert_RAW_data_ADC_24b_to_16b( RAW_DATA_24_ADC, 5 );

				if( VALUE_COMP1() == 0 && VALUE_COMP2() == 1 && VALUE_COMP4() == 0){ 
					ADC_data_transmit[0] = 0x04;  //Error  COMP2
					CONF_MOD_ptr->status_module = 0x15;

				} else if( VALUE_COMP1() == 1 && VALUE_COMP2() ==0 && VALUE_COMP4() == 0){  
					ADC_data_transmit[0] = 0x02; //Error  COMP1
					CONF_MOD_ptr->status_module = 0x15;

				} else if( VALUE_COMP4() == 1 ){ 
					ADC_data_transmit[0] =  0x06; //Error  COMP4
					CONF_MOD_ptr->status_module = 0x15;

				} else {
					ADC_data_transmit[0] = 0x01; // No Error
				} 
	
				if( ADC_PARAM_ptr->DRDY_GOOD_flag != 1 ){ // do not got sync signal

					while(ADC_PARAM_ptr->PULSE_flag == 1);

					//ADC_PARAM_ptr->PULSE_flag = 0;
					CONF_MOD_ptr->status_module = 0x15;

					if(ADC_PARAM_ptr->Count_MCLK  == 8 ){
						CONF_MOD_ptr->status_module = 0x1D;
						ADC_PARAM_ptr->Count_MCLK = 0x00;
						RAW_DATA_16_ADC = 0;
					}

					ADC_data_transmit[0] = ADC_data_transmit[0] & 0xF8;
				}
			}

			ADC_data_transmit[0] =  ADC_data_transmit[0] | (CONF_MOD_ptr->addr_module << 3);
			ADC_data_transmit[1] = 0x00;
			ADC_data_transmit[2] = (uint8_t)(RAW_DATA_16_ADC>>8);
			ADC_data_transmit[3] = (uint8_t)RAW_DATA_16_ADC;

		}

	
		//////TO DO доделать !
		if(UART1_BUF_ptr->received_command_flag == SET){ //Get Command
		// обнулить длину буфера 
			if(UART1_BUF_ptr->ADC_data_request_flag == SET){ //get request to sent data ADC
				UART1_BUF_ptr->ADC_data_request_flag=0; //clear flag interrupt
				Data_transmite_UART_9B ((uint16_t*)ADC_data_transmit, USART1);

 			} else { // parse command 
 				UART1_BUF_ptr->received_command_flag=RESET; //clear flag interrupt
 				UART1_BUF_ptr->UART_rec_buf_len=0;
				Parser_command ( *UART1_BUF_ptr, CONF_MOD_ptr, ADC_PARAM_ptr , PWM_TIM2_CH2_PA1, USART1);

 			}
		}

	}

	



//******************below test zone******************************//


	LL_USART_TransmitData9(USART1, 0x16B);

	//while(1){
		//LL_USART_TransmitData9(USART1, 0x06B);
	//}

	uint8_t i=0;
	uint8_t pin=1;
//for test
	while(1){

		pin=LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13);

		if(pin==0){
			while(!LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13)){

			};

			i++;
			Set_Ficlk_and_F_SAx(i, PWM_TIM2_CH2_PA1);
			if(i==130){
				i=0;
			}
		}
	}



}



