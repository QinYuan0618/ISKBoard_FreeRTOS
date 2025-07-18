/*
 * miscdev.c
 *
 *  Created on: 2025/07/18
 *      Author: qinyuan
 */

#include "miscdev.h"


/*
 *+----------------------+
 *|      printf API      |
 *+----------------------+
 */
#include "usart.h"  /* 全局变量huart1定义在 usart.c中，并在sart.h头文件中声明 */

/* gcc 编译器中的 printf 函数将会调用 __io_putchar() 函数，实现最终的字符打印
 * keil编译器中的 printf 函数将会调用 fputc() 函数，实现最终的字符打印
 * 这里我们定义一个宏 PUTCHAR_PROTOTYPE 来兼容这两个编译器所需要的函数原型；
 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/*
 * STM32CubeIDE使用的是 gcc 编译器，它会内建 __GNUC__ 宏定义，所以下面PUTCHAR_PROTOTYPE将会扩展为:
 *  int __io_putchar(int ch)
 */
PUTCHAR_PROTOTYPE
{
    /* 调用STM32 HAL库的串口发送函数，将printf要打印的这个字符通过串口发送出去 */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}


