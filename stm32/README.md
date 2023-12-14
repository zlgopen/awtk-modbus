# STM32 移植指南

## 下载 AWTK/AWTK-MODBUS 到同一个目录

```
git clone https://github.com/zlgopen/awtk
```

```
git clone https://github.com/zlgopen/awtk-modbus
```

## 打开 keil 模板工程，

### 添加源文件

* awtk/src/tkc 目录中的全部 C 文件（建议放到 tkc 分组中）。

* awtk/src/service 目录中的全部 C 文件（建议放到 service 分组中）。

* awtk/src/streams 目录中的全部 C 文件（建议放到 streams 分组中）。

* awtk/src/streams/serial 目录中的全部 C 文件（建议放到 streams 分组中）。

* awtk/src/platforms 目录中的的下列文件（建议放到 platform 分组中）。

```
awtk\src\platforms\common\sys_tick_handler.c
awtk\src\platforms\freertos\mutex.c
awtk\src\platforms\freertos\rtos.c
awtk\src\platforms\freertos\semaphore.c
awtk\src\platforms\freertos\thread.c
awtk\src\platforms\raw\cond_var_null.c
awtk\src\platforms\raw\dl_null.c
awtk\src\platforms\raw\fs_os.c
```

* 当前 platform 目录中的下列文件（建议放到 platform 分组中）。

```
platform\assert.c
platform\platform.c
platform\serial_helper.c
platform\sys_tick.c
platform\uart_hal.c
```

* awtk-modbus/src 目录中的全部 C 文件（建议放到 modbus 分组中）。

* 添加 FreRTOS 文件 （建议放到 freertos 分组中）。

```
FreeRTOS\Source\croutine.c
FreeRTOS\Source\event_groups.c
FreeRTOS\Source\list.c
FreeRTOS\Source\queue.c
FreeRTOS\Source\tasks.c
FreeRTOS\Source\timers.c
FreeRTOS\Source\portable\MemMang\heap_4.c
FreeRTOS\Source\FreeRTOSConfig.h
```

> 下面这个文件根据硬件平台选择相应的文件：

```
FreeRTOS\Source\portable\RVDS\ARM_CM7\r0p1\port.c
```

## 添加 include 路径

```
awtk\src
awtk-modbus\src
FreeRTOS\Source
FreeRTOS\Source\include
```

> 下面的路径根据硬件平台选择相应的路径：

```
FreeRTOS\Source\portable\RVDS\ARM_CM7\r0p1
```

## 移植


* 移植tick中断

```
platform\sys_tick.c
```

* 串口

```
platform\uart_hal.c
```

参考

```
stm32743.inc
```
