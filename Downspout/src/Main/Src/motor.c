#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "motor.h"
#include "custom_delay.h"
#include "custom_gpio.h"
#include "custom_lcd.h"
#include "custom_switch.h"

volatile uint32_t motorTickL = 0;
volatile uint32_t motorTickR = 0;


Custom_GPIO_t motorL[4] = {
		{ Motor_L1_GPIO_Port, Motor_L1_Pin },
		{ Motor_L3_GPIO_Port, Motor_L3_Pin },
		{ Motor_L2_GPIO_Port, Motor_L2_Pin },
		{ Motor_L4_GPIO_Port, Motor_L4_Pin },
};

Custom_GPIO_t motorR[4] = {
		{ Motor_R1_GPIO_Port, Motor_R1_Pin },
		{ Motor_R3_GPIO_Port, Motor_R3_Pin },
		{ Motor_R2_GPIO_Port, Motor_R2_Pin },
		{ Motor_R4_GPIO_Port, Motor_R4_Pin },
};

volatile static uint8_t phases[8] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08,
		0x09 };

void Motor_Start() {
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM3);

	LL_TIM_EnableCounter(TIM4);
	LL_TIM_EnableIT_UPDATE(TIM4);
}

void Motor_Stop() {
	LL_TIM_DisableIT_UPDATE(TIM3);
	LL_TIM_DisableCounter(TIM3);

	LL_TIM_DisableIT_UPDATE(TIM4);
	LL_TIM_DisableCounter(TIM4);

	for (int i = 0; i < 4; i++) {
		Custom_GPIO_Set_t(motorL + i, 0);
		Custom_GPIO_Set_t(motorR + i, 0);
	}
}

void Motor_L_Start() {
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM3);
}

void Motor_R_Start() {
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_EnableIT_UPDATE(TIM4);
}

void Motor_L_Stop() {
	LL_TIM_DisableIT_UPDATE(TIM3);
	LL_TIM_DisableCounter(TIM3);

	for (int i = 0; i < 4; i++) {
		Custom_GPIO_Set_t(motorL + i, 0);
	}
}
void Motor_R_Stop() {
	LL_TIM_DisableIT_UPDATE(TIM4);
	LL_TIM_DisableCounter(TIM4);

	for (int i = 0; i < 4; i++) {
		Custom_GPIO_Set_t(motorR + i, 0);
	}
}

volatile int deltaTickL = 500;
volatile int deltaTickR = 500;
volatile int directionTickL = 1;
volatile int directionTickR = 1;

void Motor_L_TIM3_IRQ() {
	static uint8_t index = 0;
	static uint8_t phase;
	phase = phases[index];
	Custom_GPIO_Set_t(motorL + 0, phase & 0x01);
	Custom_GPIO_Set_t(motorL + 1, phase & 0x02);
	Custom_GPIO_Set_t(motorL + 2, phase & 0x04);
	Custom_GPIO_Set_t(motorL + 3, phase & 0x08);

	if (deltaTickL >= 500) {
		return;
	}

	index += directionTickL;
	index &= 0x07;
	deltaTickL++;
}

void Motor_R_TIM4_IRQ() {
	static uint8_t index = 0;
	static uint8_t phase;
	phase = phases[7 - index];
	Custom_GPIO_Set_t(motorR + 0, phase & 0x01);
	Custom_GPIO_Set_t(motorR + 1, phase & 0x02);
	Custom_GPIO_Set_t(motorR + 2, phase & 0x04);
	Custom_GPIO_Set_t(motorR + 3, phase & 0x08);

	if (deltaTickR >= 500) {
		return;
	}

	index += directionTickR;
	index &= 0x07;
	deltaTickR++;
}

__STATIC_INLINE uint32_t Motor_Calc_ARR(float velo) {
//unsigned short 최대값
	if (velo < 0.01) return 65535;
//타이머 클럭 = 1MHz , 400 step 1바퀴
//지름이 5cm바퀴 기준
//1초에 n바퀴 ARR = (타이머 클럭) / n * (step)
//n m/s ARR = (타이머 클럭) * (5.3(CM)) / 100(CM) * (step) * n
	else return 416.26102692 / velo;
}

void Motor_Set_Speed(float VL, float VR) {
	LL_TIM_SetAutoReload(TIM3, Motor_Calc_ARR(VL));
	LL_TIM_SetAutoReload(TIM4, Motor_Calc_ARR(VR));
}


//우수받이 테스트
void Downspout() {
	Custom_LCD_Clear();
	uint8_t switchLeft;
	uint8_t mode = 4;
	uint8_t preMode = 2;
	int32_t velostat = 0;
	uint8_t sel_ultrasonic = 1;
	uint8_t max_trash = 25;
	Motor_Set_Speed(0.1f, 0.1f);
	LL_ADC_Enable(ADC1);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);

	for (;;) {

		LL_ADC_REG_StartConversionSWStart(ADC1);
		while (!LL_ADC_IsActiveFlag_EOCS(ADC1));
		LL_ADC_ClearFlag_EOCS(ADC1);
		Custom_LCD_Clear();
		velostat = LL_ADC_REG_ReadConversionData12(ADC1);
		Custom_LCD_Printf("/0S:%d", velostat);
		if(sel_ultrasonic == 1 ){
			HCSR04_Read(); // L초음파
			sel_ultrasonic = 2;
		}else if(sel_ultrasonic == 2){
			HCSR04_Read2(); // R초음파
			sel_ultrasonic = 1;
		}
		if(Distance < max_trash && Distance2 < max_trash) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); // 수거함이 꽉참
		else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); // 수거함이 꽉차지 않음
		Custom_LCD_Printf("/1L:%dR:%d", Distance, Distance2); // 초음파 센서값 출력
		HAL_Delay(100);

		switchLeft = Custom_Switch_Read();
		if ((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8)) && (mode == 4 && preMode != 1)
				&& velostat < 30) mode = 1; //비가 옴
		if ((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0)
				&& (mode == 4 && preMode != 2)) mode = 2; // 비가 그침

		if (mode == 1) {
			directionTickL = 1;
			deltaTickL = 0;
			preMode = mode;
		}
		else if (mode == 2) {
			directionTickL = -1;
			deltaTickL = 0;
			preMode = mode;
		}

		if (500 - deltaTickL) {
			Motor_L_Start();
			mode = 3;
		}
		else {
			Motor_L_Stop();
			mode = 4;
		}

		if (switchLeft == CUSTOM_SW_BOTH) break;
	}
}
