
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_COMM_H
#define __UART_COMM_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_rcc.h"


 extern void _Error_Handler(char *, int);

 typedef struct{

	uint8_t UART_Recive_Buf[4];
	//uint8_t UART_Transmite_Buf[4]; // Подумать на счет удалить 

	uint8_t UART_rec_buf_len;
	uint8_t recive_data_permit_flag; //1=Yes 0=No
 	uint8_t received_command_flag; 
 	uint8_t ADC_data_request_flag; // 1 = Need send data ADC

 }_UART_BUF;







#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
