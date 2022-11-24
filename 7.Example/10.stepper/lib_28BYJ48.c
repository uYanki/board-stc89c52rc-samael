#include "lib_28BYJ48.h"

// 四相单四拍 A->B->C->D
void stepMotor_414(uint8_t step) {
    switch (step) {
        case 1:  // A
            MOTOR_A_HIGH;
            MOTOR_B_LOW;
            MOTOR_C_LOW;
            MOTOR_D_LOW;
            break;
        case 2:  // B
            MOTOR_A_LOW;
            MOTOR_B_HIGH;
            MOTOR_C_LOW;
            MOTOR_D_LOW;
            break;
        case 3:  // C
            MOTOR_A_LOW;
            MOTOR_B_LOW;
            MOTOR_C_HIGH;
            MOTOR_D_LOW;
            break;
        case 4:  // D
            MOTOR_A_LOW;
            MOTOR_B_LOW;
            MOTOR_C_LOW;
            MOTOR_D_HIGH;
            break;
    }
}

// 四相双四拍 AB->BC->CD->DA
void stepMotor_424(uint8_t step) {
    switch (step) {
        case 1:  // AB
            MOTOR_A_HIGH;
            MOTOR_B_HIGH;
            MOTOR_C_LOW;
            MOTOR_D_LOW;
            break;
        case 2:  // BC
            MOTOR_A_LOW;
            MOTOR_B_HIGH;
            MOTOR_C_HIGH;
            MOTOR_D_LOW;
            break;
        case 3:  // CD
            MOTOR_A_LOW;
            MOTOR_B_LOW;
            MOTOR_C_HIGH;
            MOTOR_D_HIGH;
            break;
        case 4:  // DA
            MOTOR_A_HIGH;
            MOTOR_B_LOW;
            MOTOR_C_LOW;
            MOTOR_D_HIGH;
            break;
    }
}

// 四相八拍:A->AB->B->BC->C->CD->D->DA
void stepMotor_48(uint8_t step) {
    switch (step) {
        case 1:  // A
            MOTOR_A_HIGH;
            MOTOR_B_LOW;
            MOTOR_C_LOW;
            MOTOR_D_LOW;
            break;
        case 2:  // AB
            MOTOR_A_HIGH;
            MOTOR_B_HIGH;
            MOTOR_C_LOW;
            MOTOR_D_LOW;
            break;
        case 3:  // B
            MOTOR_A_LOW;
            MOTOR_B_HIGH;
            MOTOR_C_LOW;
            MOTOR_D_LOW;
            break;
        case 4:  // BC
            MOTOR_A_LOW;
            MOTOR_B_HIGH;
            MOTOR_C_HIGH;
            MOTOR_D_LOW;
            break;
        case 5:  // C
            MOTOR_A_LOW;
            MOTOR_B_LOW;
            MOTOR_C_HIGH;
            MOTOR_D_LOW;
            break;
        case 6:  // CD
            MOTOR_A_LOW;
            MOTOR_B_LOW;
            MOTOR_C_HIGH;
            MOTOR_D_HIGH;
            break;
        case 7:  // D
            MOTOR_A_LOW;
            MOTOR_B_LOW;
            MOTOR_C_LOW;
            MOTOR_D_HIGH;
            break;
        case 8:  // DA
            MOTOR_A_HIGH;
            MOTOR_B_LOW;
            MOTOR_C_LOW;
            MOTOR_D_HIGH;
            break;
    }
}

void stepMotor_stop() {
    MOTOR_A_LOW;
    MOTOR_B_LOW;
    MOTOR_C_LOW;
    MOTOR_D_LOW;
}

void stepMotor_run(uint8_t mode /* 1:四相单四拍,2:四相双四拍 */,
                   uint8_t dir /* 0:反向 1:正向 */,
                   uint8_t speed /* 速度,值越大就越慢 */) {
    uint8_t i = 1;
    switch (mode) {
        case 1:  // 四相八拍
            if (dir) {
                // dir = 1: 正向
                for (i = 1; i < 9; ++i) {
                    stepMotor_48(i);
                    delay_ms(speed);
                }
            } else {
                // dir = 0: 反向
                for (i = 8; i > 0; --i) {
                    stepMotor_48(i);
                    delay_ms(speed);
                }
            }
            break;
        case 2:  // 四相单四拍
            if (dir) {
                // dir = 1: 正向
                for (i = 1; i < 5; ++i) {
                    stepMotor_414(i);
                    delay_ms(speed);
                }
            } else {
                // dir = 0: 反向
                for (i = 4; i > 0; --i) {
                    stepMotor_414(i);
                    delay_ms(speed);
                }
            }
            break;
        case 3:  // 四相双四拍
            if (dir) {
                // dir = 1: 正向
                for (i = 1; i < 5; ++i) {
                    stepMotor_424(i);
                    delay_ms(speed);
                }
            } else {
                // dir = 0: 反向
                for (i = 4; i > 0; --i) {
                    stepMotor_424(i);
                    delay_ms(speed);
                }
            }
            break;
    }
}
