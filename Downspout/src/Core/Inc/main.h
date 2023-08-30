/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin LL_GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define Sensor_Mux0_Pin LL_GPIO_PIN_0
#define Sensor_Mux0_GPIO_Port GPIOC
#define Sensor_Mux1_Pin LL_GPIO_PIN_1
#define Sensor_Mux1_GPIO_Port GPIOC
#define Sensor_Mux2_Pin LL_GPIO_PIN_2
#define Sensor_Mux2_GPIO_Port GPIOC
#define LCD_RS_Pin LL_GPIO_PIN_1
#define LCD_RS_GPIO_Port GPIOA
#define LCD_E1_Pin LL_GPIO_PIN_4
#define LCD_E1_GPIO_Port GPIOA
#define LD2_Pin LL_GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LCD_D7_Pin LL_GPIO_PIN_4
#define LCD_D7_GPIO_Port GPIOC
#define LCD_Backlight_Pin LL_GPIO_PIN_5
#define LCD_Backlight_GPIO_Port GPIOC
#define LCD_D4_Pin LL_GPIO_PIN_2
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin LL_GPIO_PIN_10
#define LCD_D5_GPIO_Port GPIOB
#define Motor_L1_Pin LL_GPIO_PIN_12
#define Motor_L1_GPIO_Port GPIOB
#define Motor_L3_Pin LL_GPIO_PIN_13
#define Motor_L3_GPIO_Port GPIOB
#define Motor_L4_Pin LL_GPIO_PIN_14
#define Motor_L4_GPIO_Port GPIOB
#define LCD_D6_Pin LL_GPIO_PIN_15
#define LCD_D6_GPIO_Port GPIOB
#define TMS_Pin LL_GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin LL_GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Motor_R1_Pin LL_GPIO_PIN_10
#define Motor_R1_GPIO_Port GPIOC
#define Motor_R3_Pin LL_GPIO_PIN_11
#define Motor_R3_GPIO_Port GPIOC
#define Motor_R4_Pin LL_GPIO_PIN_12
#define Motor_R4_GPIO_Port GPIOC
#define SWO_Pin LL_GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SWL_Pin LL_GPIO_PIN_4
#define SWL_GPIO_Port GPIOB
#define SWR_Pin LL_GPIO_PIN_5
#define SWR_GPIO_Port GPIOB
#define Motor_L2_Pin LL_GPIO_PIN_8
#define Motor_L2_GPIO_Port GPIOB
#define Motor_R2_Pin LL_GPIO_PIN_9
#define Motor_R2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

extern TIM_HandleTypeDef htim1;

extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart2;

extern uint8_t Distance;
extern uint8_t Distance2;
void HCSR04_Read();
void HCSR04_Read2();

