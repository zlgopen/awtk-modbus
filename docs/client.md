## 客户端工具配置格式

## 1.connect 连接

### 参数

- url: 连接地址
- slave: 从机ID(RTU模式有效)


### 示例

```ini
[connect]
 url = tcp://localhost:502
```

```ini
[connect]
 url = serial:///dev/ttys005
 slave=1
```

## 2.write_bit 写位 

### 参数

- addr: 地址
- data: 数据

### 示例

```ini
[write_bit]
 addr = 0x130
 count = 1
 data = 1
```

## 3.write_bits 写多个位

### 参数

- addr: 地址
- count: 数量
- data: 数据, 逗号分隔

### 示例

```ini
[write_bits]
 addr = 0x130
 count = 10
 data = 1,0,1,1,0,0,1,1,1,1
```

## 4.read_bits 读多个位

### 参数

- addr: 地址
- count: 数量
- data: 期望数据(可选), 逗号分隔

### 示例

```ini
[read_bits]
 addr = 0x130
 count = 10
 data = 1,0,1,1,0,0,1,1,1,1
```

## 5.write_register 写寄存器

### 参数

- addr: 地址
- data: 数据

### data 支持变量

>参考 write_registers

### 示例

```ini
[write_register]
 addr = 0x160
 data = 1
```

## 6.write_registers 写多个寄存器

### 参数

- addr: 地址
- count: 数量
- data: 数据, 逗号分隔

### data支持变量

- year: 年
- month: 月
- day: 日
- hour: 时
- minute: 分
- second: 秒
- rand: 随机数
- count: 计数器

### 示例

```ini
[write_registers]
 addr = 0x160
 count = 10
 data = 1,0,1,1,0,0,1,1,1,1
```

```ini
[write_registers]
 addr = 40000
 count = 8 
 data = year,month,day,hour,minute,second,rand,count
```
  
## 10.read_registers 读多个寄存器

### 参数

- addr: 地址
- count: 数量
- data: 数据, 逗号分隔

### 示例

```ini
[read_registers]
 addr = 0x160
 count = 10
 data = 1,0,1,1,0,0,1,1,1,1
```

## 11. read_input_registers 读多个输入寄存器

### 参数

- addr: 地址
- count: 数量
- data: 期望数据(可选), 逗号分隔

### 示例

```ini
[read_input_registers]
 addr = 0x300
 count = 12
 data = 0,1,2,3,4,5,6,7,8,9,10,11
```

## 12. read_input_bits 读多个输入位

### 参数

- addr: 地址
- count: 数量
- data: 期望数据(可选), 逗号分隔

### 示例

```ini
[read_input_bits]
 addr = 0x200
 count = 50
 data = 0,1,0,1,0,1,0,1,0,1, 0,1,0,1,0,1,0,1,0,1, 0,1,0,1,0,1,0,1,0,1, 0,1,0,1,0,1,0,1,0,1, 0,1,0,1,0,1,0,1,0,1,
```

## 13.sleep 休眠

### 参数

- time: 时间(毫秒)

### 示例

```ini
[sleep]
  time = 1000
```

## 14.rewind 重置

### 示例

```ini
[rewind]
```

## 15.goto 跳转

### 参数
- target : 目标

### 示例

```ini
[goto]
  target = write_register
```

## 16.close 关闭连接

### 示例

```ini
[close]
```
