# DS_Framework_G474RE

> [!NOTE]
> - 该项目依赖STM32G474RE主控芯片
> - 该项目主要用于电源的开发（起源于2025NUEDC电源题的动机）
> - 对于大多使用场景，可以使用以下环路过程：HRTIM产生PWM -> PWM中心触发ADC采样 -> DMA运输ADC数据完成触发中断 -> 中断中编写闭环代码，再次使能ADC等待新一轮闭环

## 快速开始
- **Fork** 本仓库，创建属于你自己的分支。
- 使用 `git clone` 将仓库克隆到本地。
- 使用 `CLion` 打开它。
- 打开 `STM32CubeMX` 生成代码。
- 在bsp_hrtim.c文件开头的fHRTIM宏定义中输入你设置的hrtim频率，不懂就不要乱动
- 在main.c中引入头文件
```c++
#include "bsp_hrtim.h"
#include "bsp_dual_adc.h"
```
3. 在main函数中，while(1) 开始前插入初始化代码
```c++
    BSP_ADC_DualSampleInit();//使能ADC采样
    set_deadtime(TIM_A, 100, 100);//设置死区
    init_hrtim(&hhrtim1, TIM_A, 40000);//设置定时器A的PWM频率（建议为MASTER频率的整数倍）
    set_duty(TIM_A, 0.1f);//设置定时器A的PWM占空比
    init_hrtim(&hhrtim1, MASTER, 20000);//设置定时器MASTER的频率（用于多通道PWM的同步，以及ADC的采样触发）
```
4. 声明并定义ADC中断回调函数，在里面编写闭环代码
```c++
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
        //此处编写闭环代码
        BSP_ADC_DualSampleStart();//再次使能ADC
    }
```
5. 注意安全，准备好防爆盾不要被电容或者MOS管炸死