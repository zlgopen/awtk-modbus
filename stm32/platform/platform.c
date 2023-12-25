/**
 * file:   platform.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  platform dependent function of stm32
 *
 * copyright (c) 2018 - 2018 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-12 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"
#include "tkc/platform.h"
#include "platforms/common/rtos.h"

void sys_tick_init(int SYSCLK);

#ifdef STM32F10X_HD
#define CPU_FREQ 72
#elif defined(STM32F429xx)
#define CPU_FREQ 180
#elif defined(STM32H743xx)
#define CPU_FREQ 400
#else
#error "not support yet"
#endif/*STM32F10X_HD*/

ret_t platform_prepare(void) {
  static bool_t inited = FALSE;

  if (!inited) {
    inited = TRUE;
  }

  sys_tick_init(CPU_FREQ);
	
  return RET_OK;
}
