/*
 * spi_adc.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */

#include "stm32l4xx.h"
#include "stm32l4xx_ll_spi.h"
#include "SetupPeriph.h"
#include "conf_a_module.h"
#include "spi_adc.h"

#include  <stdio.h>

/**
  * @brief  Get data ADC AD7767
  * @param  SPIx
  * @retval Data ADC 24 bits 
  */
int32_t SPI_Get_RAW_data_ADC7767 ( SPI_TypeDef *SPIx ){

	uint32_t counter=0;
	int32_t data_adc_raw=0;
	uint8_t adc_data_mas[3] = {0};

	while(LL_SPI_IsActiveFlag_BSY(SPIx)==SET); //check that SPI not busy 

	for(uint8_t i=0; i<3; i++){
		LL_SPI_TransmitData8(SPIx, 0x00);

		counter=0;
		while(LL_SPI_IsActiveFlag_TXE(SPIx) == RESET){
			counter++;
			if(counter==1000000){//150ms
				Error_Handler();
				goto exit_error;
			}
		}

		counter=0;
		while(LL_SPI_IsActiveFlag_RXNE(SPIx) == RESET){
			counter++;
			if(counter==1000000){ //150ms
				Error_Handler();
				goto exit_error;
			}

		}

		adc_data_mas[i] = LL_SPI_ReceiveData8(SPIx);
	}

	data_adc_raw = (adc_data_mas[0]<<16) | (adc_data_mas[1]<<8) | (adc_data_mas[2]);


	#ifdef DEBUGprintf
			printf("RAW_Data_AD7767=%lu \r\n",(unsigned long)data_adc_raw);
	#endif

	return data_adc_raw;

	exit_error:
	#ifdef DEBUGprintf
		printf("ERROR!Read ADC through SPI\r\n" );
	#endif

	return -1;
}


/**
  * @brief  Convert raw data ADC 24 to 16 bits 
  * @param  24 bit data ADC
  * @retval 16 bit data ADC 
  */

int16_t convert_RAW_data_ADC_24b_to_16b( int32_t raw_data_adc_24b, uint8_t Vref_adc, _SETTINGS_MODULE *config_module ){

	//raw_data_adc_24b =  0x800001; //0x7FFFFF;//
	int16_t RAW_DATA_16_ADC = 0;

	float DATA_24_ADC = 0;
	float DATA_16_ADC = 0;

	if ( (raw_data_adc_24b & 0x800000) == 0x800000 ){
		raw_data_adc_24b = raw_data_adc_24b & 0x7FFFFF; 
		raw_data_adc_24b = raw_data_adc_24b | 0xFF800000; 
	}

	if(config_module->amp_factor_K2 == 10 ){
		raw_data_adc_24b = raw_data_adc_24b * 2;

	} else if (config_module->amp_factor_K2 == 11){
		raw_data_adc_24b = raw_data_adc_24b * 4;
	}

	if( raw_data_adc_24b > 8388607 ){
		raw_data_adc_24b = 8388607;
		config_module->saturation_math_COMP4 = 1;
		
	}else if(raw_data_adc_24b < -8388608 ){
		raw_data_adc_24b = -8388608;
		config_module->saturation_math_COMP4 = 1;

	}

	DATA_24_ADC = (((float)raw_data_adc_24b)/16777216)*Vref_adc;

	DATA_16_ADC = ((DATA_24_ADC* 65536)/Vref_adc);

	if(DATA_16_ADC<0){
		if(DATA_16_ADC > -32768){
			DATA_16_ADC = DATA_16_ADC - 0.55;
		}
	}else{
		if(DATA_16_ADC < 32767){
			DATA_16_ADC = DATA_16_ADC + 0.55;
		}
	}
			
	//RAW_DATA_16_ADC = (int16_t)((DATA_24_ADC* 65536)/Vref_adc);
	RAW_DATA_16_ADC = (int16_t)DATA_16_ADC;

	#ifdef DEBUGprintf
			printf("DATA_ADC_24b=%f \r\n DATA_16_ADC =%f \r\n DATA_16_ADC = %lu \r\n", DATA_24_ADC, DATA_16_ADC, (signed long)RAW_DATA_16_ADC );
	#endif

	return RAW_DATA_16_ADC;
} 




