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

* 移植 tick 中断

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

## 内存配置

* TKC 的内存配置 platform/platform.c

默认为 40K，请根据自己的情况调整，不能少于 10K。

```c
uint32_t s_heap_mem[10240];
```

* FreeRTOS 的内存配置 FreeRTOS/Source/FreeRTOSConfig.h

默认为 200K，请根据自己的情况调整，如果线程数 < 4，64K 应该是够的。

```c
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 200 * 1024 ) )
```

## 其它

* 如果出现下列错误：

```
Undefined symbol wcscmp
```

> 请定义下面的宏

```
WITH_WCSXXX
```