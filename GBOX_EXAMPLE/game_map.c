//#include <stdlib.h>
//#include "device_driver.h"
//#include "2440addr.h"
//
//#define N (10)
//
//typedef struct str{
//	int x, y;
//	int old_x, old_y;
//	int init_x, init_y;
//}POS;
//
//// monster 만들기
//typedef struct obj
//{
//	int timer;
//	int move_flag;
//	POS pos;
//	short w, h;
//	unsigned short color;
//	int speed_step;
//	int move_step;
//	int attack_flag;
//}OBJ;
//
//OBJ Player = { 0, 0, { 130, 100, 130, 100, 130, 100 }, 0, 0, BLUE, 3, 8, 0 };
//OBJ Zombie[N] = { { 0, 1, { 350, 120, 350, 120, 350, 120 }, 30, 30, RED, 1, 3, 0 } };
//POS map_pos = { 0, 0, 0, 0, 0, 0 };
//
//OBJ Bullet = { 0, };
//
//void Call_Map(const unsigned short int *fp, const unsigned short int *map)
//{
//	// fp : 전체 맵
//	register int xx, yy;
//	register int height = fp[0], width=fp[1];
//
//	for(yy=0;yy<height;yy++)
//	{
//		for(xx=0;xx<width;xx++)
//		{
//			if((Player.pos.x>160)&&(Player.pos.x<320))
//				Lcd_Put_Pixel(xx+160,yy,(int)map[yy*width+xx]);
//			else if((Player.pos.x>320)&&(Player.pos.x<480))
//				Lcd_Put_Pixel(xx+320,yy,(int)map[yy*width+xx]);
//			else if((Player.pos.x>480)&&(Player.pos.x<640))
//				Lcd_Put_Pixel(xx+480,yy,(int)map[yy*width+xx]);
//			//Lcd_Draw_BMP()
//		}
//	}
//}
//
//void Erase_Map(int x, int y, int w, int h, const unsigned short int *fp)
//{
//	register int map_w = fp[0];
//	register int i, j;
//
//	for (i = y; i<y + h; i++)
//	{
//		for (j = x; j<x + w; j++)
//		{
//			Lcd_Put_Pixel(j, i, (int)fp[map_w*i + j + 2]);
//		}
//	}
//}
