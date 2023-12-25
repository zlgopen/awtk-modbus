
#include "tkc/mem.h"
#include "../FreeRTOS/Source/portable/MemMang/heap_4.c"

/*保证FreeRTOS提供的函数。*/

ret_t os_mem_init(void) {
  prvHeapInit();
  return RET_OK;
}

void* os_mem_alloc(size_t size) {
  return pvPortMalloc(size);
}

void os_mem_free(void* ptr) {
  vPortFree(ptr);
}

void* tk_mem_realloc(void* pv, size_t size) {
  uint8_t* puc = (uint8_t*)pv;
  BlockLink_t* pxLink;

  if (pv != NULL) {
    puc -= xHeapStructSize;
    pxLink = (void*)puc;

    if (pxLink->xBlockSize >= size) {
      return pv;
    } else {
      void* new_ptr = pvPortMalloc(size);
      if (new_ptr != NULL) {
        memcpy(new_ptr, pv, pxLink->xBlockSize);
        vPortFree(pv);
      }
      return new_ptr;
    }
  } else {
    return pvPortMalloc(size);
  }
}

void* os_mem_calloc(size_t nmemb, size_t size) {
  void* ptr = pvPortMalloc(nmemb * size);
  if (ptr != NULL) {
    memset(ptr, 0x00, nmemb * size);
  }
  return ptr;
}

/*************TKC 内存管理函数****************/

/*FreeRTOS会在第一次使用时初始化，本函数无需调用*/
ret_t tk_mem_init(void* buffer, uint32_t size) {
  return os_mem_init();
}

void* tk_calloc(uint32_t nmemb, uint32_t size, const char* func, uint32_t line) {
  return os_mem_calloc(nmemb, size);
}

void* tk_realloc(void* ptr, uint32_t size, const char* func, uint32_t line) {
  return tk_mem_realloc(ptr, size);
}

void* tk_alloc(uint32_t size, const char* func, uint32_t line) {
  return os_mem_alloc(size);
}

void tk_free(void* ptr) {
  os_mem_free(ptr);
}

void tk_mem_dump(void) {
  return;
}

/**平台SDK的内存管理函数**/
void myfree(uint8_t memx, void* ptr) {
  tk_free(ptr);
}

void* mymalloc(uint8_t memx, uint32_t size) {
  return tk_alloc(size, __FUNCTION__, __LINE__);
}

void* myrealloc(uint8_t memx, void* ptr, uint32_t size) {
  return tk_realloc(ptr, size, __FUNCTION__, __LINE__);
}
