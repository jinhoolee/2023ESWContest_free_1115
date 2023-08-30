#ifndef INC_CUSTOM_GPIO_H_
#define INC_CUSTOM_GPIO_H_

#include "stm32f4xx.h"

typedef volatile struct {
	GPIO_TypeDef *port;
	uint32_t pinMask;
} Custom_GPIO_t;

static inline void Custom_GPIO_Set(GPIO_TypeDef *GPIOx, uint32_t PinMask,
		uint32_t value) {

	GPIOx->ODR = (GPIOx->ODR & ~PinMask)
			| (PinMask & (((!!value) << 31) >> 31));

#if 0

	/*
	 * 아래 구문은 위와 동일한 역할을 하지만, 파이프라이닝 분기 예측 실패 시 속도가 조금 더 느려질 수 있다.
	 * 사실 그 속도 차이가 크게 유의미하지는 않을 수 있다만...
	 */

	if(value){
		LL_GPIO_SetOutputPin(GPIOx, PinMask);
	}else{
		LL_GPIO_ResetOutputPin(GPIOx, PinMask);
	}
#endif
}

static inline void Custom_GPIO_Set_t(Custom_GPIO_t *Pin, uint32_t value) {
	Pin->port->ODR = (Pin->port->ODR & ~Pin->pinMask)
			| (Pin->pinMask & (((!!value) << 31) >> 31));
}

#endif /* INC_CUSTOM_GPIO_H_ */
