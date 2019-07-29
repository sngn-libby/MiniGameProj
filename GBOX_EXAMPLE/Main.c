/***********************************************************/
// ������ ����� �Լ��� ȣ���ϴ� ��ƾ�� ������ ����!
/***********************************************************/
#if 01

#include <stdlib.h>
#include "device_driver.h"
#include "2440addr.h"
//#include "myCharacter.c"

// out
#define printf 	Uart_Printf

// colour
#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800

#define BACK_COLOR 	BLACK
#define WIDTH 		320
#define HEIGHT 		240

unsigned short make_pixel(int r, int g, int b)
{
	return (r>>3)<<11 | (g>>3)<<6 | (b>>3) << 1 ;
}


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


/*------------------------------------------------------------------------------*/

typedef struct str{
	int x, y;
}POS;
typedef struct size{
	int w, h;
}SIZE;

// monster �����
typedef struct obj
{
	int timer;
	int move_flag;

	POS pos;
	POS init_pos;
	POS old_pos;
	SIZE size;
	unsigned short color;
	int speed_step;
	int move_step;
	int attack_flag;
}OBJ;

OBJ Player={0,1,{160,120},{160,120},{160,120},{59,38}, RED, 3, 8, 0 };
OBJ Zombie={0,1,{160,120},{160,120},{160,120},{59,38}, RED, 3, 8, 0 };

/*------------------------------------------------------------------------------*/

void Objects(void);
//void Map(void);
void Game_Init(void);
void Map_init(void);
void Map_move(int id);

/*===============================================================================*/

void Main(void)
{

	int i;
	int map_id = 0;

	MMU_Init();
	Led_Init();
	Key_Poll_Init();
	Uart_Init(115200);
	Timer0_Init();
	Timer3_Buzzer_Init();
	Timer4_Init();
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0x0000);

	Uart_Printf("Welcome Zombie Escape Game\n");


	for(i=0;;i++)
	{
		if(Player.pos.x>=LCD_XSIZE) map_id++;
		Map_move(map_id);
		Objects();
	}
//	Objects();

}

/*===============================================================================*/

#include "./Image/surgery.h"
#include "./Image/road.h"

//void Map_init(void);

const unsigned short * Maps[] = {surgery, road};

//void Map(void)
//{
//	Map_init();
//
//}
/*
	void Lcd_Select_Buffer_Mode(unsigned int mode);
 	void Lcd_Set_Virtual_Display_Start(int x, int y);
	void Lcd_Select_Draw_Frame_Buffer(unsigned int id)
	void Lcd_Select_Display_Frame_Buffer(unsigned int id)
	void Lcd_Graphic_Init(void)
 */

void Map_init(void)
{
	int xtmp, ytmp;

	Lcd_Get_Info_BMP(&xtmp, &ytmp, Maps[0]);
	Lcd_Draw_BMP(0, 0, Maps[0]);
	//Timer4_Delay(1000);

}

void Map_move(int id)
{
	Lcd_Select_Buffer_Mode(VIRTUAL_MODE);

	Lcd_Set_Virtual_Display_Start(LCD_XSIZE*id,LCD_YSIZE*id);
	Lcd_Select_Draw_Frame_Buffer(id);
	Lcd_Select_Display_Frame_Buffer(id);
	//Lcd_Graphic_Init();
}
/*===============================================================================*/

#include "./Image/man_01.h"
#include "./Image/man_04.h"

const unsigned short * Man[] = {man_01,man_04};

void Character_init(void);
void Draw_Character(void);
int Character_move(void);
void Monster_init(void);
void Monster_move(void);
void explosion(void);

/*------------------------------------------------------------------------------*/

void Objects(void)
{
	Character_init();
	Draw_Character();
	Monster_init();

	for(;;)
	{
		if(Character_move()){
			Character_move();
			Draw_Character();
		}
	}
}



void Game_Init(void)
{
	Lcd_Clr_Screen(BACK_COLOR);
	Timer0_Repeat(20);
}

void Character_init(void)
{
//	int xtmp, ytmp;
//
//	Lcd_Get_Info_BMP(&xtmp, &ytmp, man_04);
//	Uart_Printf("X=[%d], Y=[%d]\n", xtmp, ytmp);
//	Uart_Printf("X=[%d], Y=[%d]\n", Player.pos.x,  Player.pos.y);
//	Lcd_Draw_BMP((Player.pos.x-(xtmp/2)), (Player.pos.y-(ytmp/2)), man_04);
//	Timer4_Delay(1000);
//
//	for(i=0; i<(sizeof(Man)/sizeof(Man[1])); i++)
//	{
//		Lcd_Get_Info_BMP(&xtmp, &ytmp, Man[1]); // �׸�ũ�⼳��
//		Uart_Printf("X=[%d], Y=[%d]\n", xtmp, ytmp);
//		Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), Man[]); // �ҷ��� ��ġ ����
//		Timer4_Delay(1000);
//	}
}


void Draw_Character(void)
{
	if((Player.pos.x >= WIDTH)&&(Player.pos.y >= HEIGHT))
	{
		Player.pos = Player.old_pos;
	}

	if(Player.move_flag)
	{
		Lcd_Draw_Bar(Player.old_pos.x, Player.old_pos.y, Player.old_pos.x+Player.size.w, Player.old_pos.y+Player.size.h, BACK_COLOR);
		Lcd_Draw_Bar(Player.pos.x, Player.pos.y, Player.pos.x+Player.size.w, Player.pos.y+Player.size.h, Player.color);
		Player.move_flag = 0;

		Uart_Printf("[move] x, y: %d %d", Player.pos.x, Player.pos.y);
	}
}



int Character_move(void)
{
	int key = 0;
	int dy[] = {-1, 0, 1, 0};
	int dx[] = {0, -1, 0, 1};

	if(Key_Get_Pressed())
	{
		key = Key_Get_Pressed()-1;
		Player.move_flag = 1;

		Player.old_pos = Player.pos;
		Player.pos.x += dx[key]*Player.speed_step;
		Player.pos.y += dy[key]*Player.speed_step;
		return 1;
	}
	else if(Key_Get_Pressed()==9)
	{
		Uart_Printf("%d\n", Key_Get_Pressed());
		return 0;
	}

	return 0;
}


void explosion(void)
{
	/*
	 if(Ufo_beam.cd_flag == 1 || Gun.cd_flag == 1)
	{
		Lcd_Draw_Bar(Gun.pos_back[0], Gun.pos_back[1]-40, Gun.pos_back[0] + 30, Gun.pos_back[1], WHITE);
		Timer4_Delay(100);
		Lcd_Draw_Bar(Gun.pos_back[0], Gun.pos_back[1]-40, Gun.pos_back[0] + 30, Gun.pos_back[1], BLUE);
		Timer4_Delay(100);
		Lcd_Draw_Bar(Gun.pos_back[0], Gun.pos_back[1]-40, Gun.pos_back[0] + 30, Gun.pos_back[1], RED);
		Timer4_Delay(100);
		Lcd_Draw_Bar(Gun.pos_back[0], Gun.pos_back[1]-40, Gun.pos_back[0] + 30, Gun.pos_back[1], BLACK);
	}
	 */
}

void Monster_init(void)
{
	//int i;
}
void Monster_move(void)
{
	int key = 0;
	if(Timer0_Check_Expired())
	{
		Zombie.timer++;
	}
}

#endif

