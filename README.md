#  AWTK-MODBUS

Modbus 协议是一种通信协议，用于在工业自动化系统中实现设备之间的通信。它最初由 Modicon（现在是施耐德电气的一部分）在 1979 年开发，成为工业领域中最常用的通信协议之一。

Modbus 协议简单、易于实现和使用，因此在工业自动化领域得到广泛应用。它可以用于监控和控制各种设备，如传感器、执行器、电机驱动器等。通过使用 Modbus 协议，不同厂家的设备可以方便地进行集成和通信，实现设备之间的数据交换和控制操作。

AWTK-MODBUS 实现了 Modbus 协议的基本功能。其主要特色有：

* 小巧。充分利用 [TKC](https://github.com/zlgopen/tkc.git) 中的函数，大大减少代码量。
* 可移植。可以在 PC 和嵌入式设备上运行。
* 支持 TCP 和 RTU 两种通信方式。
* 支持客户端 (Master) 和 服务器 (Slave)。
* 与 AWTK 无缝集成。服务端 (Slave) 可以集成到 AWTK 的主循环。
* 服务器 (Slave) 支持设置钩子函数，在处理请求的前后被调用，方便对数据进行预读和刷新。

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

> 注意：awtk 和 awtk-modbus 并列放在同一级目录。

* 编译 PC 版本

```
scons
```

* 编译 LINUX FB 版本

```
scons LINUX_FB=true
```

> 完整编译选项请参考 [编译选项](https://github.com/zlgopen/awtk-widget-generator/blob/master/docs/build_options.md)

## 运行示例

* TCP 服务端 (Slave)

```
./bin/modbus_server tcp://localhost:502
```

或（集成到 AWTK 主循环）

```
./bin/modbus_server_awtk serial:///dev/ttys124
```

* TCP 客户端 (Master)

```
./bin/modbus_client data/tcp.ini
```

* RTU 服务端 (Slave)

```
./bin/modbus_server serial:///dev/ttys124
```

或（集成到 AWTK 主循环）

```
./bin/modbus_server_awtk serial:///dev/ttys124
```

* RTU 客户端 (Master)

```
./bin/modbus_client data/rtu.ini
```

> MacOS 上可以用 socat 模拟串口。如：

```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

> 本文以 Linux/MacOS 为例，Windows 可能会微妙差异，请酌情处理。
