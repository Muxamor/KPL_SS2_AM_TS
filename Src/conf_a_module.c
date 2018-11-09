/*
 * IC_fn.c
 *
 *  Created on: Jul 10, 2018
 *      Author: muxamor
 */


#include "stm32l4xx.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"
#include "SetupPeriph.h"
#include "conf_a_module.h"

#include  <stdio.h>

/**
  * @brief  Setup Ficlk and F_SAx settings cutoff frequency of low-pass filter
  * @param  Parametr value
  * @param  PWM_TIM2_CH2_PA1 or PWM_TIM3_CH1_PA6
  * @retval An ErrorStatus enumeration
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
ErrorStatus Set_Ficlk_and_F_SAx(uint8_t par_value, enum PWR_TIMx timer_number){

	uint32_t Counter_TIM_Value, Fcut, Ficlk;
	//Fcut -  cutoff frequency of low-pass filter (Hz)
	//Ficlkt -  real value of the frequency fed to the low-pass filter (Hz)

	Fcut=par_value*10;// cutoff frequency of low-pass filter (Hz)

	if(Fcut == 10){
		Ficlk = 1300;//Hz
		
	}else if(Fcut >= 1270){
		Ficlk = 130000;//Hz

	}else{
		//Ficlk = ( ((Fcut%50)==0) ? ((Fcut<1000) ? (Fcut=Fcut+5) : (Fcut=Fcut+50)) : Fcut )*100;
		Ficlk = ( ((Fcut%50)==0) ? (Fcut=Fcut+5): Fcut )*100;
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
		Error_Handler();
		return ERROR;
	}

    // Control F_SAx
	if( Fcut==10 /*|| Fcut==20 */){
		F_SA0_Reset();
		F_SA1_Reset();
#ifdef DEBUGprintf
			printf("Param_value=%d  Fcut=%luHz  Ficlk=%luHz  F_SA0=0 F_SA1=0\r\n",par_value, (unsigned long)Fcut, (unsigned long)Ficlk );
#endif

	}else if ( Fcut >= 20 && Fcut < 330 ){
		F_SA0_Set();
		F_SA1_Reset();
#ifdef DEBUGprintf
			printf("Param_value=%d  Fcut=%luHz  Ficlk=%luHz  F_SA0=1 F_SA1=0\r\n",par_value, (unsigned long)Fcut, (unsigned long)Ficlk );
#endif

	}else if( Fcut >= 330 ){
		F_SA0_Set();
		F_SA1_Set();
#ifdef DEBUGprintf
			printf("Param_value=%d  Fcut=%luHz  Ficlk=%luHz  F_SA0=1 F_SA1=1\r\n",par_value, (unsigned long)Fcut, (unsigned long)Ficlk );
#endif

	}

	return SUCCESS;
}

/**
  * @brief  Setup amplification factor K1
  * @param  namber_value
  * @retval An ErrorStatus enumeration
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
ErrorStatus Set_Amp_Factor_K1(uint8_t namber_value){

	uint16_t K1=0;

	switch(namber_value){
		case 0: // Set K1=1
			PA3_10_Set();
			PA4_100_Set();
			PA5_1000_Set();
			K1=1;
			break;
		case 1:// Set K1=10
			PA3_10_Reset();
			PA4_100_Set();
			PA5_1000_Set();
			K1=10;
			break;
		case 2:// Set K1=100
			PA3_10_Set();
			PA4_100_Reset();
			PA5_1000_Set();
			K1=100;
			break;
		case 3:// Set K1=1000
			PA3_10_Set();
			PA4_100_Set();
			PA5_1000_Reset();
			K1=1000;
			break;
		default:
			Error_Handler();
			return ERROR;
	}

#ifdef DEBUGprintf
	printf("K1 = %d \r\n",K1);
#endif

	return SUCCESS;
}

/**
  * @brief  Setup amplification factor K2
  * @param  namber_value
  * @retval An ErrorStatus enumeration
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
ErrorStatus Set_Amp_Factor_K2(uint8_t namber_value){

	uint16_t K2=0;
	
	switch(namber_value){

		case 0: // Set K2=1
			PB0_G0_Reset();
			PB1_G1_Set();
			PB2_G2_Reset();
			PB10_G3_Reset();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=1;
			break;
		case 1: // Set K2=2
			PB0_G0_Set();
			PB1_G1_Set();
			PB2_G2_Reset();
			PB10_G3_Reset();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=2;
			break;
		case 2: // Set K2=4
			PB0_G0_Reset();
			PB1_G1_Reset();
			PB2_G2_Set();
			PB10_G3_Reset();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=4;
			break;
		case 3: // Set K2=8
			PB0_G0_Set();
			PB1_G1_Reset();
			PB2_G2_Set();
			PB10_G3_Reset();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=8;
			break;
		case 4: // Set K2=16
			PB0_G0_Reset();
			PB1_G1_Set();
			PB2_G2_Set();
			PB10_G3_Reset();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=16;
			break;
		case 5: // Set K2=32
			PB0_G0_Set();
			PB1_G1_Set();
			PB2_G2_Set();
			PB10_G3_Reset();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=32;
			break;
		case 6: // Set K2=64
			PB0_G0_Reset();
			PB1_G1_Reset();
			PB2_G2_Reset();
			PB10_G3_Set();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=64;
			break;
		case 7: // Set K2=128
			PB0_G0_Set();
			PB1_G1_Reset();
			PB2_G2_Reset();
			PB10_G3_Set();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=128;
			break;
		case 8: // Set K2=256
			PB0_G0_Reset();
			PB1_G1_Set();
			PB2_G2_Reset();
			PB10_G3_Set();
			PB11_G4_Reset();
			F_SA2_Reset();
			K2=256;
			break;
		case 9: // Set K2=512
			PB0_G0_Reset();
			PB1_G1_Set();
			PB2_G2_Reset();
			PB10_G3_Set();
			PB11_G4_Reset();
			F_SA2_Set();
			K2=512;
			break;
		case 10: // Set K2=1024
			PB0_G0_Reset();
			PB1_G1_Set();
			PB2_G2_Reset();
			PB10_G3_Set();
			PB11_G4_Reset();
			F_SA2_Set();
			K2=1024;
			break;
		case 11: // Set K2=2048
			PB0_G0_Reset();
			PB1_G1_Set();
			PB2_G2_Reset();
			PB10_G3_Set();
			PB11_G4_Reset();
			F_SA2_Set();
			K2=2048;
			break;
		default:
			Error_Handler();
			return ERROR;
	}

#ifdef DEBUGprintf
	printf("K2 = %d \r\n",K2);
#endif

	return SUCCESS;
}


/**
  * @brief  Manual setup module through configuration board.
  * @param  number_settings
  * @retval An ErrorStatus enumeration
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
 */
ErrorStatus Manual_settings(uint8_t namber_settings, enum PWR_TIMx timer_numberr){

	uint8_t namber_settings_K1_k2,namber_settings_Fcut;

	namber_settings_K1_k2 = 0x7 & namber_settings;
	namber_settings_Fcut = namber_settings >> 3;

#ifdef DEBUGprintf
	printf("\r\nManual configuration of the analog module:\r\n");
#endif

	switch (namber_settings_K1_k2){

		case 0: //K1=1 K2=1 
			Set_Amp_Factor_K1(0);
			Set_Amp_Factor_K2(0);
			break;

		case 1: //K1=10 K2=1 
			Set_Amp_Factor_K1(1);
			Set_Amp_Factor_K2(0);
			break;

		case 2: //K1=10 K2=2
			Set_Amp_Factor_K1(1);
			Set_Amp_Factor_K2(1);
			break;

		case 3: //K1=10 K2=4
			Set_Amp_Factor_K1(1);
			Set_Amp_Factor_K2(2);
			break;

		case 4: //K1=100 K2=1
			Set_Amp_Factor_K1(2);
			Set_Amp_Factor_K2(0);
			break;

		case 5: //K1=100 K2=2
			Set_Amp_Factor_K1(2);
			Set_Amp_Factor_K2(1);
			break;

		case 6: //K1=100 K2=4
			Set_Amp_Factor_K1(2);
			Set_Amp_Factor_K2(2);
			break;

		case 7: //K1=1000 K2=2048
			Set_Amp_Factor_K1(3);
			Set_Amp_Factor_K2(11);
			break;

		default:
			Error_Handler();
			return ERROR;
	}

	switch(namber_settings_Fcut){

		case 0: //Fcut=10
			Set_Ficlk_and_F_SAx(1, timer_numberr);
			break;

		case 1: //Fcut=300
			Set_Ficlk_and_F_SAx(30, timer_numberr);
			break;

		case 2: //Fcut=700
			Set_Ficlk_and_F_SAx(70, timer_numberr);
			break;

		case 3: ////Fcut=20
			Set_Ficlk_and_F_SAx(2, timer_numberr);
			break;

		default:
			Error_Handler();
			return ERROR;
	}

	return SUCCESS;
}

