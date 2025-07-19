# DS_Framework_G474RE

## 快速开始
1. 先clone到本地然后cubemx生成工程（和RM一样）
2. 在bsp_hrtim.c文件开头的CLOCK_F宏定义中输入你设置的hrtim频率，不懂就不要乱动
2. 在main.c中引入头文件
```c++
#include "bsp_hrtim.h"
#include "bsp_hrtim_adc.h"
```
3. 在main函数中，while(1) 开始前插入初始化代码
```c++
  init_hrtim(&hhrtim1,TIM_A,200000,0);
  init_hrtim(&hhrtim1,TIM_B,200000,0);
  init_hrtim(&hhrtim1,TIM_C,200000,0);
  init_hrtim(&hhrtim1,TIM_D,25000,0);
  init_hrtim(&hhrtim1,TIM_E,120000,0);
  init_hrtim(&hhrtim1,TIM_F,15000,0);
  init_hrtim(&hhrtim1,TIM_A,200000,1);
  set_duty(TIM_E,0.1);
  set_deadtime(TIM_E,60,100);
  uint32_t data1[3] = {1,1,1};
  uint32_t data2[3] = {1,1,1};
  adc_init(&hadc1,data1,3);
  adc_init(&hadc2,data2,3);
```
4. 享受你的电源题坐牢之旅