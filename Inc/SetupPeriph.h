/*
 * Setup peripheral of MC STM32L452
 */

#ifndef SETUPPERIPH_H_
#define SETUPPERIPH_H_

enum PWR_TIMx{
  PWM_TIM2_CH2_PA1,
  PWM_TIM3_CH1_PA6
};

void LL_Init(void);
void SystemClock_Config(void);
void SetupLED(void);
void SetupGPIO(void);
void USART1_Init(void);
void SPI2_Init(void);
void I2C1_Init(void);
void PWM_Init(enum PWR_TIMx timer_number);
void SetupInterrupt(void);
void MX_IWDG_Init(void);

/*The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_4  |                 0-15              |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority
  ===========================================================================================================================*/

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/*For ON/OFF LED need to use:                                        */
#define LED_Yellow_HL1_ON()  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10)
#define LED_Yellow_HL1_OFF() LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10)

#define LED_Green_HL2_ON()   LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_11)
#define LED_Green_HL2_OFF()  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_11)

#define LED_Green_HL3_ON()   LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12)
#define LED_Green_HL3_OFF()  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12)


#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* SETUPPERIPH_H_ */
