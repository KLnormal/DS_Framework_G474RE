#include "arm_math.h"
#include "SOGI.h"

/**
  * @brief  SOGI初始化
  * @param  hsogi: SOGI句柄指针
  * @param  k: 阻尼系数
  * @param  f0: 中心频率(Hz)
  * @param  fs: 采样频率(Hz)
  * @retval None
  */
void SOGI_Init(SOGI_HandleTypeDef *hsogi, float32_t k, float32_t f0, float32_t fs) {
    // 设置基本参数
    hsogi->k = k;
    hsogi->omega = 2.0f * PI * f0;  // 角频率 (rad/s)
    hsogi->Ts = 1.0f / fs;         // 采样周期

    // 预计算中间常量
    const float32_t w = hsogi->omega;
    const float32_t k_w = k * w;
    const float32_t w2 = w * w;

    // 计算行列式
    const float32_t det = 1.0f + k_w / 2 / fs + w2 / 4 / fs / fs;

    // 计算离散状态矩阵Ad
    hsogi->Ad11 = (1 - k_w / 2 / fs - w2 / 4 / fs / fs) / det;
    hsogi->Ad12 = w / det / fs;
    hsogi->Ad21 = -w / det / fs;
    hsogi->Ad22 = (1 + k_w / 2 / fs - w2 / 4 / fs / fs) / det;

    // 计算输入矩阵Bd
    hsogi->Bd1 = (k_w / 2 / fs) / det;  // Bd2 = 0

    // 初始化状态变量
    hsogi->v_prev = 0.0f;
    hsogi->qv_prev = 0.0f;
    hsogi->u_prev = 0.0f;

    // 初始化输出
    hsogi->v = 0.0f;
    hsogi->qv = 0.0f;
}

/**
  * @brief  SOGI处理函数 (每次采样调用)
  * @param  hsogi: SOGI句柄指针
  * @param  u_k: 当前输入值
  * @retval None
  */
void SOGI_Update(SOGI_HandleTypeDef *hsogi, float32_t u_k) {
    // 计算输入和 (u_k + u_{k-1})
    const float32_t u_sum = u_k + hsogi->u_prev;

    // 状态更新方程
    const float32_t x1 = hsogi->Ad11 * hsogi->v_prev +
                     hsogi->Ad12 * hsogi->qv_prev +
                     hsogi->Bd1 * u_sum;

    const float32_t x2 = hsogi->Ad21 * hsogi->v_prev +
                     hsogi->Ad22 * hsogi->qv_prev;  // Bd2 = 0

    // 更新输出
    hsogi->v = x1;
    hsogi->qv = x2;

    // 保存状态用于下一次迭代
    hsogi->v_prev = x1;
    hsogi->qv_prev = x2;
    hsogi->u_prev = u_k;
}

//// 使用示例
//int main(void) {
//    // HAL初始化等代码...
//
//    // 创建SOGI实例
//    SOGI_HandleTypeDef hsogi;
//
//    // 配置参数
//    const float32_t k = 1.414f;      // 阻尼系数 √2
//    const float32_t f0 = 50.0f;      // 中心频率50Hz
//    const float32_t fs = 10000.0f;   // 采样频率10kHz
//
//    // 初始化SOGI
//    SOGI_Init(&hsogi, k, f0, fs);
//
//    while (1) {
//        // 从ADC获取新采样值 (示例)
//        float32_t adc_value = read_ADC();
//
//        // 更新SOGI
//        SOGI_Update(&hsogi, adc_value);
//
//        // 获取输出
//        float32_t v_out = hsogi.v;    // 同相分量
//        float32_t qv_out = hsogi.qv;  // 正交分量
//
//        // 使用输出值...
//        // (例如: 锁相环、谐波检测等)
//
//        // 等待下一个采样周期
//        HAL_Delay(1);  // 实际使用定时器中断更精确
//    }
//}