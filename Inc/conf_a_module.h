

#ifndef __CONF_A_MODULE_H
#define __CONF_A_MODULE_H

#ifdef __cplusplus
 extern "C" {
#endif


 extern void _Error_Handler(char *, int);

 typedef struct{
 	uint8_t addr_module;
 	uint8_t addr_module_req_data_adc; //aaaa a001'B
 	uint8_t start_stop_ADC; // 0-Stop without stop ADC,   1 - stop with stop ADC,  2 - Start ADC 
 	uint8_t amp_factor_K1;  // value of amplifier factor K1
 	uint8_t amp_factor_K2;  // value of amplifier factor K2
 	uint8_t Fcut_value; // Cutof frequency
 	uint8_t status_module; /// Спросить как првильно  по битам или просто число 

 }_SETTINGS_MODULE;

 ErrorStatus Set_Ficlk_and_F_SAx(uint8_t par_value, enum PWR_TIMx timer_number);
 ErrorStatus Set_Amp_Factor_K1(uint8_t namber_value);
 ErrorStatus Set_Amp_Factor_K2(uint8_t namber_value);
 ErrorStatus Manual_settings(uint8_t namber_settings, enum PWR_TIMx timer_numberr);








#ifdef __cplusplus
}
#endif


#endif /* __CONF_A_MODULE_H */
