#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

extern volatile uint32_t motorTickL;
extern volatile uint32_t motorTickR;

void Motor_Start();
void Motor_Stop();

void Motor_L_TIM3_IRQ();
void Motor_R_TIM4_IRQ();
void Motor_Set_Speed(float VL, float VR);

void Downspout();

#endif /* INC_MOTOR_H_ */
