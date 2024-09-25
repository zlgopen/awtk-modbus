2024-09-25
  * 修复外部主机写线圈时候响应信息出错的问题(感谢智明提供补丁)
  * 修复外部主机无法修改保持寄存器的问题以及在modbus_server_channel增加锁 保护数据(感谢智明提供补丁)
  * 使用mutex锁代替mutex避免死锁(感谢智明提供补丁)

2024-09-19
  * 修复06功能码响应数据不正确的问题(感谢雨欣提供补丁)

2024-07-23
  * 增加支持23操作码（支持同时读写操作）(感谢智明提供补丁)

2024-07-19
  * 部分文件加上utf8bom，避免vs2015编译出错(感谢陈谭提供补丁)

2024-06-24
  * 增加配置channel的unit_id函数(感谢智明提供补丁) 
  * 增加配置帧间隔时间和帧应答超时时间(感谢智明提供补丁) 

2024-06-17
  * 如果modbus client channel指定了unit id，就用它，没有指定就用默认的。

2024-04-17
  * 修复 api doc错误。
  * 修复modbus\_client\_channel\_update重复定义的问题(感谢俊杰提供补丁)

2024-03-25
  * 完善modbus client channel，对于长数据分多次读写数据。

2024-03-21
  * 增加文档 server\_conf.md。
  
2024-03-20
  * modbus client channel 启用keep\_last\_value\_if\_read\_failed参数。

2024-03-11
  * 完善 modbus\_memory\_default，支持触发事件。

2024-03-10
  * 重构 modbus\_memory\_default。

2024-03-8
    * client tool 增加rewind/goto指令。
    * client 支持变量。
    * 增加client工具配置文档。

2024-03-7
  * 增加 modbus\_client\_channel
  * 增加 modbus\_init\_req

2024-03-6
  * 完善server_ex。
  * retry only when CRC error

2024-02-23
  * 修正通信出错后（比如slave端突然休眠），tid错位导致通信无法恢复的情况。方法是通信出错并重试之后，执行新增的modbus\_client_flush_read_buffer函数读空输入缓冲区(感谢陈谭提供补丁)

2024-02-13
  * 增加server demo，从配置文件加载数据。

2024-01-07
  * 修复 transaction_id 检查失败问题。

2023-12-25
  * 完善stm32工程，内存统一由FreeRTOS管理。

2023-11-07
  * 将memory改为接口，去掉hook。

