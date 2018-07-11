

#ifndef IC_FN_H_
#define IC_FN_H_

#ifdef __cplusplus
 extern "C" {
#endif


 extern void _Error_Handler(char *, int);

 ErrorStatus Set_Ficlk(uint8_t par_value, enum PWR_TIMx timer_number);



#ifdef __cplusplus
}
#endif


#endif /* IC_FN_H_ */
