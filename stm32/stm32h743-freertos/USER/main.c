#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ltdc.h"
#include "lcd.h"
#include "sdram.h"
#include "usmart.h"
#include "pcf8574.h"
#include "mpu.h"
#include "malloc.h"
#include "w25qxx.h"
#include "sdmmc_sdcard.h"
#include "nand.h"
#include "ftl.h"
#include "ff.h"
#include "touch.h"
#include "exfuns.h"
#include "FreeRTOS.h"
#include "task.h"

/************************************************
 ALIENTEK 阿波罗STM32H7开发板 实验42
 FATFS实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/platform.h"

#include "fs_test.h"

#include "modbus_app.h"

static void system_init(void) {
  Cache_Enable();
  MPU_Memory_Protection();
  HAL_Init();
  Stm32_Clock_Init(160, 5, 2, 4);
  delay_init(400);

  LED_Init();
  KEY_Init();
  SDRAM_Init();
  LCD_Init();
  W25QXX_Init();
  LTDC_Display_Dir(1);
  FTL_Init();
  exfuns_init();
  tp_dev.init();

  f_mount(fs[0], "0:", 1);
  f_mount(fs[1], "1:", 1);

  tkc_fs_test();
}

int main(void) {
  platform_prepare();
  system_init();

  rtos_init();
  modbus_service_start("1");
  rtos_start();

  return 0;
}
