#include "SVPWM.h"

#define SQRT3 1.7320508075688772935274463415059f

float32_t duty_a, duty_b, duty_c;

/**
  * @brief  角频率发生器
  * @param  w: 角频率
  * @param  step: 执行周期（步长），单位：秒
  * @retval 随时间变化的角度
  */
float32_t FrequencyGenerator(float32_t w, float32_t step) {
    static float32_t angle = 0;
    angle = angle + w * step;
    angle = fmod(angle, 2 * PI);
    return angle >= 0 ? angle : (angle + 2 * PI);
}

/**
  * @brief  SVPWM波形发生器
  * @param  Uq：q轴电压
  * @param  Ud：d轴电压
  * @param  angle：相位角（弧度）
  * @param  Km：调制系数
  * @retval None
  */
void SVPWM(float32_t Uq, float32_t Ud, float32_t angle, float32_t Km) {
    float32_t Ualpha, Ubeta, sin_val, cos_val;
    sin_val = arm_sin_f32(angle), cos_val = arm_cos_f32(angle);
    arm_inv_park_f32(Ud, Uq, &Ualpha, &Ubeta, sin_val, cos_val);

    //扇区计算
    float32_t u_1 = Ubeta;
    float32_t u_2 = SQRT3 / 2.0f * Ualpha - Ubeta / 2.0f;
    float32_t u_3 = SQRT3 / (-2.0f) * Ualpha - Ubeta / 2.0f;
    uint8_t a, b, c;
    if (u_1 > 0.0) a = 1; else a = 0;
    if (u_2 > 0.0) b = 1; else b = 0;
    if (u_3 > 0.0) c = 1; else c = 0;



    float32_t t_1, t_2;
    switch (4 * c + 2 * b + a) {
        case 3:
            t_1 = SQRT3 / Km * u_2;
            t_2 = SQRT3 / Km * u_1;
            break;
        case 1:
            t_1 = -SQRT3 / Km * u_2;
            t_2 = -SQRT3 / Km * u_3;
            break;
        case 5:
            t_1 = SQRT3 / Km * u_1;
            t_2 = SQRT3 / Km * u_3;
            break;
        case 4:
            t_1 = -SQRT3 / Km * u_1;
            t_2 = -SQRT3 / Km * u_2;
            break;
        case 6:
            t_1 = SQRT3 / Km * u_3;
            t_2 = SQRT3 / Km * u_2;
            break;
        case 2:
            t_1 = -SQRT3 / Km * u_3;
            t_2 = -SQRT3 / Km * u_1;
            break;
    }
    //过调制保护
    if (t_1 + t_2 >= 1) {
        float32_t t = t_1 + t_2;
        t_1 = t_1 / t;
        t_2 = t_2 / t;
    }



    // 分配占空比到三个定时器
    float32_t t_com0 = (1 - t_1 - t_2) / 2;
    float32_t t_comy = t_com0 + t_2;
    float32_t t_comx = t_comy + t_1;

    switch (4 * c + 2 * b + a) {
        case 3:
            duty_a = t_comx;
            duty_b = t_comy;
            duty_c = t_com0;
            break;
        case 1:
            duty_a = t_comy;
            duty_b = t_comx;
            duty_c = t_com0;
            break;
        case 5:
            duty_a = t_com0;
            duty_b = t_comx;
            duty_c = t_comy;
            break;
        case 4:
            duty_a = t_com0;
            duty_b = t_comy;
            duty_c = t_comx;
            break;
        case 6:
            duty_a = t_comy;
            duty_b = t_com0;
            duty_c = t_comx;
            break;
        case 2:
            duty_a = t_comx;
            duty_b = t_com0;
            duty_c = t_comy;
            break;
        default:
            duty_a = 0;
            duty_b = 0;
            duty_c = 0;
            break;
    }
}
