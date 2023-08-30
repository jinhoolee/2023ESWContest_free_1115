#include "custom_switch.h"

#define LONG_OFF	0x01
#define SHORT_ON	0x02
#define LONG_ON		0x04
#define SHORT_OFF	0x08

#define TIME_SHROT	80	// Unit : millisecond
#define TIME_LONG	500 // Unit : millisecond

typedef struct {
	GPIO_TypeDef *port;	// Port of switch
	uint32_t pinMask;	// Pin of switch
	uint32_t timer;		//
	uint8_t state;		//
} ButtonState_t;

static void Custom_Switch_Init_ButtonState(ButtonState_t *State,
		GPIO_TypeDef *GPIOx, uint32_t PinMask) {
	State->port = GPIOx;
	State->pinMask = PinMask;
	State->timer = 0;
	State->state = LONG_OFF;
}

static uint8_t Custom_Switch_State_Machine(ButtonState_t *State) {
	/**
	 * 이 함수는 State 구조체의 값을 기반으로 아래 설명된 바와 같이 state machine을 구현한다.
	 * 이 함수는 1ms마다 호출됨을 전제로 한다.
	 */
	bool currentPushed = !(LL_GPIO_ReadInputPort(State->port) & State->pinMask);
	bool pushEvent = false;

	switch (State->state) {

		case LONG_OFF:
			if (currentPushed) {
				State->state = SHORT_ON;
				State->timer = TIME_SHROT;
			}
			break;

		case SHORT_ON:
			State->timer--;
			if (State->timer == 0) {
				pushEvent = true;
				State->state = LONG_ON;
				State->timer = TIME_LONG;
			}
			break;

		case LONG_ON:
			if (!currentPushed) {
				State->state = SHORT_OFF;
				State->timer = TIME_SHROT;
				break;
			}
			State->timer--;
			if (State->timer == 0) {
				pushEvent = true;
				State->timer = TIME_LONG;
			}
			break;

		case SHORT_OFF:
			State->timer--;
			if (State->timer == 0) {
				State->state = LONG_OFF;
			}
			break;
	}

	return pushEvent;
}

uint8_t Custom_Switch_Read(void) {

	static bool isInitialized = false;
	static ButtonState_t sw1, sw2;
	if (!isInitialized) {
		isInitialized = true;
		Custom_Switch_Init_ButtonState(&sw1, SW1_PORT, SW1_PIN);
		Custom_Switch_Init_ButtonState(&sw2, SW2_PORT, SW2_PIN);
	}

	Custom_Delay_ms(1);
	uint8_t sw1PushEvent = Custom_Switch_State_Machine(&sw1);
	uint8_t sw2PushEvent = Custom_Switch_State_Machine(&sw2);

	/*
	 * 두 버튼이 동시에 눌리는 것을 처리하기 위해, 한 버튼의 푸시 이벤트가 발생했고 다른 버튼이 Short-On 상태에 있다면 두 버튼이 동시에 눌린 것으로 간주한다.
	 */
	if ((sw1.state & SHORT_ON) && sw2PushEvent) {
		sw1PushEvent = true;
		sw1.state = sw2.state;
		sw1.timer = sw2.timer;
	}
	if ((sw2.state & SHORT_ON) && sw1PushEvent) {
		sw2PushEvent = true;
		sw2.state = sw1.state;
		sw2.timer = sw1.timer;
	}

	uint8_t buttonPushEvent = 0;
	if (sw1PushEvent) buttonPushEvent |= CUSTOM_SW_1;
	if (sw2PushEvent) buttonPushEvent |= CUSTOM_SW_2;

	return buttonPushEvent;
}

uint8_t Custom_Switch_Wait_Read(uint32_t millisecond) {
	for (int i = 0; i < millisecond; i++) {
		uint8_t swState = Custom_Switch_Read();
		if (swState) return swState;
	}
	return 0;
}
