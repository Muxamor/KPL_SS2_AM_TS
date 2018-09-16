/*
 * i2c.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */

#include "stm32l4xx.h"
#include "SetupPeriph.h"
#include "stm32l4xx_ll_i2c.h"
#include "i2c.h"

#include  <stdio.h>

/**
  * @brief  Read address analog module throught I2C TCA9554PWR
  * @param  namber_value
  * @retval An ErrorStatus enumeration
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
uint8_t I2C_Read_addr_a_module(I2C_TypeDef *I2Cx,uint32_t SlaveAddr){

	uint8_t address_module;

	SlaveAddr=SlaveAddr<<1;
	
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx)==SET); 

	LL_I2C_HandleTransfer(I2Cx, SlaveAddr,LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE ); //LL_I2C_GENERATE_START_READ
	while(LL_I2C_IsActiveFlag_TXE(I2Cx)==RESET);


	LL_I2C_TransmitData8(I2Cx, 0x00);
	while(LL_I2C_IsActiveFlag_TXE(I2Cx)==RESET);
	while(LL_I2C_IsActiveFlag_TC(I2Cx)==RESET);

	LL_I2C_HandleTransfer(I2Cx, SlaveAddr,LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_RESTART_7BIT_READ); //LL_I2C_MODE_SOFTEND

	while(LL_I2C_IsActiveFlag_STOP(I2Cx)==RESET);
	while(LL_I2C_IsActiveFlag_RXNE(I2Cx)==RESET);

	address_module=(0x1F & (LL_I2C_ReceiveData8(I2Cx))); //0x1F & - because address contain only 5 bits (xxxAAAAA)

	LL_I2C_ClearFlag_STOP(I2Cx);

#ifdef DEBUGprintf
			printf("Address Module=%d \r\n", address_module);
#endif

	return address_module;

}

