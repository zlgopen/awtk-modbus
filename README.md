#  AWTK-MODBUS

Modbus 协议是一种通信协议，用于在工业自动化系统中实现设备之间的通信。它最初由 Modicon（现在是施耐德电气的一部分）在 1979 年开发，成为工业领域中最常用的通信协议之一。

Modbus 协议简单、易于实现和使用，因此在工业自动化领域得到广泛应用。它可以用于监控和控制各种设备，如传感器、执行器、电机驱动器等。通过使用 Modbus 协议，不同厂家的设备可以方便地进行集成和通信，实现设备之间的数据交换和控制操作。

[AWTK-MODBUS](https://github.com/zlgopen/awtk-modbus.git) 实现了 Modbus 协议的基本功能。其主要特色有：

* 小巧。充分利用 [TKC](https://github.com/zlgopen/tkc.git) 中的函数，大大减少代码量。
* 可移植。可以在 PC 和嵌入式设备上运行。
* 支持 TCP 和 RTU 两种通信方式。
* 支持客户端 (Master) 和 服务器 (Slave)。
* 与  [AWTK](https://github.com/zlgopen/awtk.git) 无缝集成。服务端 (Slave) 可以集成到 AWTK 的主循环。
* 服务器 (Slave) 支持自定义的memory，在处理请求的前后被调用，方便对数据进行预读和刷新。
* 支持设置自动重试的次数。

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

* 编译 awtk-modbus 并编译

```
git clone https://github.com/zlgopen/awtk-modbus.git
cd awtk-modbus; scons; cd -
```

> 注意：awtk 和 awtk-modbus 并列放在同一级目录。

* 编译 LINUX FB 版本

```
scons LINUX_FB=true
```

> 完整编译选项请参考 [编译选项](https://github.com/zlgopen/awtk-widget-generator/blob/master/docs/build_options.md)

## 运行示例

### MODBUS-TCP

* 服务端

```
./bin/modbus_server tcp://localhost:502
```

* 服务端 (集成到AWTK主循环)

```
./bin/modbus_server_awtk tcp://localhost:502
```

* 服务端 (支持从配置文件加载参数，方便测试)

```
bin/modbus_server_ex [url] [config]
```

示例：

```
./bin/modbus_server_ex tcp://localhost:502 config/default.ini
```

> RTU 可以指定 unit id。

```
bin/modbus_server_ex [url] [config] [unit_id]
```

示例：

```
./bin/modbus_server_ex serial:///dev/ttys017 config/default.ini 1
./bin/modbus_server_ex serial:///dev/ttys017 config/default.ini 2
```

* 客户端

```
./bin/modbus_client data/tcp.ini
```

### MODBUS-RTU

* 服务端

```
./bin/modbus_server serial:///dev/ttys124
```

* 服务端 (集成到AWTK主循环)

```
./bin/modbus_server_awtk serial:///dev/ttys124
```

* 客户端

```
./bin/modbus_client data/rtu.ini
```

### MODBUS-RTU-OVER-TCP

* 服务端

```
./bin/modbus_server rtu+tcp://localhost:502
```

* 服务端 (集成到AWTK主循环)

```
./bin/modbus_server_awtk rtu+tcp://localhost:502
```

* 客户端

```
./bin/modbus_client data/rtu_over_tcp.ini
```

> MacOS 上可以用 socat 模拟串口。如：

```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

> 本文以 Linux/MacOS 为例，Windows 可能会微妙差异，请酌情处理。

### 致谢

 * 感谢 QY 同学提供大量建设性意见和使用反馈。


