

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

 }_ADC_PARAMETERS;



 uint32_t SPI_Get_data_ADC7767 (SPI_TypeDef *SPIx);




#ifdef __cplusplus
}
#endif


#endif /* __SPI_ADC_H */
