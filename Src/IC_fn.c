/*
 * IC_fn.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */


#include "stm32l4xx.h"
#include "SetupPeriph.h"
#include "IC_fn.h"
#include "stm32l4xx_ll_tim.h"

#include  <stdio.h>


ErrorStatus Set_Ficlk(uint8_t par_value, enum PWR_TIMx timer_number){

	uint32_t Counter_TIM_Value, Fcut, Ficlk;
	//Fcut -  cutoff frequency of low-pass filter (Hz)
	//Ficlkt -  real value of the frequency fed to the low-pass filter (Hz)


	Fcut=par_value*10;// cutoff frequency of low-pass filter (Hz)

	if(Fcut==10){
		Ficlk=1300;//Hz
	}else if(Fcut>=1270){
		Ficlk=130000;//Hz
	}else{
		Ficlk = ( ((Fcut%50)==0) ? ((Fcut<1000) ? (Fcut=Fcut+5) : (Fcut=Fcut+50)) : Fcut )*100;
	}

	Counter_TIM_Value = SystemCoreClock/Ficlk;

	if(timer_number == PWM_TIM2_CH2_PA1){
		LL_TIM_DisableCounter(TIM2); // Disable timer for resetup
		LL_TIM_SetCounter(TIM2, 0);
		LL_TIM_SetAutoReload(TIM2, Counter_TIM_Value-1);
		LL_TIM_OC_SetCompareCH2(TIM2, ((Counter_TIM_Value/2)-1));
		//LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH2);
		LL_TIM_EnableCounter(TIM2);
	}else if(timer_number == PWM_TIM3_CH1_PA6){
		LL_TIM_DisableCounter(TIM3); // Disable timer for resetup
		LL_TIM_SetCounter(TIM3, 0);
		LL_TIM_SetAutoReload(TIM3, Counter_TIM_Value-1);
		LL_TIM_OC_SetCompareCH1(TIM3, ((Counter_TIM_Value/2)-1));
		//LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH2);
		LL_TIM_EnableCounter(TIM3);
	}else{
		return ERROR;
		Error_Handler();
	}

#ifdef DEBUGprintf
	printf("Param_value=%d Fcut=%lu Ficlk=%lu \r\n",par_value, (unsigned long)Fcut, (unsigned long)Ficlk );
#endif

	return SUCCESS;
}


