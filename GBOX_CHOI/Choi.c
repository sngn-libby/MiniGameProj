#include "device_driver.h"
#include "2440addr.h"
#include <stdlib.h>
typedef struct
{
	int day;
	int year;
	int mon;
	int date;
	int hour;
	int min;
	int sec;
}RTC_TIME;

int RTC_Get_Time(void)
{
	RTC_TIME a;
	/*
	a.day = (int)rBCDDAY;
	a.year = (int)rBCDYEAR;
	a.mon = (int)rBCDMON;
	a.date = (int)rBCDDATE;
	a.hour = (int)rBCDHOUR;
	a.min = (int)rBCDMIN;
	a.sec = (int)rBCDSEC;

	if(!a.sec)
	{
		a.day = (int)rBCDDAY;
		a.year = (int)rBCDYEAR;
		a.mon = (int)rBCDMON;
		a.date = (int)rBCDDATE;
		a.hour = (int)rBCDHOUR;
		a.min = (int)rBCDMIN;
	}

	time->day = a.day;
	time->year = ((a.year >> 4) & 0xf) * 10 + (a.year & 0xf);
	time->mon = ((a.mon >> 4) & 0xf) * 10 + (a.mon & 0xf);
	time->date = ((a.date >> 4) & 0xf) * 10 + (a.date & 0xf);
	time->hour = ((a.hour >> 4) & 0xf) * 10 + (a.hour & 0xf);
	time->min = ((a.min >> 4) & 0xf) * 10 + (a.min & 0xf);
	time->sec = ((a.sec >> 4) & 0xf) * 10 + (a.sec & 0xf);
	 */
	a.hour = (int)rBCDHOUR;
	a.min = (int)rBCDMIN;
	a.sec = (int)rBCDSEC;
	return  ((a.hour >> 4) & 0xf) * 10 + (a.hour & 0xf) * 360 + \
			((a.min >> 4) & 0xf) * 10 + (a.min & 0xf) * 60 + \
			((a.sec >> 4) & 0xf) * 10 + (a.sec & 0xf);

}

#define printf 	Uart_Printf

void User_Main(void);
void Main(void)
{
	MMU_Init();
	Led_Init();
	Key_Poll_Init();
	Uart_Init(115200);
	Timer0_Init();
	Timer3_Buzzer_Init();
	Timer4_Init();
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0xf800);

	Uart_Printf("Welcome GBOX World!\n");
	User_Main();
	Uart_Printf("Good Bye~\n");
}

#include "macro.h"

/////// Image File
#include "./Image/BASE_MAP.h"
#include "./Image/move.h"
#include "./Image/move2.h"
#include "./Image/lmove.h"
#include "./Image/lmove2.h"
#include "./Image/amove.h"
#include "./Image/amove2.h"
#include "./Image/lamove.h"
#include "./Image/lamove2.h"
#include "./Image/at1.h"
#include "./Image/at2.h"
#include "./Image/at3.h"
#include "./Image/at4.h"
#include "./Image/at5.h"
#include "./Image/at6.h"
#include "./Image/archer.h"
#include "./Image/archer2.h"
#include "./Image/BASE_MAP_OPEN.h"
#include "./Image/BASE_MAP_CLOSED.h"
#include "./Image/target.h"
#include "./Image/aat1.h"
#include "./Image/aat2.h"
#include "./Image/aat3.h"
#include "./Image/laat1.h"
#include "./Image/laat2.h"
#include "./Image/laat3.h"
#include "./Image/BAT1.h"
#include "./Image/BAT2.h"
#include "./Image/BAT3.h"
#include "./Image/BAT4.h"
#include "./Image/start.h"
#include "./Image/arrow1.h"
#include "./Image/arrow2.h"
#include "./Image/arrow3.h"
#include "./Image/arrow4.h"
#include "./Image/arrow5.h"
#include "./Image/arrow6.h"
#include "./Image/arrow7.h"
#include "./Image/arrow8.h"
#include "./Image/arrow9.h"
#include "./Image/arrow10.h"
#include "./Image/arrow11.h"
#include "./Image/arrow12.h"
#include "./Image/ItemBoxClosed.h"
#include "./Image/ItemBoxOpen.h"
#include "./Image/itemsword.h"
#include "./Image/poop.h"
#include "./Image/itemfood.h"
#include "./Image/itemfood2.h"
#include "./Image/BOSS.h"
#include "./Image/lazer.h"


//////// define
#define ARROW BLUE
#define MON 4
#define PIXEL 10
#define SIZE 30
#define TYPE 2
#define MUSICMAX 113
#define MUSICMAX2 42
#define BASE_COLOR 	BLACK
#define ITEMNUM 8
#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800
#define BCOLOR	0x6237

/////// Image array
const unsigned short * at[12] = {at6, at5, at4, at3, at2, at1,laat3,laat2,laat1,aat3,aat2,aat1};
const unsigned short * mov[8] = {lmove2, lmove, move2, move, lamove2, lamove, amove2, amove};
const unsigned short * bat[5] = {BAT1,BAT2,BAT3,BAT4};
const unsigned short * arro[12] = {arrow1,arrow2,arrow3,arrow4,arrow5,arrow6,arrow7,arrow8,arrow9,arrow10,arrow11,arrow12};
const unsigned short * itempic[ITEMNUM]={itemsword,itemfood2,itemfood2,itemfood,poop,poop,poop,poop};

/////// struct
struct Object
{
	int x;//x좌표
	int y;//y좌표
	int timer;//타이머
	int speed;//이동 거리
	int move_flag;//이동해라
	int jump;
	int jumptimer;
	int down;
	int downtimer;
	int pos[2];//0은 점프 1은 다운(posible)
	int fall;
	int rl;//1이면 오른쪽
	int attack_flag;
	int atimer;
	int life;
	int dam;
	int abs;//무적
	int weapon;//0칼 1활
	int lock;
	int target;//1~4몬스터  0 nontarget
	int arrow_flag;
	int maxlife;
	int apa;
	int open;
};
struct ty
{
	int typenum;
	int life;//1은 해골궁수, 2는 박쥐
	int dam;
	int speed;
	int step;
};
struct Monster
{
	int x;//x좌표
	int y;//y좌표
	int timer;//타이머
	int rl;//1이면 오른쪽 0 왼
	int attack_flag;
	int mon;
	int abs;
	int death;
	int type;
	int life;
	int dam;
	int speed;//이동 거리
	int cnt;
	int det;
	int apa;
	int step;
	int atimer;
	int lazer;
};
struct Item
{
	int x;
	int y;
	int type;
	int flag;
	int det;
	int open;
	int fall;
	int timer;
};
struct Arrow
{
	int x;
	int y;
	int timer;
	int speed;
	int flag;
	int dam;
	int dest[2];
	int cnt;
	int angle;
};
struct ItemType
{
	int dam;
	int life;
};
struct print
{
	unsigned short int map[76900];  // 실제 출력할 대상
};
struct Map
{
	struct print stat;//출력할 맵
	unsigned char realstat[460800];//빨간색은 1 초록색은 2 보라색 3 노란색 4
	int x;//출력할 시작 x
	int y;//출력할 시작 y
	int open;
	int stage;
	int timer;
};

/////// extern function
extern void Lcd_Draw_MAP(int x, int y, const unsigned short int *fp, unsigned short int *mp);
extern void Lcd_Set_Trans_Mode(int mode);
extern void Lcd_Show_MAP(unsigned short int *fp, unsigned short int *mp);
extern void Lcd_Draw_Char(int x, int y, const unsigned short int *fp, unsigned short int *mp,int color);
extern int Key_Get_Pressed_Multi(void);
extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;
extern void Lcd_Draw_BMP(int x, int y, const unsigned short int *fp);
extern void Buzzer_Beep_On(int tone);
extern void Buzzer_Beep_Off(void);
extern void Lcd_Draw_BOSS(int x, int y, const unsigned short int *fp, unsigned short int *tmp,unsigned short int *mp,int color);
extern void Lcd_Draw_Lazer(int x, int y, const unsigned short int *fp,unsigned short int *tmp, unsigned short int *mp,int color);

/////// function

void Lcd_Prin_Arrow(int x1, int y1, int angle);
void Lcd_Move_arrow(int *x1,int *y1,int x2,int y2,int angle);
void mapload(const unsigned short int *fp,unsigned char *mp);
void CharSetting(void);
void Monsetting(void);
void arrowsetting(void);
void itemsetting(void);
void game_setting(void);
void init_game(void);
void Draw_mon(void);
void vibration(void);
void Draw_map(void);
void Mon_move(int i);
void funcA(int r1value, int diff, int step);
void funcB(int r1value, int diff, int step);
void funcJump(void);
void funcDown(void);
void ItemFall(void);
void funcDownFallJump(void);
int Arrow_Dir_Cal(int x, int y, int destx, int desty);
void funcAttack(int key2);
void funcDetect(void);
void funcTouch(void);
void Move_Object(void);
void func_BaseAttack(struct Monster *mon);
void collision_detect(void);
void Draw_Object(void);
void end_test(void);
int GameOver_Test(void);

/////// declaration variables

struct ItemType IT[ITEMNUM]={{1,0},{0,5},{0,5},{0,10},{1,0},{0,0},{0,0},{0,0}};
struct Item item[MON]={};
struct print before;
struct ty typ[TYPE+1]={{1,10,3,1,0},{2,8,6,2,15},{3,400,20,2,5}};
struct Map map={{{0x140,0xf0,}},{0},0,0,0,1,0};
struct Object mine;
struct Object init={60,250,0,1,0,0,0,0,0,{0},1,1,0,0,100,5,0,0,0,0,0,100,0,0};
struct Arrow arrow[5]={{0,0,0,2,0,0,{0},25,0},{0,0,0,2,0,0,{0},25,0},{0,0,0,2,0,0,{0},25,0},{0,0,0,2,0,0,{0},25,0},{0,0,0,2,0,0,{0},25,0}};//내거 1개 몬스터 4마리 각각 1개
struct Monster mon[MON];
struct Arrow dearrow={0,0,0,2,0,0,{0},30,0};
int vibrat=0;
unsigned short int tempmap[158500]={0x1B8,0x168,};
const int rearr[12][2]={{-4,0},{-14,0},{-20,0},{-20,-4},{-20,-15},{-15,-20},{-4,-20},{0,-20},{0,-15},{0,-4},{0,0},{0,0}};
const int pos[4][2]={{360,170},{460,70},{570,170},{450,410}};

/////// Music variables

enum key{C0, C0_, D0, D0_, E0, F0, F0_, G0, G0_, A0, A0_, B0, C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
enum note{N16, N8, N4, N2, N1};
const int song[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2},{0xff, 0xff}};
const int song2[][2]={{B1,3},{G1,3},{D1,3},{B1,3},{G1,3},{D1,3},{B1,3},{G1,3},{D1,3},{B1,3},{G1,3},{D1,3},{0xff,0xff}};
const int song3[]={B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,A1,G1,E1,A1,G1,E1,A1,G1,E1,A1,G1,E1,A1,G1,E1,A1,G1,E1,A1,G1,E1,A1,G1,E1,C2_,A1,F1_,D2,C2_,A1,E2,C2_,A1,D2,C2_,F1_,C2_,A1,F1_,D2,C2_,F1_,D1,E1,F1_,A1,E2,D2,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,B1,G1,D1,D2,G0,D1,C1_,G1,F1_,B0,D1,C1_,A0_,E0,G0,D1,C1_,G1,F1_,B0,D1,C1_,A0_,E0};
const int song4[]={D2,E2,D2,C2,A1,G1,A1,C2,D2,C2,D2,E2,D2,C2,A1,G1,A1,C2,F2,E2,D2,E2,D2,C2,A1,G1,A1,C2,D2,C2,F2,E2,D2,E2,D2,C2,F1,C2,D2,C2,F2,E2};
int songtime3[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,24,6,6,6,6,24,6,6,6,6,24,6,6,6,6,24};
int songtime4[]={2,1,1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,1,2,2,2,1,1,2,1,1,1,1,1,1,2,2};
const int songat[]={A1,G1,E1};
int musicnt=0;
int musicnt2=0;
int onoff=0;

///////1st Function
void init_game(void)
{
	Lcd_Clr_Screen(BLACK);
	map.stage=1;
	map.open=0;
	mine=init;
	game_setting();
}

void Move_Object(void)
{
	int key = 0;
	int key1,key2,i;

	key = Key_Get_Pressed_Multi();

	map.timer=1;
	mine.timer++;
	for(i=0;i<MON;i++)
	{
		if(mon[i].timer++>mon[i].speed&&mon[i].det==1 &&mon[i].death==0)
		{
			mon[i].timer=0;
			Mon_move(i);
		}
	}
	for(i=0;i<5;i++)
	{
		if(arrow[i].flag==1)
			arrow[i].timer++;
	}

	if(mine.abs>0)
	{
		mine.abs--;
	}
	for(i=0;i<MON;i++)
	{
		if(mon[i].abs>0&&mon[i].life>0)
			mon[i].abs--;
	}

	key1=key%16;
	key2=(key%256)/16;
	if(key2==8&&mine.lock==0)
	{
		mine.lock=1;
		onoff=!onoff;
	}
	if(key2==4&&mine.lock==0)
	{
		mine.lock=1;
		mine.open=1;
	}
	if(mine.timer >= mine.speed)
	{
		mine.timer = 0;
		switch (key1)
		{
		case 2: funcA(0, (-PIXEL+2), -PIXEL); break;
		case 8: funcB(1, (10+SIZE+2), PIXEL); break;
		case 1: funcJump(); break;  //점프
		case 4: funcDown(); break;  //아래
		}
		funcDownFallJump();
		///////////////이동 끝 공격 시작///////////////////
		funcAttack(key2);
	} // Timer 끝
	/////////////////공격 끝 몬스터 인식 ////////////
	funcDetect();
	ItemFall();
	//////////////////////투사체 이동 시작/////////////////
	if(Touch_pressed) funcTouch();
	if(mine.lock==1&&key==0)
		mine.lock=0;

}

void collision_detect(void)
{
	int i,temp;
	for(i=0;i<MON;i++)
	{

		if((mine.open==1)&&(item[i].flag==1)&&(item[i].x<mine.x+20)&&(item[i].x+30>mine.x)&&(item[i].y<mine.y+20)&&(item[i].y+10>mine.y))
		{
			temp=IT[item[i].type].life;
			mine.life+=temp;
			mine.maxlife+=temp;
			mine.dam+=IT[item[i].type].dam;
			item[i].flag=0;
			item[i].open=1;
			item[i].fall=0;
			mine.open=0;
			item[i].timer=6;
		}
	}
	if(mine.abs==0)
	{
		for(i=0;i<MON;i++)
		{
			if(mon[i].mon==1&&mon[i].death==0)
			{
				if((mon[i].x<mine.x+20)&&(mon[i].x+30>mine.x)&&(mon[i].y<mine.y+20)&&(mon[i].y+10>mine.y))
				{
					mine.abs=24;
					mine.life-=mon[i].dam;
					Buzzer_Beep_Off();
					Buzzer_Beep_On(C0);
					mine.apa=4;
					vibration();
				}
			}
			if(arrow[i+1].flag==1)
			{
				if( (arrow[i+1].x>=mine.x) && (arrow[i+1].x<mine.x+30) && (mine.y<=arrow[i+1].y) && (mine.y+30>arrow[i+1].y) )
				{
					mine.abs=24;
					mine.life-=arrow[i+1].dam;
					Buzzer_Beep_Off();
					Buzzer_Beep_On(C0);
					mine.apa=4;
					vibration();
					Lcd_Prin_Arrow(arrow[i+1].x,arrow[i+1].y,arrow[i+1].angle);
					arrow[i+1].flag=0;
					mon[i].attack_flag=0;
				}
			}
			if(mon[i].lazer==1&&mon[i].life>0)
			{
				if((mine.y>=mon[i].y-15)&&(mine.y<mon[i].y+30))
				{
					mine.abs=24;
					mine.life-=mon[i].dam;
					Buzzer_Beep_Off();
					Buzzer_Beep_On(C0);
					mine.apa=4;
					vibration();
				}
			}
		}
	}
	for(i=0;i<MON;i++)
	{
		if(mine.attack_flag==1&&mon[i].abs==0&&mon[i].death==0)
		{
			func_BaseAttack(mon+i);

			if(mon[i].life<=0)
			{
				mon[i].mon=0;
				mon[i].death=1;
				item[i].flag=1;
				item[i].x=mon[i].x;
				item[i].y=mon[i].y;
			}
		}
		if(arrow[0].flag==1&&mon[i].abs==0&&mon[i].death==0)
		{
			if((arrow[0].x>=mon[i].x-10)&&(arrow[0].x<mon[i].x+40)&&(mon[i].y-10<=arrow[0].y)&&(mon[i].y+40>arrow[0].y))
			{
				mon[i].life-=arrow[0].dam;
				Buzzer_Beep_Off();
				Buzzer_Beep_On(A0);
				mon[i].abs=8;
				mon[i].apa=4;
				vibration();
				Lcd_Prin_Arrow(arrow[i+1].x,arrow[i+1].y,arrow[i+1].angle);
				arrow[0].flag=0;
				mine.arrow_flag=0;
			}
			if(mon[i].life<=0)
			{
				mine.target=0;
				mon[i].mon=0;
				mon[i].death=1;
				item[i].flag=1;
				item[i].x=mon[i].x;
				item[i].y=mon[i].y;
			}
		}
	}
	for(i=0;i<5;i++)
	{
		if(arrow[i].flag==1)
		{
			if(map.realstat[(arrow[i].y+5)*929+arrow[i].x+5+2]==1)
			{
				if(i==0)
				{
					arrow[0].flag=0;
					mine.arrow_flag=0;
				}
				else
				{
					arrow[i].flag=0;
					mon[i-1].attack_flag=0;
				}
			}
		}
	}
}

void Draw_Object(void)
{
	int i;
	Draw_map();
	Draw_mon();
	if(mine.life>=0&&mine.apa%2==0)
	{
		if(mine.attack_flag==0)
		{
			Lcd_Draw_Char(mine.x-map.x,mine.y-map.y,mov[mine.weapon*4+mine.rl*2+(mine.move_flag)],map.stat.map,BCOLOR);
			if(mine.move_flag==1)
			{
				mine.move_flag=0;
			}
		}

		else
		{
			Lcd_Draw_Char(mine.x-map.x,mine.y-map.y,at[(mine.weapon)*6+(mine.rl)*3+(--mine.atimer)],map.stat.map,BCOLOR);
			if(mine.atimer==0)
				mine.attack_flag=0;
		}
	}
	if(mine.apa>0)
	{
		mine.apa--;
	}
	for(i=0;i<5;i++)
	{
		if(arrow[i].timer>arrow[i].speed)
		{
			arrow[i].timer=0;
			if(arrow[i].flag==1)
			{
				Lcd_Move_arrow(&arrow[i].x,&arrow[i].y,arrow[i].dest[0],arrow[i].dest[1],arrow[i].angle);//화살 그리기 어케 하지
			}
			if(arrow[i].cnt--<=0)
			{
				if(i==0)
					mine.arrow_flag=0;
				else mon[i-1].attack_flag=0;
				arrow[i].flag=0;
			}
		}
		if(arrow[i].flag==1)
		{
			Lcd_Prin_Arrow(arrow[i].x,arrow[i].y,arrow[i].angle);
		}
	}
	mine.open=0;
	Lcd_Show_MAP(map.stat.map, before.map);
	before=map.stat;
}

void end_test(void)
{
	if(map.open==1 && map.realstat[(mine.y+10)*929+mine.x+10+2]==4)
	{
		map.stage++;
		CharSetting();
		game_setting();
		map.open=0;
	}
}

int GameOver_Test(void)
{
	if(mine.life<=0)
	{
		Lcd_Clr_Screen(BLACK);
		Lcd_Printf(80,100,BCOLOR,BLACK,1.5,1.5,"GAME OVER!");
		Lcd_Printf(100,120,BCOLOR,BLACK,1.5,1.5,"Stage : %2d",map.stage);
		Key_Wait_Key_Pressed();
		Lcd_Clr_Screen(BLACK);
		Lcd_Printf(90,100,BCOLOR,BLACK,1,1,"다시 하시겠습니까?");
		Lcd_Printf(80,130,BCOLOR,BLACK,1,1,"Pressed Any Key!");
		Key_Wait_Key_Pressed();
		Timer4_Delay(300);
		return 1;
	}
	return 0;
}


/////// 2nd Function
void game_setting(void)
{
	mapload(BASE_MAP, map.realstat);
	Monsetting();
	Draw_map();
	Lcd_Draw_Char(mine.x-map.x,mine.y-map.y,move,map.stat.map, BCOLOR);
	Lcd_Show_MAP(map.stat.map, before.map);
	before=map.stat;
	arrowsetting();
	itemsetting();
}

void Monsetting(void)
{
	int i,temp;
	if(map.stage%5==0)
	{
		mon[0].x=420;
		mon[0].y=60;
		mon[0].timer=0;
		mon[0].rl=1;
		mon[0].attack_flag=0;
		mon[0].mon=0;
		mon[0].abs=0;
		mon[0].death=0;
		mon[0].type=3;
		mon[0].life=typ[2].life;
		mon[0].dam=typ[2].dam;
		mon[0].speed=typ[2].speed;
		mon[0].cnt=0;
		mon[0].det=1;
		mon[0].apa=0;
		mon[0].step=typ[2].step;
		mon[0].atimer=0;
		mon[0].lazer=0;

		for(i=1;i<MON;i++)
		{
			mon[i].x=2000;
			mon[i].y=2000;
			mon[i].timer=0;
			mon[i].rl=1;
			mon[i].attack_flag=0;
			mon[i].mon=0;
			mon[i].abs=0;
			mon[i].death=1;
			temp=rand()%TYPE+1;
			mon[i].type=temp;
			mon[i].life=map.stage*typ[temp-1].life;
			mon[i].dam=map.stage*typ[temp-1].dam;
			mon[i].speed=typ[temp-1].speed;
			mon[i].cnt=0;
			mon[i].det=0;
			mon[i].apa=0;
			mon[i].step=typ[temp-1].step+map.stage;
			mon[i].atimer=0;
			mon[i].lazer=0;
		}
	}
	else
	{
		for(i=0;i<MON;i++)
		{
			mon[i].x=pos[i][0];
			mon[i].y=pos[i][1];
			mon[i].timer=0;
			mon[i].rl=1;
			mon[i].attack_flag=0;
			mon[i].mon=0;
			mon[i].abs=0;
			mon[i].death=0;
			temp=rand()%TYPE+1;
			mon[i].type=temp;
			mon[i].life=map.stage*typ[temp-1].life;
			mon[i].dam=map.stage*typ[temp-1].dam;
			mon[i].speed=typ[temp-1].speed;
			mon[i].cnt=0;
			mon[i].det=0;
			mon[i].apa=0;
			mon[i].step=typ[temp-1].step+map.stage;
			mon[i].atimer=0;
			mon[i].lazer=0;
		}
	}
}

void Draw_map(void)
{
	int temp,i,cnt=0;
	if(vibrat==6)
	{
		map.x-=5;
		vibrat--;
	}
	else if(vibrat==1)
	{
		map.x+=5;
		vibrat--;
	}
	else if(vibrat>0)
	{
		temp=(vibrat%2)?10:-10;
		if(mine.x<160) map.x = 0;
		else if(mine.x>770) map.x = 610;
		else map.x=mine.x-160;
		if(mine.y<120) map.y=0;
		else if(mine.y>320) map.y= 240;
		else map.y=mine.y-90;
		map.x+=vibrat;
		vibrat--;
	}
	else
	{
		if(mine.x<160) map.x = 0;
		else if(mine.x>770) map.x = 610;
		else map.x=mine.x-160;
		if(mine.y<120) map.y=0;
		else if(mine.y>320) map.y= 240;
		else map.y=mine.y-90;
	}
	if(map.stage%5!=0){
		for(i=0;i<MON;i++)
		{
			if(mon[i].life<=0)
			{
				cnt++;
			}
		}
		if(cnt==MON) map.open=1;
	}
	else
	{
		if(mon[0].life<=0)
		{
			map.open=1;
		}
	}
	if(map.open==0)	Lcd_Draw_MAP(map.x,map.y,BASE_MAP_CLOSED,map.stat.map);
	else if(map.open==1) Lcd_Draw_MAP(map.x,map.y,BASE_MAP_OPEN,map.stat.map);
}

void arrowsetting(void)
{
	int i;
	for(i=0;i<5;i++)
	{
		arrow[i]=dearrow;
	}
}

void itemsetting(void)
{
	int i,temp;
	for(i=0;i<MON;i++)
	{
		temp=rand()%ITEMNUM;
		item[i].x=0;
		item[i].y=0;
		item[i].type=temp;
		item[i].flag=0;
		item[i].det=0;
		item[i].open=0;
		item[i].fall=0;
		item[i].timer=0;
	}
}





void Lcd_Prin_Arrow(int x1, int y1, int angle)
{
	int rex,rey;
	rex=rearr[angle][0];
	rey=rearr[angle][1];
	int xx=x1+rex;
	int yy=y1+rey;
	if((xx<mine.x+110) && (xx>mine.x-155) && (yy>mine.y-70) && (yy<mine.y+100))
		Lcd_Draw_Char(x1+rex-map.x,y1+rey-map.y,arro[angle],map.stat.map,BCOLOR);
}

void Lcd_Move_arrow(int *x1,int *y1,int x2,int y2,int angle)
{

	int cnt=0;
	int dx,dy,e;
	dx=x2-*x1;
	dy=y2-*y1;
	if(dx>=0)
	{
		if(dy >= 0) 	// dy>=0
		{
			if(dx>=dy) 	// 1/8 octant
			{
				e=dy-dx/2;
				while(*x1<=x2&&cnt++<15)
				{
					if(e>0){*y1+=1;e-=dx;}
					*x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(*y1<=y2&&cnt++<15)
				{
					if(e>0){*x1+=1;e-=dy;}
					*y1+=1;
					e+=dx;
				}
			}
		}
		else		   	// dy<0
		{
			dy=-dy;   	// dy=abs(dy)

			if(dx>=dy) 	// 8/8 octant
			{
				e=dy-dx/2;
				while(*x1<=x2&&cnt++<15)
				{
					if(e>0){*y1-=1;e-=dx;}
					*x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(*y1>=y2&&cnt++<15)
				{
					if(e>0){*x1+=1;e-=dy;}
					*y1-=1;
					e+=dx;
				}
			}
		}
	}
	else //dx<0
	{
		dx=-dx;			//dx=abs(dx)
		if(dy >= 0) 	// dy>=0
		{
			if(dx>=dy) 	// 4/8 octant
			{
				e=dy-dx/2;
				while(*x1>=x2&&cnt++<15)
				{
					if(e>0){*y1+=1;e-=dx;}
					*x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(*y1<=y2&&cnt++<15)
				{
					if(e>0){*x1-=1;e-=dy;}
					*y1+=1;
					e+=dx;
				}
			}
		}
		else		   	// dy<0
		{
			dy=-dy;   	// dy=abs(dy)

			if(dx>=dy) 	// 5/8 octant
			{
				e=dy-dx/2;
				while(*x1>=x2&&cnt++<15)
				{
					if(e>0){*y1-=1;e-=dx;}
					*x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(*y1>=y2&&cnt++<15)
				{
					if(e>0){*x1-=1;e-=dy;}
					*y1-=1;
					e+=dx;
				}
			}
		}
	}
}

void mapload(const unsigned short int *fp,unsigned char *mp)
{
	register int width = fp[0], height = fp[1];
	register int xx, yy;
	for(yy=0;yy<height;yy++)
	{
		for(xx=0;xx<width;xx++)
		{
			if((int)fp[yy*width+xx+2]==0xf801)
				mp[yy*width+xx+2]=1;
			//			else if((int)fp[yy*width+xx+2]==0x71a9)
			//				mp[yy*width+xx+2]=3;
			else if((int)fp[yy*width+xx+2]==0x595)
				mp[yy*width+xx+2]=2;
			else if((int)fp[yy*width+xx+2]==0xffc1)
				mp[yy*width+xx+2]=4;
			else mp[yy*width+xx+2]=0;
		}
	}
}
void CharSetting(void)
{
	if(mine.x>160)
	{
		mine.x=40;
		mine.rl=1;
	}
	else if(mine.x<160)
	{
		mine.x=860;
		mine.rl=0;
	}
	mine.timer=0;
	mine.speed=1;
	mine.move_flag=0;
	mine.life+=10;
	mine.maxlife+=10;
	mine.dam+=1;
	mine.target=0;
	mine.apa=0;
	mine.open=0;
}

void vibration(void)
{
	vibrat=6;
}

void Draw_mon(void)
{
	int i;
	for(i=0;i<MON;i++)
	{
		if(mon[i].mon==1&&mon[i].death==0&&mon[i].apa%2==0)
		{
			if(mon[i].type==1)
			{
				if(mine.target==i+1)
				{
					Lcd_Set_Trans_Mode(1);
					Lcd_Draw_BOSS(mon[i].x+10-map.x,mon[i].y-map.y,target,tempmap,map.stat.map,BCOLOR);
					Lcd_Set_Trans_Mode(0);
				}
				if(mon[i].rl==1)
					Lcd_Draw_Char(mon[i].x-map.x,mon[i].y-map.y,archer,map.stat.map,BCOLOR);
				else
					Lcd_Draw_Char(mon[i].x-map.x,mon[i].y-map.y,archer2,map.stat.map,BCOLOR);
			}
			else if(mon[i].type==2)
			{
				if(mine.target==i+1)
				{
					Lcd_Set_Trans_Mode(1);
					Lcd_Draw_BOSS(mon[i].x+10-map.x,mon[i].y-map.y,target,tempmap,map.stat.map,BCOLOR);
					Lcd_Set_Trans_Mode(0);
				}
				Lcd_Draw_Char(mon[i].x-map.x,mon[i].y-map.y,bat[mon[i].cnt],map.stat.map,BCOLOR);
			}
			else
			{
				if(mon[i].lazer==1&&mon[i].life>0)
				{
					Lcd_Draw_Lazer(mon[i].x-30-map.x,mon[i].y-30-map.y,lazer,tempmap,map.stat.map,BCOLOR);
					if(mon[i].atimer>=18)
						mon[i].lazer=0;
				}
				Lcd_Draw_BOSS(mon[i].x-30-map.x,mon[i].y-30-map.y,BOSS,tempmap,map.stat.map,BCOLOR);
				if(mine.target==i+1)
				{
					Lcd_Set_Trans_Mode(1);
					Lcd_Draw_BOSS(mon[i].x-map.x,mon[i].y-map.y,target,tempmap,map.stat.map,BCOLOR);
					Lcd_Set_Trans_Mode(0);
				}

			}
		}
		if(mon[i].apa>0)
		{
			mon[i].apa--;
		}
		if(item[i].det)
		{
			if(item[i].open)
				Lcd_Draw_Char(item[i].x-map.x,item[i].y-map.y,ItemBoxOpen,map.stat.map,BCOLOR);
			else
				Lcd_Draw_Char(item[i].x-map.x,item[i].y-map.y,ItemBoxClosed,map.stat.map,BCOLOR);
		}
		if(item[i].timer>0)
		{
			Lcd_Draw_Char(item[i].x-map.x,item[i].y-map.y-30+5*item[i].timer,itempic[item[i].type],map.stat.map,BCOLOR);
			item[i].timer--;
		}
	}
}

void Mon_move(int i)
{
	if(mon[i].type==1)
	{
		if(mon[i].attack_flag==0)
		{
			mon[i].attack_flag=1;
			arrow[i+1].dam=mon[i].dam;
			arrow[i+1].x=mon[i].x+(mon[i].rl)*20;
			arrow[i+1].y=mon[i].y+15;
			arrow[i+1].dest[0]=mine.x+15;
			arrow[i+1].dest[1]=mine.y+15;
			arrow[i+1].cnt=8;
			arrow[i+1].flag=1;
			arrow[i+1].speed=3;
			arrow[i+1].angle=Arrow_Dir_Cal(arrow[i+1].x,arrow[i+1].y,arrow[i+1].dest[0],arrow[i+1].dest[1]);
		}
		if(mine.x>mon[i].x)
			mon[i].rl=1;
		else
			mon[i].rl=0;
	}
	else if(mon[i].type==2)
	{
		if(++mon[i].cnt>=4)
			mon[i].cnt=0;
		if(mine.x+mon[i].step<mon[i].x)
		{
			mon[i].x-=mon[i].step;
		}
		else if(mon[i].x+mon[i].step<mine.x)
		{
			mon[i].x+=mon[i].step;
		}
		if(mine.y<mon[i].y)
		{
			mon[i].y-=mon[i].step;
		}
		else if(mine.y+30>mon[i].y)
		{
			mon[i].y+=mon[i].step;
		}
	}
	else
	{
		if(mine.x+mon[i].step<mon[i].x)
		{
			mon[i].x-=mon[i].step;
		}
		else if(mon[i].x+mon[i].step<mine.x)
		{
			mon[i].x+=mon[i].step;
		}
		if(mine.y<mon[i].y)
		{
			mon[i].y-=mon[i].step;
		}
		else if(mine.y+30>mon[i].y)
		{
			mon[i].y+=mon[i].step;
		}
		///이동 끝
		if(mon[i].atimer<4)
		{
			arrow[mon[i].atimer+1].dam=mon[i].dam;
			arrow[mon[i].atimer+1].x=mon[i].x+(mon[i].rl)*20;
			arrow[mon[i].atimer+1].y=mon[i].y+15;
			arrow[mon[i].atimer+1].dest[0]=mine.x+15;
			arrow[mon[i].atimer+1].dest[1]=mine.y+15;
			arrow[mon[i].atimer+1].cnt=8;
			arrow[mon[i].atimer+1].flag=1;
			arrow[mon[i].atimer+1].speed=3;
			arrow[mon[i].atimer+1].angle=Arrow_Dir_Cal(arrow[i+1].x,arrow[i+1].y,arrow[i+1].dest[0],arrow[i+1].dest[1]);
		}//화살 1방씩
		if(mon[i].atimer%16==0)
		{
			mon[i].lazer=1;
		}

		mon[i].atimer++;
		if(mon[i].atimer>=32)
		{
			mon[i].atimer=0;
		}
	}
}

void funcA(int r1value, int diff, int step)
{
	mine.rl=r1value;
	if(map.realstat[mine.y*929+mine.x+diff]==0/*||map.realstat[mine.y*929+mine.x-PIXEL+2]==3*/)
		mine.x-=PIXEL;
	else if(map.realstat[mine.y*929+mine.x+diff]==4 && map.open==1)
		mine.x-=PIXEL;
	if(mine.x<0)
		mine.x=10;
	mine.move_flag = 1;
}

void funcB(int r1value, int diff, int step)
{
	mine.rl=r1value;
	if(map.realstat[mine.y*929+mine.x+diff]==0/*||map.realstat[mine.y*929+mine.x-PIXEL+2]==3*/)
		mine.x+=PIXEL;
	else if(map.realstat[mine.y*929+mine.x+diff]==4 && map.open==1)
		mine.x+=PIXEL;
	if(mine.x>960)
		mine.x=920;
	mine.move_flag = 1;
}

void funcJump(void)
{
	if(mine.pos[0]==0)
	{
		mine.jump=1;
		mine.fall=1;
		mine.jumptimer=10;
		mine.pos[0]=1;
	}
	mine.move_flag = 1;
}

void funcDown(void)
{
	if(mine.pos[1]==0 && mine.down==0)
	{
		if(map.realstat[(mine.y+SIZE)*929+mine.x+2]==2)
		{
			if(map.realstat[(mine.y+SIZE)*929+mine.x+SIZE+2]==2||map.realstat[(mine.y+SIZE)*929+mine.x+SIZE+2]==0)
			{
				mine.down=1;
				mine.downtimer=4;
				mine.pos[1]=1;
				mine.fall=0;
			}
		}
		else if(map.realstat[(mine.y+SIZE)*929+mine.x+2]==0&&map.realstat[(mine.y+SIZE)*929+mine.x+SIZE+2]==2)
		{
			mine.down=1;
			mine.downtimer=4;
			mine.pos[1]=1;
			mine.fall=0;
		}
	}
	mine.move_flag = 1;
}

void ItemFall(void)
{
	int i;
	for(i=0;i<MON;i++)
	{
		if(item[i].fall==0)
		{
			if(map.realstat[(item[i].y+SIZE)*929+item[i].x+2]==0&&map.realstat[(item[i].y+SIZE)*929+item[i].x+SIZE+2]==0/*||map.realstat[(mine.y+SIZE)*929+mine.x+2]==3*/)
				item[i].y+=PIXEL;
			else
			{
				item[i].fall=1;
			}
		}
	}
}

void funcDownFallJump(void)
{
	int temp1,temp2;

	if(mine.down==1)  //  key1과 상관 없음
	{
		if(--mine.downtimer==0)
		{
			mine.down=0;
			mine.pos[1]=0;
		}
	}
	if(mine.jump==1)  // key1과 상관 없음
	{
		if((map.realstat[(mine.y-PIXEL)*929+mine.x+2]!=1)&&(map.realstat[(mine.y-PIXEL)*929+mine.x+2+SIZE-2-1]!=1))
		{
			mine.y-=PIXEL;
		}
		if(--mine.jumptimer==0)
		{
			mine.jump=0;
			mine.fall=1;
		}
		mine.move_flag = 1;
	}

	if(mine.fall==0)
	{
		if(map.realstat[(mine.y+SIZE)*929+mine.x+2]==0&&map.realstat[(mine.y+SIZE)*929+mine.x+SIZE+2]==0/*||map.realstat[(mine.y+SIZE)*929+mine.x+2]==3*/)
			mine.y+=PIXEL;
		else if(mine.down==1)
		{
			mine.y+=PIXEL;
		}
		else
		{
			mine.fall=1;
			mine.pos[0]=0;
		}
		mine.move_flag = 1;
	}
	else if(mine.jump==0)
	{
		temp1=map.realstat[(mine.y+SIZE)*929+mine.x+2];
		temp2=map.realstat[(mine.y+SIZE)*929+mine.x+SIZE];
		if(temp1==0 && temp2==0) mine.fall=0;
	}
}

int Arrow_Dir_Cal(int x, int y, int destx, int desty)
{
	double temp1,temp2,temp;
	if(destx==x)
	{
		if(desty>y)
			return 0;
		else return 6;
	}
	else if(desty==y)
	{
		if(destx>x)
			return 3;
		else return 9;
	}
	else
	{
		temp1=destx-x;
		temp2=desty-y;
		temp=temp1/temp2;
		if(temp1>0)
		{
			if(temp2>3.732*temp1)
				return 6;
			else if(temp2>temp1)
				return 5;
			else if(temp2>0.2679*temp1)
				return 4;
			else if(temp2>(-0.2679)*temp1)
				return 3;
			else if(temp2>-temp1)
				return 2;
			else if(temp2>(-3.732)*temp1)
				return 1;
			else return 0;
		}
		else
		{
			if(temp2>-3.732*temp1)
				return 6;
			else if(temp2>-temp1)
				return 7;
			else if(temp2>-0.2679*temp1)
				return 8;
			else if(temp2>0.2679*temp1)
				return 9;
			else if(temp2>temp1)
				return 10;
			else if(temp2>3.732*temp1)
				return 11;
			else return 0;
		}
	}

}

void funcAttack(int key2)
{
	int temp,temp2;
	if(key2==1)
	{
		//	printf("%d %d %d %d %d %d\n",mine.x,mine.y,map.open,mon[1].life,mon[2].life,mon[3].life);
		if(mine.weapon==0)
		{
			if(mine.attack_flag==0)
			{
				mine.attack_flag=1;
				mine.atimer=3;
			}
		}
		else if(mine.lock==0 && mine.target>0)
		{
			mine.lock=1;
			temp=mine.arrow_flag;
			if(mine.x<mon[mine.target-1].x)
				temp2=1;
			else temp2=0;
			if(temp==0&&temp2==mine.rl)
			{
				arrow[temp].dam=mine.dam;
				arrow[temp].x=mine.x+(mine.rl)*20;
				arrow[temp].y=mine.y+15;
				arrow[temp].dest[0]=mon[mine.target-1].x+15;
				arrow[temp].dest[1]=mon[mine.target-1].y+15;
				arrow[temp].cnt=8;
				arrow[temp].flag=1;
				arrow[temp].angle=Arrow_Dir_Cal(arrow[temp].x,arrow[temp].y,arrow[temp].dest[0],arrow[temp].dest[1]);
				mine.arrow_flag++;
				mine.attack_flag=1;
				mine.atimer=3;
			}
		}
	}

	if(mine.lock==0 && key2==2)
	{
		mine.weapon = !mine.weapon;
		mine.lock=1;
	}
}

void funcDetect(void)
{
	int i;
	for(i=0;i<MON;i++)
	{
		if(mon[i].det==0&&mon[i].death==0)
		{
			if(mon[i].type==3)
			{
				if((mon[i].x<=mine.x+200)&&(mon[i].x>=mine.x-230))
				{
					if((mon[i].y<=mine.y+170)&&(mon[i].y>=mine.y-140))
						mon[i].det=1;
				}
			}
			else
			{
				if((mon[i].x<=mine.x+160)&&(mon[i].x>=mine.x-190))
				{
					if((mon[i].y<=mine.y+130)&&(mon[i].y>=mine.y-100))
						mon[i].det=1;
				}
			}
		}
		if(mon[i].mon==0 && mon[i].death==0)
		{
			if((mon[i].x<=mine.x+100)&&(mon[i].x>=mine.x-130))
			{
				if((mon[i].y<=mine.y+90)&&(mon[i].y>=mine.y-60))
					mon[i].mon=1;
			}
		}
		else
		{
			if((mon[i].x>mine.x+130)||(mon[i].x<mine.x-160))
				mon[i].mon=0;
			if((mon[i].y>mine.y+120)||(mon[i].y<mine.y-90))
				mon[i].mon=0;
		}
		if(item[i].flag||item[i].open)
		{
			if((item[i].x<=mine.x+100)&&(item[i].x>=mine.x-130))
			{
				if((item[i].y<=mine.y+90)&&(item[i].y>=mine.y-60))
				{
					item[i].det=1;
					item[i].fall=0;
				}
				else item[i].det=0;
			}
			else item[i].det=0;
		}
	}
}

void funcTouch(void)
{
	int temp1, temp2;
	int i;

	mine.lock=1;
	temp1=Touch_x+map.x;
	temp2=Touch_y+map.y;
	//		Uart_Printf("Pressed: X=%u,Y=%u, target = %d, %d\n", temp1, temp2, mine.target,mon[0].mon);
	for(i=0;i<MON;i++)
	{
		if(mon[i].mon==0) continue;
		if(mon[i].type==3)
		{
			if((temp1>=mon[i].x-SIZE)&&(temp1<mon[i].x+2*SIZE)&&(temp2>=mon[i].y-SIZE)&&(temp2<mon[i].y+2*SIZE))
			{
				mine.target = (mine.target!=(i+1)) ? (i+1) : 0;
			}
			continue;
		}
		if((temp1>=mon[i].x)&&(temp1<mon[i].x+SIZE)&&(temp2>=mon[i].y)&&(temp2<mon[i].y+SIZE))
		{
			mine.target = (mine.target!=(i+1)) ? (i+1) : 0;
		}

	}
}

void func_BaseAttack(struct Monster *mon)
{
	int range = 0;
	int mineX = mine.x;
	int mineY = mine.y;
	int monX = mon->x;
	int monY = mon->y;

	if(mine.rl==1)
	{
		if((mineX+60 > monX) && (mineX < (monX+30)) && ( monY < (mineY+30)) && (monY > mineY-10))
		{
			range = 1;
		}
	}
	else
	{
		if((mineX-60 < monX) && (mineX > monX) && (monY < mineY+30) && (monY > mineY-10))
		{
			range = 1;
		}
	}
	if (range == 1)
	{
		mon->life-=mine.dam;
		Buzzer_Beep_Off();
		Buzzer_Beep_On(A0);
		mon->apa=4;
		mon->abs=8;
		vibration();
	}
}


///////Main Function and Music
void User_Main(void)
{

	int cnt=0;

	srand(RTC_Get_Time());

	Timer0_Repeat(10);
	Touch_ISR_Enable(1);

start:
	Lcd_Draw_BMP(0,0,start);
	init_game();

	for(;;)
	{
		if(Timer0_Check_Expired())
		{
			cnt++;
			if(cnt%2==0)
			{
				Move_Object();
				collision_detect();
				if(map.timer==1)
				{
					Draw_Object();
				}
			}
		}
		end_test();

		if(GameOver_Test())  //  안좋은 방법입니당
		{
			goto start;
		}
	}

}
