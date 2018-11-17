
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "SetupPeriph.h"
#include "conf_a_module.h"
#include "spi_adc.h"
#include "i2c.h"
#include "uart_comm.h"

#include  <stdio.h>

/****************************TODO*************************
1!!.  Read settings of module from flash 
1. Write flash. If it will be need.
2. Don't forget to ON WatchDog

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
	
	//uint32_t RAW_DATA_24_ADC_unsigned = 0;

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
	IWDG_Init();
	printf("Finish setup periphery. Success! \r\n");
	
	//Default configuration board;
	PB14_STOP_ADC_Set(); 
	PC7_MCLK_Reset();
    ADC_PARAM_ptr->ADC_DRDY_flag = 0;
    ADC_PARAM_ptr->DRDY_GOOD_flag = 0;
    ADC_PARAM_ptr->PULSE_flag = 0;
    ADC_PARAM_ptr->Count_MCLK = 0;

    UART1_BUF_ptr->UART_rec_buf_len = 0; 
    UART1_BUF_ptr->recive_data_permit_flag = 0; 
    UART1_BUF_ptr->received_command_flag = 0;
    UART1_BUF_ptr->ADC_data_request_flag = 0;

    Set_Ficlk_and_F_SAx(1,PWM_TIM2_CH2_PA1);
    Set_Amp_Factor_K1(0);
    Set_Amp_Factor_K2(0);

    CONF_MOD_ptr->Fcut_value = 1;
    CONF_MOD_ptr->amp_factor_K1 = 0;
    CONF_MOD_ptr->amp_factor_K2 = 0;

    //TODO Read settings of module from flash.  
   	CONF_MOD_ptr->format_data_ADC_16b_24b = 0;

    printf("Default settings. Success! \r\n");

    LL_IWDG_ReloadCounter(IWDG);

	LED_Yellow_HL1_OFF();
	LED_Green_HL2_OFF();

	CONF_MOD_ptr->addr_module = I2C_Read_addr_a_module(I2C1, ADDR_I2C_TCA9554PWR);	
	
	// Manual settings if jumper is set
	if( LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_2) == RESET ){ //check jumper
		Manual_settings(CONF_MOD_ptr->addr_module, PWM_TIM2_CH2_PA1);
		CONF_MOD_ptr->addr_module = 0x01;
	}else{
		Set_Output_mode_PD2();
	} 

	CONF_MOD_ptr->addr_module_req_data_adc = (CONF_MOD_ptr->addr_module << 3)| 0x01;
	CONF_MOD_ptr->status_module = 0x01;

	LED_Green_HL3_ON();

/************Only for setup board********************************/
	/*uint32_t counter=0;
	while(1){

		if( VALUE_COMP1() == 1 ||  VALUE_COMP2() == 1 ){
			LED_Yellow_HL1_ON();
			counter=10000;

		}else if( VALUE_COMP4() == 1 ){
			LED_Green_HL2_ON();
			counter=10000;

		}else{
			if( counter != 0 ){
				counter--;
			}

			if(counter==1){
				LED_Yellow_HL1_OFF();
				LED_Green_HL2_OFF();
			}
		}
	}*/
/***********************************************************/

	while(1){

		LL_IWDG_ReloadCounter(IWDG);
		if(CONF_MOD_ptr->counter_toggle_led_hl3 == 128 ){
			CONF_MOD_ptr->counter_toggle_led_hl3=0;
			LED_Green_HL3_TOGGLE();
		}

		//Read ADC if we got ADC interrupt 
		if(CONF_MOD_ptr->start_stop_ADC == 0x02 && ADC_PARAM_ptr->ADC_DRDY_flag==1 ){

		//	PD2_Set(); // For to measure processing time

			ADC_PARAM_ptr->ADC_DRDY_flag=0;
			RAW_DATA_24_ADC = 0;
			ADC_data_transmit[0] = 0;
			//RAW_DATA_24_ADC_unsigned = 0;

			RAW_DATA_24_ADC = SPI_Get_RAW_data_ADC7767( SPI2 ); 
			//RAW_DATA_24_ADC_unsigned = SPI_Get_RAW_data_ADC7767_unsigned ( SPI2 );

			if(RAW_DATA_24_ADC == -1){ //SPI hardware problem

				RAW_DATA_16_ADC = 0;
				RAW_DATA_24_ADC = 0;
				CONF_MOD_ptr->status_module = 0x05;

			}else{ //No Error
			//	LL_IWDG_ReloadCounter(IWDG);
				LL_IWDG_ReloadCounter(IWDG);
				if(CONF_MOD_ptr->format_data_ADC_16b_24b == 0){ //16b format data ADC
					RAW_DATA_16_ADC = Math_convert_RAW_data_ADC_24b_to_16b( RAW_DATA_24_ADC, 5.0, CONF_MOD_ptr);
				}

				if( VALUE_COMP1() == 1 && VALUE_COMP2() == 0 && VALUE_COMP4() == 0){ 
					ADC_data_transmit[0] = 0x04;  //Error  COMP2
					CONF_MOD_ptr->status_module = 0x15;

				} else if( VALUE_COMP1() == 0 && VALUE_COMP2() ==1 && VALUE_COMP4() == 0){  
					ADC_data_transmit[0] = 0x02; //Error  COMP1
					CONF_MOD_ptr->status_module = 0x15;

				} else if( VALUE_COMP4() == 1 || CONF_MOD_ptr->saturation_math_COMP4 == 1 ){ 
					ADC_data_transmit[0] =  0x06; //Error  COMP4
					CONF_MOD_ptr->status_module = 0x15;
					CONF_MOD_ptr->saturation_math_COMP4 = 0;

				} else {
					ADC_data_transmit[0] = 0x01; // No Error
				} 
	
				if( ADC_PARAM_ptr->DRDY_GOOD_flag != 1 ){ // do not got sync signal

					Error_Handler();

					while(ADC_PARAM_ptr->PULSE_flag == SET);

					ADC_PARAM_ptr->PULSE_flag = 0;
					CONF_MOD_ptr->status_module = 0x15;

					if(ADC_PARAM_ptr->Count_MCLK  == 8 ){ // counter in interrupt
						Error_Handler();
						LED_Yellow_HL1_ON();
						CONF_MOD_ptr->status_module = 0x1D;
						ADC_PARAM_ptr->Count_MCLK = 0x00;
						RAW_DATA_16_ADC = 0;
					}

					ADC_data_transmit[0] = 0; //Set error flag in the parsel
				}
			}

			ADC_data_transmit[0] =  ADC_data_transmit[0] | (CONF_MOD_ptr->addr_module << 3);

			if(CONF_MOD_ptr->format_data_ADC_16b_24b == 0){ //16b format data ADC
				ADC_data_transmit[1] = 0x00;
				ADC_data_transmit[2] = (uint8_t)(RAW_DATA_16_ADC>>8);
				ADC_data_transmit[3] = (uint8_t)RAW_DATA_16_ADC;
			}else{
				ADC_data_transmit[1] = (uint8_t)(RAW_DATA_24_ADC>>16);//24b format data ADC
				ADC_data_transmit[2] = (uint8_t)(RAW_DATA_24_ADC>>8);
				ADC_data_transmit[3] = (uint8_t)RAW_DATA_24_ADC;
			}

			CONF_MOD_ptr->counter_toggle_led_hl3++;

		//	PD2_Reset(); // For to measure processing time
		}

		if(UART1_BUF_ptr->received_command_flag == SET){ //Get Command
			UART1_BUF_ptr->received_command_flag=RESET; //clear flag interrupt
			UART1_BUF_ptr->UART_rec_buf_len=0;

			LL_IWDG_ReloadCounter(IWDG);
			if(UART1_BUF_ptr->ADC_data_request_flag == SET){ //get request to sent data ADC
				UART1_BUF_ptr->ADC_data_request_flag=0; //clear flag interrupt
				Data_transmite_UART_9B (ADC_data_transmit, USART1);

 			} else { // parse command 
 				//LL_IWDG_ReloadCounter(IWDG);
 				Parser_command ( *UART1_BUF_ptr, CONF_MOD_ptr, ADC_PARAM_ptr, PWM_TIM2_CH2_PA1, USART1 );

  			}
		}
	}

}



