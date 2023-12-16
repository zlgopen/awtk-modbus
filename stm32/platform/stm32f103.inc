//TODO：本文件串口没有实现
//////////////////////////////////////////////////////////////////////////////////

#include "stdio.h"

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

static ret_t uart_do_open(uart_device_t* dev, int fd, int baudrate) {
  return RET_OK;
}

ret_t uart_do_close(uart_device_t* dev) {
  return RET_OK;
}

static int uart_send_char(uart_device_t* dev, uint8_t ch) {
  return 1;
}

static int uart_do_send(uart_device_t* dev, const void* buffer, uint32_t size) {
  uint32_t i = 0;
  for (i = 0; i < size; i++) {
    uart_send_char(dev, ((uint8_t*)buffer)[i]);
  }

  return size;
}