#ifndef __MOTOR_H
#define __MOTOR_H

void MotorA_Init(void);
void MotorA_SetPWM(int8_t PWM);
void MotorB_Init(void);
void MotorB_SetPWM(int8_t PWM);
float PID_A(float targetA);
float PID_B(float targetB);
int angle(float Angle,float Gyroy,float Mechanical_Angle);

//AIN1:PC14
//AIN2:PC15
//BIN1:PA4
//BIN2:PA5

#endif
