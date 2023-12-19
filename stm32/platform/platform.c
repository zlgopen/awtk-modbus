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
#define TKC_HEAP_SIZE (1024 * 5)
#else
#define TKC_HEAP_SIZE (1024 * 10)
#endif/*STM32F10X_HD*/

uint32_t s_heap_mem[TKC_HEAP_SIZE];

ret_t platform_prepare(void) {
  static bool_t inited = FALSE;

  if (!inited) {
    inited = TRUE;
    tk_mem_init(s_heap_mem, sizeof(s_heap_mem));
  }

  sys_tick_init(400);

  tk_mem_init_stage2();
	
  return RET_OK;
}
