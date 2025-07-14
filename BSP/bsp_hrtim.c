//
// Created by 15082 on 2025/7/7.
//

#include "bsp_hrtim.h"
#define TIM_PORT(time_port) 0x1UL << (17U+tim_port)
#define TIM_CHANEL(time_port,CHANEL) (0x01<<(tim_port*2+CHANEL-1))
#define range(num,min,max) (((num)>=(min)) && ((num)<=(max)))
#define MAX_PORT_CNT 6
#define CLOCK_F 170.00

typedef void (*f)(void);
f hrtim_callback_ary[MAX_PORT_CNT+4];

struct_hrtim hrtim_ary[6+4];


//频率的范围为15.62Khz ~ 1024K hz
void init_hrtim(HRTIM_HandleTypeDef *hrtimx, e_tim tim_port,int32_t target_frequency)
{
    HAL_HRTIM_WaveformCounterStop(&hhrtim1, TIM_PORT(tim_port));
    hrtim_ary[tim_port].tim_port = tim_port;
    hrtim_ary[tim_port].deadtime = 100;//默认开启死区 100cnt
    if (range(target_frequency,FREQUENCY_MIN,FREQUENCY_MAX))
        hrtim_ary[tim_port].frequency = target_frequency;
    else hrtim_ary[tim_port].frequency = 25*1000;
    hrtim_ary[tim_port].rate = CLOCK_F * 32ULL * 1000ULL * 1000ULL /2/ (uint32_t)hrtim_ary[tim_port].frequency;
    hrtim_ary[tim_port].count_ns = 1000/CLOCK_F;
    HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg;
    pTimeBaseCfg.Period = hrtim_ary[tim_port].rate;
    pTimeBaseCfg.RepetitionCounter = 0x00;
    pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
    pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;
    HAL_HRTIM_TimeBaseConfig(&hhrtim1, tim_port, &pTimeBaseCfg);
    HAL_HRTIM_WaveformOutputStart(hrtimx, TIM_CHANEL(tim_port,1)|TIM_CHANEL(tim_port,2));//chanel 从1开始
    HAL_HRTIM_WaveformCounterStart(hrtimx, TIM_PORT(tim_port));

}

//target_duty输入百分比，100%为1
void set_duty(e_tim tim_port, float target_duty)
{
    int32_t ccr;
    ccr = hrtim_ary[tim_port].rate*target_duty;
    __HAL_HRTIM_SetCompare(&hhrtim1,tim_port,HRTIM_COMPAREUNIT_1,ccr);
}
//输入单位nm，最高支持370ns
void set_deadtime(e_tim tim_port,float deadtime_rising, float deadtime_falling)
{
    int deadtime_count_ris = 8*deadtime_rising/hrtim_ary[tim_port].count_ns;
    int deadtime_count_fal = 8*deadtime_falling/hrtim_ary[tim_port].count_ns;
    // int deadtime_count_ris = deadtime_rising/hrtim_ary[tim_port].count_ns;
    // int deadtime_count_fal = deadtime_falling/hrtim_ary[tim_port].count_ns;
    if (range(deadtime_count_ris,0,511) && range(deadtime_count_fal,0,511))
    {
        //tmd挨个改东西太多了，部分不太可能改的东西就写死了，不想弄了，下班！
        HRTIM_DeadTimeCfgTypeDef deadtime_cfg;
        deadtime_cfg.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8; //默认8倍频
        deadtime_cfg.RisingValue = deadtime_count_ris;
        deadtime_cfg.FallingValue = deadtime_count_fal;
        deadtime_cfg.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
        deadtime_cfg.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
        deadtime_cfg.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
        deadtime_cfg.FallingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
        deadtime_cfg.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
        deadtime_cfg.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
        HAL_HRTIM_DeadTimeConfig(&hhrtim1,tim_port,&deadtime_cfg);
    }
    else Error_Handler();
};
void hrtim_it_init(e_tim tim_port,  void (*f) (void))
{
    __HAL_HRTIM_TIMER_ENABLE_IT(&hhrtim1,tim_port,HRTIM_TIM_IT_REP);
    hrtim_callback_ary[tim_port] = f;
}
void hrtim_it_use(e_tim tim_port)
{
    hrtim_callback_ary[tim_port]();
}

