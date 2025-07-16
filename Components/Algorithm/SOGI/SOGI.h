#ifndef _SOGI_H
#define _SOGI_H

#include "arm_math.h"

// SOGI结构体定义
typedef struct {
    // 配置参数
    float32_t k;        // 阻尼系数 (通常取√2=1.414f)
    float32_t omega;    // 中心频率 (rad/s)
    float32_t Ts;       // 采样周期 (秒)

    // 中间计算值 (预计算以提高效率)
    float32_t Ad11, Ad12, Ad21, Ad22;
    float32_t Bd1;

    // 状态变量
    float32_t v_prev;  // 状态1前值 (v)
    float32_t qv_prev;  // 状态2前值 (qv)
    float32_t u_prev;   // 前一次输入值

    // 输出
    float32_t v;        // 同相输出
    float32_t qv;       // 正交输出
} SOGI_HandleTypeDef;

/**
  * @brief  SOGI初始化
  * @param  hsogi: SOGI句柄指针
  * @param  k: 阻尼系数
  * @param  f0: 中心频率(Hz)
  * @param  fs: 采样频率(Hz)
  * @retval None
  */
void SOGI_Init(SOGI_HandleTypeDef *hsogi, float32_t k, float32_t f0, float32_t fs);

/**
  * @brief  SOGI处理函数 (每次采样调用)
  * @param  hsogi: SOGI句柄指针
  * @param  u_k: 当前输入值
  * @retval None
  */
void SOGI_Update(SOGI_HandleTypeDef *hsogi, float32_t u_k);

#endif