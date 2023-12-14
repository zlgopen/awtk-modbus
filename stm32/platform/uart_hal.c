/**
 * File:   uart_hal.c
 * Author: AWTK Develop Team
 * Brief:  uart_hal
 *
 * Copyright (c) 2023 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-11-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "uart_hal.h"

#ifdef HMI_ZDP1440D
#define WITH_UART 1
#include "zdp.inc"
#elif defined(STM32H743xx)
#define WITH_UART 1
#include "stm32743.inc"
#endif /*HMI_ZDP1440D*/

/*========================================================*/

#ifdef WITH_UART

int uart_debug(const char* format, ...) {
  int ret = 0;
  va_list va;
  char str[1024] = {0};
  uint32_t size = sizeof(str) - 1;
  return_value_if_fail(format != NULL, 0);

  va_start(va, format);
  ret = tk_vsnprintf(str, size, format, va);
  va_end(va);

  uart_do_send(NULL, str, strlen(str));

  return ret;
}

static bool_t uart_is_fd_valid(int32_t fd) {
  return fd > 0 && fd <= MAX_UART_COUNT;
}

static uart_device_t* uart_get_dev(int32_t fd) {
  int32_t index = fd - 1;
  return_value_if_fail(uart_is_fd_valid(fd), NULL);
  return &s_uart_devices[index];
}

int uart_open(const char* name, int baudrate) {
  int32_t fd = name != NULL ? tk_atoi(name) : 1;
  uart_device_t* dev = uart_get_dev(fd);
  return_value_if_fail(dev != NULL, -1);

  if (dev->rx_ring_buff != NULL) {
    return fd;
  }

  dev->rx_ring_buff = ring_buffer_create(2048, 2048);
	
	uart_do_open(dev, fd, baudrate);
	
  return fd;
}

int uart_close(int fd) {
  uart_device_t* dev = uart_get_dev(fd);
  return_value_if_fail(dev != NULL, -1);

  uart_do_close(dev);
  ring_buffer_destroy(dev->rx_ring_buff);
  dev->rx_ring_buff = NULL;

  return 0;
}

int uart_read(int fd, void* buffer, uint32_t size) {
  uart_device_t* dev = uart_get_dev(fd);
  return_value_if_fail(dev != NULL, -1);
  return_value_if_fail(dev->rx_ring_buff != NULL, -1);

  return ring_buffer_read(dev->rx_ring_buff, buffer, size);
}

bool_t uart_has_data(int fd) {
  uart_device_t* dev = uart_get_dev(fd);
  return_value_if_fail(dev != NULL, FALSE);
  return_value_if_fail(dev->rx_ring_buff != NULL, FALSE);

  return !ring_buffer_is_empty(dev->rx_ring_buff);
}

int uart_write(int fd, const void* buffer, uint32_t size) {
  uart_device_t* dev = uart_get_dev(fd);
  return_value_if_fail(dev != NULL, -1);
  return_value_if_fail(dev->rx_ring_buff != NULL, -1);

  return uart_do_send(dev, buffer, size);
}

void uart_test(const char* name) {
  int len = 0;
  int fd = uart_open(name, 115200);
  uint8_t buff[128] = {0};

  if (fd >= 0) {
    int count = 10000;
    while (count-- > 0) {
      len = uart_read(fd, buff, sizeof(buff));
      if (len > 0) {
        uart_write(fd, buff, len);
      } else {
        sleep_ms(10);
      }
    }

    uart_close(fd);
  } else {
    uart_debug("uart open %s failed\r\n", name);
  }
}
#else
int uart_debug(const char* format, ...) {
  return 0;
}
#endif /*WITH_UART*/
