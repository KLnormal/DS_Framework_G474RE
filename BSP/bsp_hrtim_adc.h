//
// Created by 15082 on 2025/7/14.
//

#ifndef BSP_HRTIM_ADC_H
#define BSP_HRTIM_ADC_H

#include <bsp_hrtim.h>

void adc_init(ADC_HandleTypeDef *adc_port, uint32_t *data_ary, uint32_t data_len);

#endif //BSP_HRTIM_ADC_H
