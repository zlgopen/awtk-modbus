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
#include "ff.h"
#include "exfuns.h"
#include "ftl.h"

#include "tkc/mem.h"
#include "tkc/platform.h"
#include "modbus_app.h"

static ret_t fatfs_test() {
  FRESULT r;

  r = f_mkdir("0:/test.dir");
  assert(r == FR_OK);

  r = f_unlink("0:/test.dir");
  assert(r == FR_OK);

  r = f_mkdir("1:/test.dir");
  assert(r == FR_OK);

  r = f_unlink("1:/test.dir");
  assert(r == FR_OK);

  return RET_OK;
}

static ret_t system_init(void) {
  HAL_Init();                       // ��ʼ��HAL��
  Stm32_Clock_Init(360, 25, 2, 8);  // ����ʱ��,180Mhz
  delay_init(180);                  // ��ʼ����ʱ����
  LED_Init();                       // ��ʼ��LED
  KEY_Init();                       // ��ʼ������
  SDRAM_Init();                     // SDRAM��ʼ��
  W25QXX_Init();

  FTL_Init();
  exfuns_init();
  f_mount(fs[0], "0:", 1);
  f_mount(fs[1], "1:", 1);

  fatfs_test();

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
