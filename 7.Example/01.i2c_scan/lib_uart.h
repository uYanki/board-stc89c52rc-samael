#ifndef __LIB_UART__
#define __LIB_UART__

#include <REGX52.H>

#ifndef _LIB_FUNC_
#define _LIB_FUNC_

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#endif

void uart_init(void);
void uart_sendline(void);
void uart_sendchar(char byte);
void uart_sendstr(char* string);
void uart_sendstr_ln(char* string);  // 带换行符

// 妈卖批 51单片机 得加上关键字 reentrant 才能递归, 否则出错, 操, 排了好久的错
void uart_senddec(int number) reentrant;
void uart_sendhex(int number) reentrant;

// 51单片机关键字: https://blog.csdn.net/armtravel/category_1136032.html

extern void uart_handle(uint8_t recv);

#endif
