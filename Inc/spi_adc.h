

#ifndef __SPI_ADC_H
#define __SPI_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif


  extern void _Error_Handler(char *, int);


 typedef struct {
 	uint8_t ADC_DRDY_flag; //=1 - ADC data ready to read 
 	uint8_t DRDY_GOOD_flag; //=1 - got DRDY_GOOD it is mean that we no have error in synchronization in ADC
 	uint8_t PULSE_flag; //=1 - got PULSE 
 	uint8_t Count_MCLK; // = counter MCLK

 }_ADC_PARAMETERS;



int32_t SPI_Get_RAW_data_ADC7767 ( SPI_TypeDef *SPIx );
uint32_t SPI_Get_RAW_data_ADC7767_unsigned ( SPI_TypeDef *SPIx );

int16_t Math_convert_RAW_data_ADC_24b_to_16b( int32_t raw_data_adc_24b, float  Vref_adc, _SETTINGS_MODULE *config_module );




#ifdef __cplusplus
}
#endif


#endif /* __SPI_ADC_H */
