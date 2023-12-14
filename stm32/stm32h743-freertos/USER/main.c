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
 ALIENTEK ������STM32H7������ ʵ��42
 FATFSʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#include "tkc/platform.h"
#include "modbus_app.h"


static void system_init(void) {
  Cache_Enable();                 
  MPU_Memory_Protection();        
  HAL_Init();                   
  Stm32_Clock_Init(160,5,2,4); 
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
}

int main(void)
{
	system_init();
  platform_prepare();

  rtos_init();
	modbus_service_start("1");
  rtos_start();

  return 0;
}

