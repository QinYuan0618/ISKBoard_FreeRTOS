/*
 * bsp_led.c
 *
 *  Created on: Jul 18, 2025
 *      Author: qinyuan
 */

#include "bsp_led.h"

/**
 * @brief  LED GPIO配置
 * @param  无
 * @retval 无
 */
void LED_GPIO_Config(void)
{
    /* GPIO在MX_GPIO_Init()中已经初始化，这里只是预留函数 */
    /* 如果需要额外的LED配置，可以在这里添加 */

    /* 确保LED初始状态为关闭 */
    LED1_OFF;
}
