

#ifndef __CONF_A_MODULE_H
#define __CONF_A_MODULE_H

#ifdef __cplusplus
 extern "C" {
#endif


 extern void _Error_Handler(char *, int);

 typedef struct{
 	uint8_t addr_module;
 	uint8_t addr_module_req_data_adc; //aaaa a001'B

 }_SETTINGS_MODULE;

 ErrorStatus Set_Ficlk_and_F_SAx(uint8_t par_value, enum PWR_TIMx timer_number);
 ErrorStatus Set_Amp_Factor_K1(uint8_t namber_value);
 ErrorStatus Set_Amp_Factor_K2(uint8_t namber_value);
 ErrorStatus Manual_settings(uint8_t namber_settings, enum PWR_TIMx timer_numberr);








#ifdef __cplusplus
}
#endif


#endif /* __CONF_A_MODULE_H */
