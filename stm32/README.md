# STM32 移植指南

## 下载 AWTK/AWTK-MODBUS/AWTK-FS-ADAPTER 到同一个目录

```
git clone https://github.com/zlgopen/awtk
```

```
git clone https://github.com/zlgopen/awtk-modbus
```

```
git clone https://github.com/zlgopen/awtk-fs-adapter
```

> 如果访问 github 不方便，可以从 gitee 上获取。

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
awtk\src\platforms\raw\fs_test.c
```

* 当前 platform 目录中的下列文件（建议放到 platform 分组中）。

```
platform\assert.c
platform\platform.c
platform\serial_helper.c
platform\sys_tick.c
platform\uart_hal.c
platform\memory_freertos.c
platform\fs_test.c
```

* awtk-modbus/src 目录中的全部 C 文件（建议放到 modbus 分组中）。

* awtk-fs-adapter 目录中的下列文件 （建议放到 platform 分组中）。

```
src\fs_os_fatfs.c
src\fs_mt.c
```

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

## 多线程访问文件系统

FATFS 默认不支持多线程访问，TKC FS 做了一层包装（参考 fs_mt.c)，如果需要启用多线程访问。请在工程中定义宏：WITH\_FS\_MT

## 内存配置

> 内存全部由 FreeRTOS 统一管理。

* 在工程中定义宏：WITH\_OS\_MEM
* 从工程中去掉文件 malloc.c
* 添加 platform/memory_freertos.c
* 老版本需要从工程中去掉文件 heap_4.c

根据自己的情况，修改 FreeRTOS 的内存配置 (FreeRTOS/Source/FreeRTOSConfig.h)

```c
#ifdef STM32F10X_HD
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 60 * 1024 ) )
#else
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 128 * 1024 ) )
#endif
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

* 如果出现下列错误：

```
..\OBJ\awtk.axf: Error: L6200E: Symbol PendSV_Handler multiply defined (by port.o and stm32f4xx_it.o).
..\OBJ\awtk.axf: Error: L6200E: Symbol SVC_Handler multiply defined (by port.o and stm32f4xx_it.o).
..\OBJ\awtk.axf: Error: L6200E: Symbol SysTick_Handler multiply defined (by sys_tick_handler.o and stm32f4xx_it.o).
```

> 请删除 stm32f4xx_it.c 中对应函数。


* 如果在 rtos_start 之前调用了 sleep_ms 函数，可能会进入死循环。可以用下面的方法解决：


将

```
static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;
```

改为

```
static UBaseType_t uxCriticalNesting = 0;
```