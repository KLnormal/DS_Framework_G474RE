#include "bsp_uart.h"
#include "main.h"
#include <stdarg.h>
#include <stdio.h>


#define TX_BUF_SIZE 1024
char tx_buf[TX_BUF_SIZE];


void FloatToByte(float f, uint8_t byte[]) {
    FloatLongType fl;
    fl.fdata = f;
    byte[0] = (uint8_t) fl.ldata;
    byte[1] = (uint8_t) (fl.ldata >> 8);
    byte[2] = (uint8_t) (fl.ldata >> 16);
    byte[3] = (uint8_t) (fl.ldata >> 24);
}

void BSP_UART_TransmitFloat(UART_HandleTypeDef *huartx, int num, ...) {
    uint8_t byte[4];
    va_list arg;
    va_start(arg, num);
    for (int i = 0; i < num; ++i) {
        float data = va_arg(arg, double);
        FloatToByte(data, byte);
        HAL_UART_Transmit_DMA(huartx, byte, 4);
    }
    va_end(arg);
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};
    HAL_UART_Transmit_DMA(huartx, tail, 4);
}

void BSP_UART_Transmit(UART_HandleTypeDef *huartx, const char *format, ...) {
    va_list arg;
    uint16_t len;
    va_start(arg, format);
    len = vsnprintf(tx_buf, TX_BUF_SIZE, format, arg);
    va_end(arg);
    HAL_UART_Transmit_DMA(huartx, (uint8_t *) tx_buf, len);
}
