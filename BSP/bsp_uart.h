#ifndef _VOFA_H
#define _VOFA_H

#include "main.h"


typedef union {
    float fdata;
    uint32_t ldata;
} FloatLongType;


void BSP_UART_TransmitFloat(UART_HandleTypeDef *huartx, int num, ...);
void BSP_UART_Transmit(UART_HandleTypeDef *huartx, const char *format, ...);


#endif
