#ifndef CUSTOM_DELAY_CUSTOM_DELAY_H_
#define CUSTOM_DELAY_CUSTOM_DELAY_H_

#include <stdint.h>
#include "stm32f4xx_ll_tim.h"

#define CUSTOM_DELAY_TIM TIM2

__STATIC_INLINE void Custom_Delay_us(uint32_t microsecond) {
	CUSTOM_DELAY_TIM->ARR = microsecond - 1;
	CUSTOM_DELAY_TIM->SR &= ~(0x0001); // Clear UEV flag
	CUSTOM_DELAY_TIM->CR1 |= 1UL;
	while ((CUSTOM_DELAY_TIM->SR & 0x0001) != 1)
		;
}

__STATIC_INLINE void Custom_Delay_ms(uint32_t millisecond) {
	Custom_Delay_us(millisecond * 1000);
}

#endif /* CUSTOM_DELAY_CUSTOM_DELAY_H_ */
