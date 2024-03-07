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

