#include "SysTick.h"

static u8 fac_us = 0;  // us��ʱ������
static u16 fac_ms = 0; // ms��ʱ������

void sys_tick_init(uint32_t noused) {
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD = fac_ms;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

//��ʼ���ӳٺ���
// SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
// SYSCLK:ϵͳʱ��Ƶ��
void SysTick_Init(void) {
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SystemCoreClock / 8000000;
  fac_ms = (u16)fac_us * 1000;
}

//��ʱnus
// nusΪҪ��ʱ��us��.
void delay_us(u32 nus) {
  u32 temp = 0;
  SysTick->LOAD = nus * fac_us; //ʱ�����
  SysTick->VAL = 0x00;          //��ռ�����
  do {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
}

//��ʱnms
//ע��nms�ķ�Χ
// SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms) {
  u32 temp = 0;
  SysTick->LOAD = (u32)nms * fac_ms; //ʱ�����(SysTick->LOADΪ24bit)
  SysTick->VAL = 0x00;               //��ռ�����
  do {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
}