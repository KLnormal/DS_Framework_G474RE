# DS_Framework_G474RE

## 快速开始
1. 先clone到本地然后cubemx生成工程（和RM一样），要求配置HRTIM的时钟频率为128MHZ，32倍频
2. 在main.c中引入头文件
`#include "bsp_hrtim.h"`
3. 在main函数中，while(1) 开始前插入初始化代码
```c++
  init_hrtim(&hhrtim1,TIM_A,25000);
  init_hrtim(&hhrtim1,TIM_B,25000);
  init_hrtim(&hhrtim1,TIM_C,25000);
  init_hrtim(&hhrtim1,TIM_D,25000);
  init_hrtim(&hhrtim1,TIM_E,25000);
  init_hrtim(&hhrtim1,TIM_F,25000);
```
4. 享受你的电源题坐牢之旅