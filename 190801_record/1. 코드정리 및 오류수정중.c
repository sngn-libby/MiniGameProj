/***********************************************************/
// 다음은 사용자 함수를 호출하는 루틴임 지우지 말것!
/***********************************************************/
#if 01

#include <stdlib.h>
#include "device_driver.h"
#include "2440addr.h"
//#include "myCharacter.c"

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

	a.hour = (int)rBCDHOUR;
	a.min = (int)rBCDMIN;
	a.sec = (int)rBCDSEC;
	return  ((a.hour >> 4) & 0xf) * 10 + (a.hour & 0xf) * 360 + \
		((a.min >> 4) & 0xf) * 10 + (a.min & 0xf) * 60 + \
		((a.sec >> 4) & 0xf) * 10 + (a.sec & 0xf);

}

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
	return (r >> 3) << 11 | (g >> 3) << 6 | (b >> 3) << 1;
}

// available map area condition
#define AREA_FLAG ((((Player.pos.x>160) && (Player.pos.x<690)) || ((Player.pos.y>120) && (Player.pos.y < 360))) ? 1:0)
//#define AREA_FLAG_1 (((Player.pos.x>85) && (Player.pos.x<160)) || ((Player.pos.x>690) && (Player.pos.x<765))) ? 1:0
//#define AREA_FLAG_1_REV ((Player.pos.x>160) && (Player.pos.x<690)) ? 1:0
//#define AREA_FLAG_2 (((Player.pos.y>50) && (Player.pos.y < 120)) || ((Player.pos.y>360) && (Player.pos.y < 420))) ? 1:0
//#define AREA_FLAG_2_REV ((Player.pos.y>120) && (Player.pos.y < 360)) ? 1:0
#define AREA_FLAG_1 (((Player.pos.y>120) && (Player.pos.y<(360-Player.h))) ? 1:0)
#define AREA_FLAG_2 (((Player.pos.x>160) && (Player.pos.x<(690-Player.w))) ? 1:0)
#define MAP_AREA_FLAG ((((Player.pos.x>160) && (Player.pos.x<(Back.w - 160 - Player.w))) && ((Player.pos.y>120) && (Player.pos.y < (Back.h - 120 - Player.h)))) ? 1:0)
#define PLY_AREA_FLAG (((Player.pos.x>85) && (Player.pos.x<(765 - Player.w)) && (Player.pos.y>50) && (Player.pos.y < (420 - Player.h))) ? 1:0)
#define ZB_AREA_FLAG(I) (((Zombie[(I)].pos.x>85) && (Zombie[(I)].pos.x< (765 - Zombie[(I)].w))\
						&& (Zombie[(I)].pos.y>50) && (Zombie[(I)].pos.y < (420 - Zombie[(I)].h))) ? 1 : 0)
#define AREA_CHECK(obj) (((obj.pos.x>85) && (obj.pos.x< (765 - obj.w))\
						&& (obj.pos.y>50) && (obj.pos.y < (420 - obj.h))) ? 1 : 0)
#define AREA_CHECK_X(X) (((X).pos.x>50) && ((X).pos.x<765-(X).w))
#define AREA_CHECK_Y(Y) (((Y).pos.y>50) && ((Y).pos.y<420-(Y).h))
#define ABS(x) ((x)<0 ? -(x) : (x))




#include "./Image/surgery_480.h"
#include "./Image/road.h"

// Player
#define PLY_SPEED (3)
#define COOLTIME (20)

#include "./Image/man_01.h"
#include "./Image/man_02.h"
#include "./Image/man_03.h"
#include "./Image/man_04.h"

// Bullet
#define BL_SPEED (6)
//#define CRASH_X(I) ((Bullet.pos.x >= Zombie[(I)].pos.x) && (Bullet.pos.x<= Zombie[(I)].pos.x+Zombie[(I)].w)) ? 1:0
//#define CRASH_Y(I) ((Bullet.pos.y >= Zombie[(I)].pos.y) && (Bullet.pos.y<= Zombie[(I)].pos.y+Zombie[(I)].h)) ? 1:0

#include "./Image/bullet_01_0.h"
#include "./Image/bullet_01_1.h"
#include "./Image/bullet_01_2.h"
#include "./Image/bullet_01_3.h"

// Zombie
//#define ACCEL(i) (Zombie[i].move_flag) ? (Zombie[i].speed_step * (i)) : 0
#define N (3)
#define DIST (50)
#define ZB_SPEED (2)

#include "./Image/zombie_01_0.h"
#include "./Image/zombie_01_1.h"
#include "./Image/zombie_01_2.h"
#include "./Image/zombie_01_3.h"

#include "./Image/zombie_02_0.h"
#include "./Image/zombie_02_1.h"
#include "./Image/zombie_02_2.h"
#include "./Image/zombie_02_3.h"

#include "./Image/zombie_03_0.h"
#include "./Image/zombie_03_1.h"
#include "./Image/zombie_03_2.h"
#include "./Image/zombie_03_3.h"

const unsigned short * Maps[] = { surgery_480, road };
const unsigned short * Man[] = { man_01, man_02, man_03, man_04 };
const unsigned short * Bullets[] = { bullet_01_0, bullet_01_1, bullet_01_2, bullet_01_3 };
const unsigned short * arr_Zombie[] = { zombie_01_0, zombie_02_0, zombie_03_0\
, zombie_01_1, zombie_02_1, zombie_03_1\
, zombie_01_2, zombie_02_2, zombie_03_2\
, zombie_01_3, zombie_02_3, zombie_03_3 };

/*------------------------------------------------------------------------------*/

void Game_In(void);

void Game_Init(void);
void Call_Map(int x, int y, int w, int h, const unsigned short int *fp);
void Map_init(void);
void Player_init(void);
void Zombie_init(void);

void Move(void);
void Move_Player(int key);
void Move_Map(int key);
void Move_Zombie(void);
void Move_Bullet(int key); // Draw도 같이 발생
void Draw_Player(int key);
void Draw_Map(int key);
void Draw_Zombie(void);
void explosion(void);
int Detect_Crash(void);
void Detect_Zombie(void);
//void Run_Zombie(void);



void Erase_Map(int x, int y, int w, int h, const unsigned short int *fp);

int Count_Zombie(void);
void Game_Out(int ver);

/*------------------------------------------------------------------------------*/

typedef struct str{
	int x, y;
	int old_x, old_y;
	int init_x, init_y;
}POS;

// Zombie 만들기
typedef struct obj
{
	int timer;
	int move_flag;
	POS pos;
	short w, h;
	unsigned short color;
	int speed_step;
	int move_step;
	int attack_flag;
	int life_flag;
	int time_step;
	int weapon_mode;
}OBJ;

typedef struct mp
{
	POS pos;
	short w, h;
	short map_num;
}MP;

OBJ Player = { 0, 0, { 130, 100, 130, 100, 130, 100 }, 0, 0, BLUE, 3, 8, 0, 5 };
OBJ Zombie[N];
OBJ Bullet = { 0, };

MP Back;


/*===============================================================================*/

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
	Lcd_Clr_Screen(0x0000);

	Uart_Printf("Welcome Zombie Escape Game\n");
	Game_In();

	Game_Init();
	//Timer0_Repeat(30);
	for (;;)
	{
		Move();
		if (Count_Zombie()) break; // 좀비가 없으면 Game_Out
	}
	//	Objects();

}

/*===============================================================================*/



void Game_In(void)
{
	Lcd_Clr_Screen(BACK_COLOR);
	Lcd_Printf(60, 110, WHITE, BACK_COLOR, 1, 1, "Press the Button to Start");
	Key_Wait_Key_Pressed();
}



void Game_Init(void)
{
	Lcd_Clr_Screen(BACK_COLOR);
	Timer0_Repeat(20);

	Map_init();
	Player_init();
	Zombie_init();
}


void Map_init(void)
{
	Lcd_Select_Buffer_Mode(VIRTUAL_MODE);
	Lcd_Select_Display_Frame_Buffer(1);
	Lcd_Select_Draw_Frame_Buffer(1);

	Back.pos.init_x = 0;
	Back.pos.init_y = 0;
	Back.pos.x = 0;
	Back.pos.y = 0;
	Back.map_num = 0;
	Back.w = Maps[Back.map_num][0];
	Back.h = Maps[Back.map_num][1];

	Lcd_Draw_BMP(Back.pos.init_x, Back.pos.init_y, Maps[Back.map_num]);
	//Call_Map(Back.pos.y,Back.pos.y,Back.w,Back.h,Maps[Back.map_num]);
	Lcd_Set_Virtual_Display_Start(Back.pos.init_x, Back.pos.init_y);

	//Timer4_Delay(1000);
}


void Player_init(void)
{
	Player.move_flag = 3;
	Player.speed_step = PLY_SPEED;
	Player.w = Man[Player.move_flag][0];
	Player.h = Man[Player.move_flag][1];

	Player.pos.init_x = (320 - Player.w) / 2;
	Player.pos.init_y = (240 - Player.h) / 2;
	Player.pos.x = (320 - Player.w) / 2;
	Player.pos.y = (240 - Player.h) / 2;

	Lcd_Set_Shape_Mode(1, 0xFFFE);
	Lcd_Draw_BMP(Player.pos.init_x, Player.pos.init_y, Man[Player.move_flag]);
	Lcd_Set_Shape_Mode(0, 0);
}

void Zombie_init(void)
{
	int i;

	srand(RTC_Get_Time());

	for (i = 0; i<N; i++)
	{
		Zombie[i].pos.init_x = rand() % 320 + 100;
		Zombie[i].pos.init_y = rand()*i % 240 + 100;
		Zombie[i].pos.x = Zombie[i].pos.init_x;
		Zombie[i].pos.y = Zombie[i].pos.init_y;
		Zombie[i].pos.old_x = Zombie[i].pos.init_x;
		Zombie[i].pos.old_y = Zombie[i].pos.init_y;
		Zombie[i].timer = 0;
		Zombie[i].time_step = rand() % 10 + 10;
		Zombie[i].move_flag = 2;
		Zombie[i].attack_flag = 2;
		Zombie[i].life_flag = 2;
		Zombie[i].speed_step = ZB_SPEED;
		Zombie[i].w = arr_Zombie[Zombie[i].move_flag * i + i][0];
		Zombie[i].h = arr_Zombie[Zombie[i].move_flag * i + i][1];
		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Zombie[i].pos.init_x, Zombie[i].pos.init_y, arr_Zombie[Zombie[i].move_flag * i + i]);
		Lcd_Set_Shape_Mode(0, 0);
	}
}



/*===============================================================================*/

void Move(void)
{
	int key;
	int i;

	if (Timer0_Check_Expired())  // 20
	{
		key = Key_Get_Pressed() - 1;

		if (key >= 0)
		{
			Player.timer++;

			// 디버깅
			//			Uart_Printf("Key: %d\n", key);
			Uart_Printf("Back_pos X =[%d], Y=[%d]\n", Back.pos.x, Back.pos.y);
			Uart_Printf("Player_pos X =[%d], Y=[%d]\n", Player.pos.x, Player.pos.y);

			//Detect_Crash();
			Move_Map(key);
			Draw_Map(key);
			Move_Player(key);
			Draw_Player(key);
			Move_Bullet(key);
		}

		for (i = 0; i<N; i++)
		{
			if (Zombie[i].life_flag > 0) Zombie[i].timer++;
			if (Zombie[i].timer == Zombie[i].time_step)
			{
				printf("Zombie[%d].time_step = %d,  timer %d\n", i, Zombie[i].time_step, Zombie[i].timer);
				Zombie[i].timer = 0;
			}

		}

		Detect_Zombie();  // 사람과 좀비 거리 계산
		Move_Zombie();    // 좀비 그려주기
	}

	return;
}


/*--- MAP --------------------------------------------------------------------*/

void Erase_Map(int x, int y, int w, int h, const unsigned short int *fp)
{
	register int map_w = fp[0];
	int i, j;

	for (i = y; i<y + h; i++)
	{
		for (j = x; j<x + w; j++)
		{
			Lcd_Put_Pixel(j, i, (int)fp[map_w*i + j + 2]);
		}
	}
}


void Call_Map(int x, int y, int w, int h, const unsigned short int *fp)
{
	register int map_w = fp[0];
	register int i, j;

	for (i = y; i<y + h; i++)
	{
		for (j = x; j<x + w; j++)
		{
			Lcd_Put_Pixel(j, i, (int)fp[map_w*i + j + 2]);
		}
	}
}


void Move_Map(int key)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };

	// TODO_2
	if ((Player.pos.x>460) && (Player.pos.x <= 463) && (Player.move_flag == 3))
	{
		Call_Map(Player.pos.x, 0, Back.w - Player.pos.x, Back.h, Maps[Back.map_num]);
	}
	else if ((Player.pos.x>460) && (Player.pos.x <= 463) && (Player.move_flag == 1))
	{
		Lcd_Draw_BMP(0, 0, Maps[Back.map_num]);
	}

	// 구역별 맵 움직임 제어
	Back.pos.old_x = Back.pos.x;
	Back.pos.old_y = Back.pos.y;


	// 구역별 맵 움직임 제어
	if (PLY_AREA_FLAG && MAP_AREA_FLAG && (Back.pos.y>0))
	{

		if ((key<4) && AREA_FLAG)
		{
			Back.pos.old_x = Back.pos.x;
			Back.pos.old_y = Back.pos.y;
			Back.pos.x += dx[key] * Player.speed_step;
			Back.pos.y += dy[key] * Player.speed_step;
		}
		else
		{
			Uart_Printf("else\n");
			Back.pos.x = Back.pos.old_x;
			Back.pos.y = Back.pos.old_y;
		}

		//Lcd_Set_Virtual_Display_Start(map_pos.x, map_pos.y);

	}

	else
	{
		Back.pos.x = Back.pos.old_x;
		Back.pos.y = Back.pos.old_y;
	}
	//	if(key<4)
	//	{
	//		if (MAP_AREA_FLAG)
	//		{
	//			// 디버깅
	//			printf("	area3\n");
	//
	//			Back.pos.x += dx[key] * Player.speed_step;
	//			Back.pos.y += dy[key] * Player.speed_step;
	//
	//			Back.pos.x = Back.pos.x>0 ? Back.pos.x : Back.pos.old_x;
	//			Back.pos.y = Back.pos.y>0 ? Back.pos.y : Back.pos.old_y;
	//
	//			Back.pos.x = Back.pos.x<690 ? Back.pos.x : Back.pos.old_x;
	//			Back.pos.y = Back.pos.y<360 ? Back.pos.y : Back.pos.old_y;
	//		}
	/*  조건에 맞지 않는데 들어옴   */
	//		else if ((Back.pos.x>160 && Back.pos.x+Player.w<690) && AREA_FLAG_1)
	//		{
	//			// 디버깅
	//			printf("	area1\n");
	//
	//			Back.pos.x += dx[key] * Player.speed_step;
	//			Back.pos.y += dy[key] * Player.speed_step;
	//
	//			Back.pos.x = Back.pos.x>0 ? Back.pos.x : Back.pos.old_x;
	//			Back.pos.y = Back.pos.y>0 ? Back.pos.y : Back.pos.old_y;
	//		}
	//		else if ((Back.pos.y>120 && Back.pos.y+Player.h<360) && AREA_FLAG_2)
	//		{
	//			// 디버깅
	//			printf("	area2\n");
	//
	//			Back.pos.x += dx[key] * Player.speed_step;
	//			Back.pos.y += dy[key] * Player.speed_step;
	//
	//			Back.pos.x = Back.pos.x>0 ? Back.pos.x : Back.pos.old_x;
	//			Back.pos.y = Back.pos.y>0 ? Back.pos.y : Back.pos.old_y;
	//		}
	//		else
	//		{
	//			// 디버깅
	//			printf("	area0\n");
	//
	//			Back.pos.x = Back.pos.old_x;
	//			Back.pos.y = Back.pos.old_y;
	//		}
	//	}
}

void Draw_Map(int key)
{
	Lcd_Set_Virtual_Display_Start(Back.pos.x, Back.pos.y);
}



/*--- PLAYER --------------------------------------------------------------------*/

void Move_Player(int key)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };

	// MOVE MODE
	if ((key<4) && (PLY_AREA_FLAG))
	{
		// 캐릭터 방향 전환
		Player.move_flag = key;
		Player.pos.old_x = Player.pos.x;
		Player.pos.old_y = Player.pos.y;

		// 디버깅
		//printf("정상진입\n"); // 정상작동

		Player.pos.x += dx[key] * Player.speed_step;
		Player.pos.y += dy[key] * Player.speed_step;

		Player.pos.x = Player.pos.x>0 ? Player.pos.x : Player.pos.old_x;
		Player.pos.y = Player.pos.y>0 ? Player.pos.y : Player.pos.old_y;
	}
	else if (key<4)
	{
		// 디버깅
		printf("PLY out of range\n");

		Player.pos.x = Player.pos.old_x;
		Player.pos.y = Player.pos.old_y;
	}

	// BATTLE MODE
	if ((key == 4) && (Player.timer == COOLTIME))
	{
		Uart_Printf("Key: %d\n", key);

		Player.timer = 0;
		Bullet.attack_flag = 2;
		Bullet.weapon_mode = 1; // 기본총알
	}
	if (key == 5 && (Player.timer == COOLTIME))
	{
		Player.timer = 0;
		Bullet.attack_flag = 2;
		Bullet.weapon_mode = 2; // 유도탄
	}
}

void Draw_Player(int key)
{
	Lcd_Set_Shape_Mode(1, 0xFFFE);
	Erase_Map(Player.pos.old_x, Player.pos.old_y, Player.w, 2 * Player.h, Maps[Back.map_num]);
	Lcd_Draw_BMP(Player.pos.x, Player.pos.y, Man[Player.move_flag]);
	Lcd_Set_Shape_Mode(0, 0);
}


/*--- BULLET --------------------------------------------------------------------*/

void Move_Bullet(int key)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };
	int dir = Player.move_flag;
	int cnt_1 = 5;

	// BULLET INIT
	if (Bullet.attack_flag == 2)
	{
		Bullet.attack_flag--;

		Bullet.w = Bullets[dir][0];
		Bullet.h = Bullets[dir][1];
		Bullet.speed_step = BL_SPEED;

		if (dir == 0)
		{
			Bullet.pos.init_x = Player.pos.x + (Player.w / 2);
			Bullet.pos.init_y = Player.pos.y + Player.h + Bullet.h;
		}
		else if (dir == 1)
		{
			Bullet.pos.init_x = Player.pos.x - Bullet.w;
			Bullet.pos.init_y = Player.pos.y + (Player.h / 2);
		}
		else if (dir == 2)
		{
			Bullet.pos.init_x = Player.pos.x + (Player.w / 2);
			Bullet.pos.init_y = Player.pos.y + Player.h;
		}
		else if (dir == 3)
		{
			Bullet.pos.init_x = Player.pos.x + Player.w;
			Bullet.pos.init_y = Player.pos.y + (Player.h / 2);
		}

		Bullet.pos.x = Bullet.pos.init_x;
		Bullet.pos.y = Bullet.pos.init_y;

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, Bullets[dir]);
		Lcd_Set_Shape_Mode(0, 0);

		// 디버깅
		Uart_Printf("BL_INIT%d : %d %d\n", dir, Bullet.pos.x, Bullet.pos.old_x); // 정상작동
	}


	// Bullet Move
	while (Bullet.attack_flag > 0)
	{
		Bullet.pos.old_x = Bullet.pos.x;
		Bullet.pos.old_y = Bullet.pos.y;

		if (Detect_Crash()) // 벽 or 좀비 맞았을 때
		{
			// 디버깅
			printf("and Crashed \n");

			Bullet.attack_flag = -1;
			Erase_Map(Bullet.pos.old_x, Bullet.pos.old_y, Bullet.w, Bullet.h, Maps[Back.map_num]);
			break;
		}

		Bullet.pos.x += (Bullet.speed_step * dx[dir]);
		Bullet.pos.y += (Bullet.speed_step * dy[dir]);

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Erase_Map(Bullet.pos.old_x, Bullet.pos.old_y, Bullet.w, Bullet.h, Maps[Back.map_num]);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, Bullets[dir]);
		Lcd_Set_Shape_Mode(0, 0);
	}

	// WEAPON MODE : 기본 총알
	if (Bullet.weapon_mode % 3 == 1)
	{
		if (cnt_1 == 0) cnt_1 = 5;

		cnt_1--;
	}
	// WEAPON MODE : 유도탄
	else if (Bullet.weapon_mode % 3 == 2)
	{
		//		int i;
		//
		//		Bullet.pos.x - .pos.x
		//		for(i=0;;i++)
		//		{
		//			Bullet.pos.x += ((Player.pos.x<Bullet.pos.x)? (Bullet.pos.x - Player.pos.x)*i/3 : (Player.pos.x - Bullet.pos.x)*i/3 );
		//			Bullet.pos.old_x = Bullet.pos.x;
		//			Bullet.pos.old_y = Bullet.pos.y;
		//		}
	}
}


int Detect_Crash(void)
{
	int i;

	if (Bullet.attack_flag)
	{
		// 벽
		if (!(AREA_CHECK_X(Bullet)) || !(AREA_CHECK_Y(Bullet)))
		{
			// 디버깅
			printf("Wall ");

			return 1;
		}

		// 좀비 //한발당 좀비 하나 죽으니까 여기에 for문 위치하는 것이 맞다.
		for (i = 0; i<N; i++)
		{
			// 생명 0일 때 소멸
			if (Zombie[i].life_flag == 0)
			{
				// 디버깅
				printf("Zombie die "); // 정상작동

				Erase_Map(Zombie[i].pos.x, Zombie[i].pos.y, Zombie[i].w, Zombie[i].h, Maps[Back.map_num]);

				return 2;
			}

			// Bullet direction 0
			if ((((Zombie[i].pos.x)<Bullet.pos.x) && ((Zombie[i].pos.x + Zombie[i].w)<Bullet.pos.x))\
				&& ((Zombie[i].pos.y + Zombie[i].h) > Bullet.pos.y))
			{
				// 디버깅
				printf("Shot!!!! 0");
				--Zombie[i].life_flag;

				return 1;
			}
			// Bullet direction 1
			else if ((Zombie[i].pos.x + Zombie[i].w)>Bullet.pos.x \
				&& ((Zombie[i].pos.y>Bullet.w) && (Zombie[i].pos.y + Zombie[i].h < Bullet.w)))
			{
				printf("Shot!!!! 1");
				--Zombie[i].life_flag;

				return 1;
			}
			// Bullet direction 2
			else if ((((Zombie[i].pos.x)<Bullet.pos.x) && ((Zombie[i].pos.x + Zombie[i].w)<Bullet.pos.x))\
				&& ((Zombie[i].pos.y - Bullet.h) > Bullet.pos.y))
			{
				printf("Shot!!!! 2");
				--Zombie[i].life_flag;

				return 1;
			}
			// Bullet direction 3
			else if ((Zombie[i].pos.x - Bullet.w > Bullet.pos.x) \
				&& ((Zombie[i].pos.y>Bullet.w) && (Zombie[i].pos.y + Zombie[i].h < Bullet.w)))
			{
				printf("Shot!!!! 3");
				--Zombie[i].life_flag;

				return 1;
			}
		}
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


/*--- ZOMBIE --------------------------------------------------------------------*/

void Detect_Zombie(void)
{
	int i;

	for (i = 0; i < N; i++)
	{
		if (Zombie[i].timer == 0 && Zombie[i].life_flag > 0)
		{
			// Control time
			if ((ABS(Player.pos.x - Zombie[i].pos.x) < DIST) && (ABS(Player.pos.y - Zombie[i].pos.y) < DIST))
			{
				//Uart_Printf("Detect_Zombie!\n"); // 정상작동
				Zombie[i].speed_step = Zombie[i].speed_step + i % 3;
			}
			else Zombie[i].speed_step = ZB_SPEED;
		}
	}
}


void Move_Zombie(void)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };
	int dir = Player.move_flag;
	int i;

	//if (dir >= 4) continue;
	for (i = 0; i<N; i++)
	{
		if (ZB_AREA_FLAG(i) && (Zombie[i].timer == 0) && (Zombie[i].life_flag))
		{
			Zombie[i].pos.old_x = Zombie[i].pos.x;
			Zombie[i].pos.old_y = Zombie[i].pos.y;
			Zombie[i].pos.x += dx[dir] * Zombie[i].speed_step; // (Player.speed_step + ACCEL(i));
			Zombie[i].pos.y += dy[dir] * Zombie[i].speed_step;

			//			if ((Zombie[i].pos.y < 0 + Zombie[i].h) || (Zombie[i].pos.x<0))  // 조건
			//			{
			//				Zombie[i].pos.x = Zombie[i].pos.old_x;
			//				Zombie[i].pos.y = Zombie[i].pos.old_y;
			//			}

			//Uart_Printf("ZB#%d : %d %d\n", i, Zombie[i].pos.x,Zombie[i].pos.y);
			//Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, 2*Zombie[i].w, 2*Zombie[i].h, Maps[Back.map_num]);
			Lcd_Set_Shape_Mode(1, 0xFFFE);
			printf("Zombie[%d].timer = %d, life_flag = %d\n", i, Zombie[i].timer, Zombie[i].life_flag);
			printf("%d %d, %d %d\n", Zombie[i].pos.old_x, Zombie[i].pos.old_y, Zombie[i].pos.x, Zombie[i].pos.y);
			printf("%d %d\n", Zombie[i].w, Zombie[i].h);
			Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, Zombie[i].w, Zombie[i].h, Maps[Back.map_num]);
			Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, arr_Zombie[Zombie[i].move_flag * i + i]);
			Lcd_Set_Shape_Mode(0, 0);
		}
	}

	return;
}

// TODO_1
void Draw_Zombie(void)
{

}


int Count_Zombie(void)
{
	int i;
	int cnt = 0;

	for (i = 0; i<N; i++)
	{
		if (Zombie[i].life_flag>0)
		{
			cnt++;
		}
	}

	if (cnt == 0)
	{
		Game_Out(1); // WIN
		return 1; // 우선은 --> 추후 다음스테이지로 넘어가게 수정
	}

	else if (Player.life_flag == 0)
	{
		Game_Out(0);
		return 1;
	}

	return 0;
}


/*===============================================================================*/

void Game_Out(int ver)
{
	// Game Over
	if (ver == 0)
	{
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Printf(10, 10, WHITE, BACK_COLOR, 1, 1, "Game Over");
	}
	// Win
	if (ver == 1)
	{
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Printf(10, 10, WHITE, BACK_COLOR, 1, 1, "!!! YOU WIN !!!");
	}
}

#endif


