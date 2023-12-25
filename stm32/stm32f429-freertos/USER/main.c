#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "ltdc.h"
#include "base/g2d.h"
#include "touch.h"

/************************************************
 ALIENTEK ������STM32F429������ʵ��42
 ������ʾʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#include "tkc/mem.h"
#include "tkc/platform.h"
#include "modbus_app.h"

static ret_t system_init(void) {
  HAL_Init();                       //��ʼ��HAL��
  Stm32_Clock_Init(360, 25, 2, 8);  //����ʱ��,180Mhz
  delay_init(180);                  //��ʼ����ʱ����
  LED_Init();                       //��ʼ��LED
  KEY_Init();                       //��ʼ������
  SDRAM_Init();                     // SDRAM��ʼ��

  return RET_OK;	
}

int main(void) {
  platform_prepare();
	system_init();

  rtos_init();
  modbus_service_start("1");
  rtos_start();
	
	return 0;
}
