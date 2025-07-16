//
// Created by 15082 on 2025/7/14.
//

#include "bsp_hrtim_adc.h"

#include "adc.h"

#define ADC1_PORTS 4
#define ADC2_PORTS 4

void adc_init(ADC_HandleTypeDef *adc_port, uint32_t *data_ary, uint32_t data_len)
{
    HAL_Delay(100);
    HAL_ADCEx_Calibration_Start(adc_port,ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(adc_port,data_ary,data_len);
}