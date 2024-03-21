# AWTK-MODBUS 服务器

## 1. 介绍

AWTK-MODBUS 提供了一个简单的 MODBUS 服务器，可以通过配置文件来定义寄存器和位的数量和初始值。

启动方法：

```sh
bin/modbus_server_ex config/default.json
```

## 2. 配置文件

配置文件使用JSON格式。

* url: 连接地址
* auto\_inc\_input\_registers : 自动增加输入寄存器，默认为false
* channels: 通道列表
  * name: 通道名称
  * writable: 是否可写
  * start: 起始地址
  * length: 长度
* init: 初始值
  * input\_registers: 输入寄存器初始值
  * input\_bits: 输入位初始值
  * registers: 寄存器初始值
  * bits: 位初始值

> 初始化的值是一个字符串，值之间用逗号分隔，每个值代表一个字节的数据。对于bits而言，一个值代表8个位。对于寄存器而言，两个值代表一个寄存器，第一个值代表低字节，第二个值代表高字节。

```json
{
  "url": "tcp://localhost:502",
  "channels": [
    {
      "name": "bits",
      "writable": true,
      "start": 0,
      "length": 1000
    },
    {
      "name": "input_bits",
      "start": 0,
      "length": 1000
    },
    {
      "name": "registers",
      "writable": true,
      "start": 0,
      "length": 1000
    },
    {
      "name": "input_registers",
      "start": 0,
      "length": 1000
    }
  ],
  "init": {
      "input_registers": "96,97,98,99,0,100,101,102,103,0"
  }
}
```