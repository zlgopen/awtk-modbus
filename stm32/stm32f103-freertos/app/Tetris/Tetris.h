#ifndef __TETRIS__H
#define __TETRIS__H
#include "system.h"

/*************����˹������Ϸ����*****************/
#define BPIXEL 10	//һ��С����߳�������ֵ
#define	X_BOXS 14	//��Ϸ�װ�X��С�������
#define Y_BOXS 25	//��Ϸ�װ�Y��С�������
#define SHAPE_COLOR 0x0000	//ͼ����ɫ->��ɫ
#define BOXS_COLOR	0X07FF	//�װ���ɫ->��ɫ
#define WILL_COLOR	0x7D7C	//�߽���ɫ->ǳ��ɫ
#define TEXT_COLOR	0x0000	//������ɫ->��ɫ

#define LEFT 	34
#define	RIGHT 	194
#define DOWN	168
#define TURN	98
#define RESET 	162
extern u8 score_buf[7],level_buf[4];
extern u16 BoxSR[Y_BOXS+4];
typedef struct
{
	u16	x;
	u16 y;
	u16 color;
	u8  CurNum;	//��ǰͼ�α��
	u8	NextNum;//��һ��ͼ�α��
	u8	TurnNum;//��תͼ�α��
	u8 	Move;
	
}_Shape;
extern _Shape NewShape;

typedef struct
{
	u16 speed;
	u8  level;
	u32 score;
}_Game;
extern _Game Game;

//void TIM4_Init(u16 arr,u16 psc);	//���ƶ���˹��������������ٶ�
void Show_TetrisFace(void);	//��ʾ����˹������Ϸ������
void Show_NextShape(u16 sx,u16 sy,u16 color);	//��ʾ��һ������ͼ��
void Start_Game(void);	//��ʼ��Ϸ
void LCD_DrawBlock(u16 sx,u16 sy,u16 color);	//LCD��С���麯��
void LCD_DrawShape(u16 sx,u16 sy,u8 n,u16 color);	//LCD��ͼ�κ���
void LCD_ClearShape(u16 sx,u16 sy,u8 n,u16 color);//���ͼ�κ���
u8 Judge(u16 sx,u16 sy,u8 n,u8 mode);
void Create_Shape(void);

void MoveLeft(void);
void MoveRight(void);
void DownFast(void);
void Transform(void);
void DownFree(void);
void ResetGame(void);

#endif
