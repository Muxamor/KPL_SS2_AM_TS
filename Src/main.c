
/* Includes ------------------------------------------------------------------*/



#include "main.h"
#include "SetupPeriph.h"
#include "conf_a_module.h"
#include "uart_comm.h"
#include "spi_adc.h"
#include "i2c.h"

#include  <stdio.h>


/****************************TODO*************************

1. в conf_a_module выяснить как работать со статусом модуля побитно или по числу status_ready;

3. Доделать проверки ошибоу в I2C.c (возможно и не делать так как используется только при включении)

6. Проверить 			while( VALUE_COMP3() != RESET || i != 1000000 ){
				i++;
			}	

			 сколько i != 1000000  по времени


**********************************************************/
//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release


_SETTINGS_MODULE config_mod, *CONF_MOD_ptr=&config_mod;

_UART_BUF uart1_buf, *UART1_BUF_ptr=&uart1_buf;


int main(void){

	uint8_t i=0, pin=1;

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
	LED_Yellow_HL1_ON();

	PB14_STOP_ADC_Set();   

	CONF_MOD_ptr->addr_module =I2C_Read_addr_a_module(I2C1, ADDR_I2C_TCA9554PWR);
	CONF_MOD_ptr-> addr_module_req_data_adc = (CONF_MOD_ptr->addr_module << 3)| 0x01;

	LED_Green_HL2_ON();
	LED_Green_HL3_ON();

	// manual settings if jumper is set
	if( LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_2) == RESET ){ //check jumper
		Manual_settings(CONF_MOD_ptr->addr_module, PWM_TIM2_CH2_PA1);
	}

	if(UART1_BUF_ptr->received_command_flag == SET){ //Get Command

		if(UART1_BUF_ptr->ADC_data_request_flag == SET){ //get request to sent data ADC
			UART1_BUF_ptr->ADC_data_request_flag=0; //clear flag interrupt

			//uint32_t SPI_Get_data_ADC7767 (SPI_TypeDef *SPIx){
			//TO DO Need to write processed DATA ADC. Reduced from 24 bit to 16 bit and prepare to send
			//TO DO Add check start flag 

 		} else { // parse command 


 		}



	}






	LL_USART_TransmitData9(USART1, 0x16B);

	//while(1){
		//LL_USART_TransmitData9(USART1, 0x06B);
	//}


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



	


while(1){
  printf("Now is succsses start \r\n");
  Error_Handler();
}

  /* Initialize all configured peripherals */
 // MX_GPIO_Init();

//  MX_I2C1_Init();
///  MX_IWDG_Init();
 // MX_SPI2_Init();
//  MX_TIM2_Init();
//  MX_TIM3_Init();
//  MX_USART1_UART_Init();

}



