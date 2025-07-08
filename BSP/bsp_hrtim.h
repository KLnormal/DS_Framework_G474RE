//
// Created by 15082 on 2025/7/7.
//

#ifndef BSP_HRTIM_H
#define BSP_HRTIM_H
#include <cstdint>
#include "hrtim.h"
#include "main.h"

typedef enum {TIM_A = 0,TIM_B,TIM_C,TIM_D,TIM_E,TIM_F} e_tim;
namespace BSP_TIM
{

    class hrtim_class
    {
    public:
        hrtim_class(HRTIM_HandleTypeDef *hrtimx, e_tim tim_port)
        {
            if (hrtimx != &hhrtim1) return;
            HAL_HRTIM_WaveformOutputStart(hrtimx, (0x01<<tim_port*2)|(0x01<<tim_port*2+1));
            HAL_HRTIM_WaveformCounterStart(hrtimx, 0x1UL << (17U+tim_port));
        };
    };

}


#endif //APP_HRTIM_H
