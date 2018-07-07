
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "SetupPeriph.h"
#include  <stdio.h>

/****************************TODO*************************

1. Протестировать настройку LL_GPIO_OUTPUT_OPENDRAIN дляLED HL1, HL2, HL3

2. Выяснить настройку при K1=1.



**********************************************************/
// LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

int main(void){

	LL_Init();
	SystemClock_Config(); //Setup sysytem clock at 80 MHz
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	printf("Hello human. I am Analog module.\r\n");
	printf("Start setup periphery STM32L452.....\r\n");
	SetupLED(); //протестить с LL_GPIO_OUTPUT_OPENDRAIN
	SetupGPIO();
	USART1_Init();
	SPI2_Init();
	Setup_I2C();

	SetupInterrupt();

	//MX_IWDG_Init();

	printf("Finish setup. Success! \r\n");


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



