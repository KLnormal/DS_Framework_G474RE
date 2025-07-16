#ifndef _SVPWM_H
#define _SVPWM_H


#include "main.h"
#include "arm_math.h"


extern float32_t duty_a, duty_b, duty_c;


/**
  * @brief  SVPWM初始化
  * @param  Vdc: 直流电压
  * @retval None
  */
void SVPWM_Init(float32_t Vdc);

/**
  * @brief  角频率发生器
  * @param  w: 角频率
  * @param  step: 执行周期（步长），单位：秒
  * @retval 随时间变化的角度
  */
float32_t FrequencyGenerator(float32_t w, float32_t step);

/**
  * @brief  SVPWM波形发生器
  * @param  Uq：q轴电压
  * @param  Ud：d轴电压
  * @param  angle：相位角（弧度）
  * @param  Km：调制系数
  * @retval None
  */
void SVPWM(float32_t Uq, float32_t Ud, float32_t angle, float32_t Km);


#endif
