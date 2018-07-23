

#ifndef IC_FN_H_
#define IC_FN_H_

#ifdef __cplusplus
 extern "C" {
#endif


 extern void _Error_Handler(char *, int);

 ErrorStatus Set_Ficlk_and_F_SAx(uint8_t par_value, enum PWR_TIMx timer_number);
 ErrorStatus Set_Amp_Factor_K1(uint8_t namber_value);
 ErrorStatus Set_Amp_Factor_K2(uint8_t namber_value);








#ifdef __cplusplus
}
#endif


#endif /* IC_FN_H_ */
