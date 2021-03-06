
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_COMM_H
#define __UART_COMM_H
#ifdef __cplusplus
 extern "C" {
#endif

 extern void _Error_Handler(char *, int);

 typedef struct{

	uint8_t UART_Recive_Buf[4];
	uint8_t UART_rec_buf_len;
	uint8_t recive_data_permit_flag; //1=Yes 0=No
 	uint8_t received_command_flag; 
 	uint8_t ADC_data_request_flag; // if 1 = Need send data ADC

 }_UART_BUF;

ErrorStatus Data_transmite_UART_9B (uint8_t mass[], USART_TypeDef *USARTx);
void Parser_command ( _UART_BUF uart_receive_buffer, _SETTINGS_MODULE * module_settings, _ADC_PARAMETERS *adc_parametrs ,enum PWR_TIMx timer_numberr,  USART_TypeDef *USARTx);


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
