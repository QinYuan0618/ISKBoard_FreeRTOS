/*
 * bsp_key.h
 *
 *  Created on: Jul 18, 2025
 *      Author: Administrator
 */

#ifndef INC_BSP_KEY_H_
#define INC_BSP_KEY_H_

#include "main.h"

/* 按键状态定义 */
#define KEY_ON      1
#define KEY_OFF     0

/* 按键GPIO定义 (需要根据你的硬件配置修改) */
#define KEY1_GPIO_PORT          Key1_GPIO_Port
#define KEY1_GPIO_PIN           Key1_Pin
#define KEY1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY2_GPIO_PORT          Key2_GPIO_Port
#define KEY2_GPIO_PIN           Key2_Pin
#define KEY2_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

/* 按键读取宏定义 */
#define KEY1_STATE_READ()       HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)
#define KEY2_STATE_READ()       HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)

/* 函数声明 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /* INC_BSP_KEY_H_ */
