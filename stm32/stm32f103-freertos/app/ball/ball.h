#ifndef _ball_H
#define _ball_H
#include "system.h"
#include "SysTick.h"


struct BALL
{
	u8 dir;  //����		1:���Ͻ� 2�����Ͻ� 3�����½� 4�����½�
	u8 x;  //x����
	u16 y;  //y����
};
extern struct BALL ball;

struct PAI
{
	u8 dir;	  //����X����
	int x;	  //����Y����
	u16 y;	  //���ķ���  1���� 2���� 3����ֹ
};
extern struct PAI pai;

struct GAME
{
	u8 life;	  //����
	u16 score;	  //����
	u8 sco;   //�÷ֱ�־
};
extern struct GAME game;
extern u8 button;
void GUI_DotP(u16 x,u16 y,u16 color);
void GUI_DotB(u16 x,u16 y,u16 color);
void game_init_show(void);	  //��Ϸ��ʼ����ʾ
void ball_play(void);	  //������ʾ
void LCD_ShowPictureEx(u16 x, u16 y, u16 wide, u16 high);
#endif
