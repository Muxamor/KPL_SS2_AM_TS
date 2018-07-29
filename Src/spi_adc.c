/*
 * IC_fn.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */

#include "stm32l4xx.h"
#include "SetupPeriph.h"
#include "stm32l4xx_ll_spi.h"
#include "spi_adc.h"

#include  <stdio.h>
/**
  * @brief  Get data ADC AD7767
  * @param  SPIx
  * @retval Data ADC 24 bits 
  */
uint32_t SPI_Get_data_ADC7767 (SPI_TypeDef *SPIx){

	uint32_t adc_data=0;
	uint8_t adc_data_mas[3]; 

	uint32_t counter=0;

	while(LL_SPI_IsActiveFlag_BSY(SPIx)==SET); //check that SPI not busy 

	for(uint8_t i=0; i<3; i++){
		LL_SPI_TransmitData8(SPIx, 0xFF);

		counter=0;
		while(LL_SPI_IsActiveFlag_TXE(SPIx) == RESET){
			counter++;
			if(counter==100000000){
				adc_data = 0xFFFFFFFF;
				Error_Handler();
				goto exit_error;
			}
		}

		counter=0;
		while(LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET){
			counter++;
			if(counter==100000000){
				adc_data = 0xFFFFFFFF;
				Error_Handler();
				goto exit_error;
			}

		}

		adc_data_mas[i] = LL_SPI_ReceiveData8(SPIx);
	}

	adc_data = (adc_data_mas[2]<<16) | (adc_data_mas[1]<<8) | (adc_data_mas[0]);

exit_error:

	#ifdef DEBUGprintf
			printf("RAW_Data_AD7767=%lu \r\n",(unsigned long)adc_data );
	#endif

	return adc_data;
}
