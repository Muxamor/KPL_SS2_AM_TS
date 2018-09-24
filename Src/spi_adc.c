/*
 * spi_adc.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */

#include "stm32l4xx.h"
#include "stm32l4xx_ll_spi.h"
#include "SetupPeriph.h"
#include "spi_adc.h"

#include  <stdio.h>
/**
  * @brief  Get data ADC AD7767
  * @param  SPIx
  * @retval Data ADC 24 bits 
  */
ErrorStatus SPI_Get_data_ADC7767 ( uint8_t adc_data_mas[], uint8_t size_mas,SPI_TypeDef *SPIx){

	uint32_t counter=0;

	while(LL_SPI_IsActiveFlag_BSY(SPIx)==SET); //check that SPI not busy 

	for(uint8_t i=0; i<size_mas; i++){
		LL_SPI_TransmitData8(SPIx, 0x00);

		counter=0;
		while(LL_SPI_IsActiveFlag_TXE(SPIx) == RESET){
			counter++;
			if(counter==10000000){
				Error_Handler();
				goto exit_error;
			}
		}

		counter=0;
		while(LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET){
			counter++;
			if(counter==10000000){
				Error_Handler();
				goto exit_error;
			}

		}

		adc_data_mas[i] = LL_SPI_ReceiveData8(SPIx);
	}


	#ifdef DEBUGprintf
			printf("RAW_Data_AD7767=%lu \r\n",(unsigned long)(adc_data_mas[0]<<16) | (adc_data_mas[1]<<8) | (adc_data_mas[2]));
	#endif

	return SUCCESS;

	exit_error:

	#ifdef DEBUGprintf
		printf("ERROR!Read ADC through SPI\r\n" );
	#endif

	return ERROR;
}

