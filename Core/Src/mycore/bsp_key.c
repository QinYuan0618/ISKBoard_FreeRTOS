/*
 * bsp_key.c
 *
 *  Created on: Jul 18, 2025
 *      Author: Administrator
 */

/*
 * bsp_key.c
 *
 *  Created on: Jul 18, 2025
 *      Author: qinyuan
 */

#include "bsp_key.h"

/**
 * @brief  按键扫描函数
 * @param  GPIOx: 按键所在的GPIO端口
 * @param  GPIO_Pin: 按键对应的GPIO引脚
 * @retval KEY_ON: 按键按下, KEY_OFF: 按键释放
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* 检测是否有按键按下 */
    if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET) {
        /* 等待按键释放 */
        while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);
        return KEY_ON;
    } else {
        return KEY_OFF;
    }
}


