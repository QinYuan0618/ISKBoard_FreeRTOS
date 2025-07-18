/*
 * bsp_led.h
 *
 *  Created on: Jul 18, 2025
 *      Author: qinyuan
 */

#ifndef SRC_MYCORE_BSP_LED_H_
#define SRC_MYCORE_BSP_LED_H_

#include "main.h"

/* LED1 定义 (使用RGB_Blue引脚) */
#define LED1_PIN                    RGB_Blue_Pin
#define LED1_GPIO_PORT              RGB_Blue_GPIO_Port

/* LED操作宏定义 */
#define LED1_ON                     HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET)
#define LED1_OFF                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET)
#define LED1_TOGGLE                 HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN)

/* LED初始化函数声明 */
void LED_GPIO_Config(void);

#endif /* SRC_MYCORE_BSP_LED_H_ */
