//
// Created by 15082 on 2025/7/14.
//

#include "bsp_hrtim_adc.h"

#include "adc.h"

#define ADC1_PORTS 4
#define ADC2_PORTS 4

uint16_t adc1_data[4] = {0};
uint16_t adc2_data[4] = {0};
uint16_t adc3_data[4] = {0};
float data_voltage[3][4] = {0};

void adc_init(ADC_HandleTypeDef *adc_port)
{
    HAL_ADC_Start(adc_port);
    HAL_ADCEx_Calibration_Start(adc_port,ADC_SINGLE_ENDED);

    if(adc_port == &hadc1)
    {
        HAL_ADC_Start_DMA(adc_port, adc1_data,ADC1_PORTS);
        HAL_ADC_Start_DMA(adc_port, adc3_data,1);
    }
    else if(adc_port == &hadc2)
        HAL_ADC_Start_DMA(adc_port, adc2_data,ADC2_PORTS);
}

void converse_adc1()
{
    HAL_ADC_Start_DMA(&hadc1, adc1_data,ADC1_PORTS);
    HAL_ADC_Start_DMA(&hadc3, adc3_data,1);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start(&hadc3);
    // HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
}

void converse_adc2()
{
    HAL_ADC_Start_DMA(&hadc2, adc2_data,ADC2_PORTS);
    HAL_ADC_Start(&hadc2);
    // HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
}