//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK STM32H7开发板
// 串口1初始化
// 正点原子@ALIENTEK
// 技术论坛:www.openedv.com
// 修改日期:2017/6/8
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2009-2019
// All rights reserved
//********************************************************************************
// V1.0修改说明
// Li XianJing <xianjimli@hotmai.com> 2023-11-09 封装成串口函数。
//////////////////////////////////////////////////////////////////////////////////

#include "system.h"
#include "stdio.h"

#define WITH_PRINTF 1
#define RXBUFFERSIZE 1

#if defined(WITH_PRINTF)
struct __FILE {
  int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) {
  x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE* f) {
  return ch;
}
#endif /*WITH_PRINTF*/

typedef struct _uart_device_t {
  uint8_t rx_buff[RXBUFFERSIZE];
  ring_buffer_t* rx_ring_buff;
} uart_device_t;

#define MAX_UART_COUNT 4
static uart_device_t s_uart_devices[MAX_UART_COUNT];

void USART1_Init(u32 bound) {
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //打开时钟

  /*  配置GPIO的模式和IO口 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //TX			   //串口输出PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);                 /* 初始化串口输入IO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //RX			 //串口输入PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                 /* 初始化GPIO */

  //USART1 初始化设置
  USART_InitStructure.USART_BaudRate = bound;                  //波特率设置
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;       //一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;          //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;                              //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //收发模式
  USART_Init(USART1, &USART_InitStructure);                        //初始化串口1

  USART_Cmd(USART1, ENABLE);  //使能串口1

  USART_ClearFlag(USART1, USART_FLAG_TC);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //开启相关中断

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;          //串口1中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化VIC寄存器、
}

static ret_t uart_do_open(uart_device_t* dev, int fd, int baudrate) {
  // UART 初始化设置
  if (fd == 1) {
    USART1_Init(baudrate);
  }

  return RET_OK;
}

ret_t uart_do_close(uart_device_t* dev) {
  return RET_OK;
}

static int uart_send_char(uart_device_t* dev, uint8_t ch) {
  USART_SendData(USART1, (u8)ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    ;

  return 1;
}

static int uart_do_send(uart_device_t* dev, const void* buffer, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    uart_send_char(dev, ((uint8_t*)buffer)[i]);
  }

  return size;
}

void USART1_IRQHandler(void) {
  uart_device_t* dev = &s_uart_devices[0];

  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    u8 r = USART_ReceiveData(USART1); 
    ring_buffer_write(dev->rx_ring_buff, &r, 1);
  }
  USART_ClearFlag(USART1, USART_FLAG_TC);
}
