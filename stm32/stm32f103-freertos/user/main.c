#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "time.h"
#include "key.h"
#include "touch.h"
#include "rtc.h"
#include "stdlib.h"

#include "tkc/thread.h"
#include "platforms/common/rtos.h"

extern ret_t platform_prepare(void);
extern void systick_enable_int(void);

void system_init(void) {
	SysTick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			
	KEY_Init();
	TOUCH_Init();
	RTC_Init();
}

#include "modbus_app.h"

int main() {
  system_init();
  platform_prepare();
	
  rtos_init();
  modbus_service_start("1");
  rtos_start();
	
	return 0;
}
