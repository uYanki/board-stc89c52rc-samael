
#ifndef __LIB_28BYJ48__
#define __LIB_28BYJ48__

#include <REGX52.H>
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

extern void delay_ms(uint16_t ms);

// 红线-> 5V

/*

步进电机:反应式步进电机(VR),永磁式步进电机(PM),混合式步进电机(HB)
28BYJ48:28指直径28毫米,B指步进电机,Y指永磁,J指带减速箱,48指可四拍或八拍允许.
保持转矩:电机通电但没转动时,定子锁住转子的力矩.通常步进电机在低速时的力矩接近保持转矩.

https://blog.csdn.net/anchoretor/article/details/113780470

*/

#define MOTOR_A P1_0
#define MOTOR_B P1_1
#define MOTOR_C P1_2
#define MOTOR_D P1_3

#define MOTOR_A_HIGH MOTOR_A = 0
#define MOTOR_A_LOW MOTOR_A = 1
#define MOTOR_B_HIGH MOTOR_B = 0
#define MOTOR_B_LOW MOTOR_B = 1
#define MOTOR_C_HIGH MOTOR_C = 0
#define MOTOR_C_LOW MOTOR_C = 1
#define MOTOR_D_HIGH MOTOR_D = 0
#define MOTOR_D_LOW MOTOR_D = 1

// void stepMotor_414(uint8_t step);
// void stepMotor_424(uint8_t step);
// void stepMotor_48(uint8_t step);

void stepMotor_stop();

// 转1圈
void stepMotor_run(uint8_t mode /* 1:四向八拍,2:四相单四拍,3:四相双四拍 */,
                   uint8_t dir /* 0:反向 1:正向 */,
                   uint8_t speed /* 速度,值越大就越慢 */);

#endif