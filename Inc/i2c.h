

#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

 extern void _Error_Handler(char *, int);

 uint8_t I2C_Read_addr_a_module(I2C_TypeDef *I2Cx,uint32_t SlaveAddr);

#ifdef __cplusplus
}
#endif


#endif /* __I2C_H */
