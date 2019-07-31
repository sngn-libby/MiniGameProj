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
#define AREA_FLAG (((Player.pos.x>160) && (Player.pos.x<690)) || ((Player.pos.y>120) && (Player.pos.y < 360))) ? 1:0
#define AREA_FLAG_1 (((Player.pos.x>85) && (Player.pos.x<160)) || ((Player.pos.x>690) && (Player.pos.x<765))) ? 1:0
#define AREA_FLAG_1_REV ((Player.pos.x>160) && (Player.pos.x<690)) ? 1:0
#define AREA_FLAG_2 (((Player.pos.y>50) && (Player.pos.y < 120)) || ((Player.pos.y>360) && (Player.pos.y < 420))) ? 1:0
#define AREA_FLAG_2_REV ((Player.pos.y>120) && (Player.pos.y < 360)) ? 1:0
#define MAP_AREA_FLAG (((Player.pos.x>160) && (Player.pos.x<690)) && ((Player.pos.y>120) && (Player.pos.y < 360))) ? 1:0
#define PLY_AREA_FLAG ((Player.pos.x>85) && (Player.pos.x<765) && (Player.pos.y>50) && (Player.pos.y < 420)) ? 1:0
#define ZB_AREA_FLAG(I) ((Zombie[(I)].pos.x>85) && (Zombie[(I)].pos.x<765) && (Zombie[(I)].pos.y>50) && (Zombie[(i)].pos.y < 360)) ? 1 : 0
#define AREA_CHECK_X(X) (((X)>50) && ((X)<765))
#define AREA_CHECK_Y(Y) (((Y)>50) && ((Y)<420))





#include "./Image/surgery_480.h"
#include "./Image/road.h"

#include "./Image/man_01.h"
#include "./Image/man_02.h"
#include "./Image/man_03.h"
#include "./Image/man_04.h"

// Bullet
#define CRASH_X(I) ((Bullet.pos.x >= Zombie[(I)].pos.x) && (Bullet.pos.x<= Zombie[(I)].pos.x+Zombie[(I)].w)) ? 1:0
#define CRASH_Y(I) ((Bullet.pos.y >= Zombie[(I)].pos.y) && (Bullet.pos.y<= Zombie[(I)].pos.y+Zombie[(I)].h)) ? 1:0

#include "./Image/bullet_01_0.h"
#include "./Image/bullet_01_1.h"
#include "./Image/bullet_01_2.h"
#include "./Image/bullet_01_3.h"

// Zombie
#define ACCEL(i) (Zombie[i].move_flag) ? (Zombie[i].speed_step * (i)) : 0
#define N (3)

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
const unsigned short * Bullets[] = { bullet_01_0,bullet_01_1,bullet_01_2,bullet_01_3 };
const unsigned short * arr_Zombie[] = {zombie_01_0,zombie_02_0,zombie_03_0\
										,zombie_01_1,zombie_02_1,zombie_03_1\
										,zombie_01_2,zombie_02_2,zombie_03_2\
										,zombie_01_3,zombie_02_3,zombie_03_3 };

/*------------------------------------------------------------------------------*/

void Game_In(void);

void Game_Init(void);
void Map_init(void);
void Character_init(void);
void Zombie_init(void);

void Move(void);
void Draw_Character(int key);
void Draw_Map(int key);
void Draw_Zombie(void);
void Draw_Bullet(int key);
void explosion(void);
int Crash_Detect(void);
void Zombie_Detect(void);
void Run_Zombie(void);



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
}OBJ;

OBJ Player = { 0, 0, { 130, 100, 130, 100, 130, 100 }, 0, 0, BLUE, 3, 8, 0, 5 };
OBJ Zombie[N];
POS map_pos = { 0, 0, 0, 0, 0, 0 };

OBJ Bullet = { 0, };




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
		if(Count_Zombie()) break; // 좀비가 없으면 Game_Out
	}
	//	Objects();

}

/*===============================================================================*/



void Game_In(void)
{
	Lcd_Clr_Screen(BACK_COLOR);
	Lcd_Printf(60,110,WHITE,BACK_COLOR,1,1,"Press the Button to Start");
	Key_Wait_Key_Pressed();
}



void Game_Init(void)
{
	Lcd_Clr_Screen(BACK_COLOR);
	Timer0_Repeat(20);

	Map_init();
	Character_init();
	Zombie_init();
}

/*
void Lcd_Select_Buffer_Mode(unsigned int mode);
void Lcd_Set_Virtual_Display_Start(int x, int y);
void Lcd_Select_Draw_Frame_Buffer(unsigned int id)
void Lcd_Select_Display_Frame_Buffer(unsigned int id)
void Lcd_Graphic_Init(void)
*/

void Map_init(void)
{
	Lcd_Select_Buffer_Mode(VIRTUAL_MODE);
	Lcd_Select_Display_Frame_Buffer(0);
	Lcd_Select_Draw_Frame_Buffer(0);

	Lcd_Draw_BMP(0, 0, Maps[0]);
	Lcd_Set_Virtual_Display_Start(0, 0);
	map_pos.x = 0;
	map_pos.y = 0;
	//Timer4_Delay(1000);
}


void Character_init(void)
{
	Player.move_flag = 3;
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

	for(i=0;i<N;i++)
	{
		Zombie[i].pos.init_x = rand()%320+100;
		Zombie[i].pos.init_y = rand()*i%240+100;
		Zombie[i].pos.x = Zombie[i].pos.init_x;
		Zombie[i].pos.y = Zombie[i].pos.init_y;
		Zombie[i].pos.old_x = Zombie[i].pos.init_x;
		Zombie[i].pos.old_y = Zombie[i].pos.init_y;

		//Zombie[i].move_flag = 1;
		Zombie[i].speed_step = 1;
		Zombie[i].attack_flag = 2;
		Zombie[i].life_flag = 2;

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Zombie[i].pos.init_x, Zombie[i].pos.init_y, arr_Zombie[i]);
		Lcd_Set_Shape_Mode(0,0);
	}
}



/*===============================================================================*/

void Move(void)
{
	int key = Key_Get_Pressed() - 1;

	if(Timer0_Check_Expired())
	{
		Zombie[0].timer++;
		if(Zombie[0].timer == 10)
		{
			Zombie[0].timer = 0;
		}

		if (key >= 0)
		{
			Uart_Printf("Key: %d\n", Key_Get_Pressed());
			Uart_Printf("map_pos X =[%d], Y=[%d]\n", map_pos.x, map_pos.y);
			Uart_Printf("Player_pos X =[%d], Y=[%d]\n", Player.pos.x, Player.pos.y);

			//Crash_Detect();
			Draw_Map(key);
			Draw_Character(key);
			Draw_Bullet(key);
		}
		Zombie_Detect();
		Draw_Zombie();
	}

	return;
}


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


void Draw_Map(int key)
{
	int dy[] = {-1,0,1,0};
	int dx[] = {0,-1,0,1};

	Uart_Printf("%d ", key);
	Uart_Printf("%d\n", AREA_FLAG);
	Uart_Printf("1: %d ", AREA_FLAG_1);
	Uart_Printf("2: %d\n", AREA_FLAG_2);

	// map 갱신
	if((Player.pos.x>460)&&(Player.pos.x<=463)&&(Player.move_flag==3))
	{
		Call_Map(Player.pos.x,0, 390, 480, Maps[0]);
	}
	else if((Player.pos.x>460)&&(Player.pos.x<=463)&&(Player.move_flag==1))
	{
		Lcd_Draw_BMP(0,0,Maps[0]);
	}

	// 구역별 맵 움직임 제어
	if(PLY_AREA_FLAG && MAP_AREA_FLAG&&(map_pos.y>0))
	{

		if((key<4)&&AREA_FLAG)
		{
			map_pos.old_x = map_pos.x;
			map_pos.old_y = map_pos.y;
			map_pos.x += dx[key] * Player.speed_step;
			map_pos.y += dy[key] * Player.speed_step;
		}
		else
		{
			Uart_Printf("else\n");
			map_pos.x = map_pos.old_x;
			map_pos.y = map_pos.old_y;
		}

		Lcd_Set_Virtual_Display_Start(map_pos.x, map_pos.y);

	}

	else
	{
		map_pos.x = map_pos.old_x;
		map_pos.y = map_pos.old_y;
	}
}


void Draw_Character(int key)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };

	printf("key : %d\n", key);
	if (key<4)
	{
		// 캐릭터 방향 전환
		Player.move_flag = key;

		// 이동
		if (PLY_AREA_FLAG)
		{
			//printf("정상진입\n");
			Uart_Printf("%d %d\n", Player.pos.x, Player.pos.old_x );
			Player.pos.old_x = Player.pos.x;
			Player.pos.old_y = Player.pos.y;
			Player.pos.x += dx[key] * Player.speed_step;
			Player.pos.y += dy[key] * Player.speed_step;

			Player.pos.x = Player.pos.x>0 ? Player.pos.x : Player.pos.old_x;
			Player.pos.y = Player.pos.y>0 ? Player.pos.y : Player.pos.old_y;
			Uart_Printf("%d %d\n", Player.pos.x, Player.pos.old_x );
//			if(key==1)
//			{
//				Erase_Map(Player.pos.x+3, Player.pos.y, Player.w, Player.h, Maps[0]);
//			}

		}
		else
		{
			Erase_Map(Player.pos.x, Player.pos.y, Player.w, Player.h, Maps[0]);
			Player.pos.x = Player.pos.old_x;
			Player.pos.y = Player.pos.old_y;
		}

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Erase_Map(Player.pos.old_x, Player.pos.old_y, Player.w, 2*Player.h, Maps[0]);
		Lcd_Draw_BMP(Player.pos.x, Player.pos.y, Man[Player.move_flag]);
		Lcd_Set_Shape_Mode(0, 0);
	}

	if(key==4) Bullet.attack_flag = 1;
	if(key==5) Bullet.move_flag = 2; // 유도탄
}

void Draw_Bullet(int key)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };
	int dir = Player.move_flag;

	// BULLET INIT
	if (Bullet.attack_flag)
	{
		printf("bullet init\n");

		Bullet.attack_flag = 0;
		Bullet.move_flag = 1;

		Bullet.pos.init_x = Player.pos.x + (Player.w/2);
		Bullet.pos.init_y = Player.pos.y + (Player.h/2);
		Bullet.pos.x = Bullet.pos.init_x;
		Bullet.pos.y = Bullet.pos.init_y;
		Bullet.pos.old_x = Bullet.pos.x;
		Bullet.pos.old_y = Bullet.pos.y;

		Bullet.w = Bullets[dir][0];
		Bullet.h = Bullets[dir][1];
		Bullet.speed_step = 10;

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, Bullets[dir]);
		Lcd_Set_Shape_Mode(0, 0);

		Uart_Printf("%d %d\n", Bullet.pos.x, Bullet.pos.old_x );
	}


	// Bullet Move
	while(Bullet.move_flag>0)
	{
		//printf("bullet while in\n"); // 정상작동

		Bullet.pos.old_x = Bullet.pos.x;
		Bullet.pos.old_y = Bullet.pos.y;

		Bullet.pos.x += (Bullet.speed_step * dx[dir]);
		Bullet.pos.y += (Bullet.speed_step * dy[dir]);

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Erase_Map(Bullet.pos.old_x, Bullet.pos.old_y, Bullet.w, Bullet.h, Maps[0]);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, Bullets[dir]);
		Lcd_Set_Shape_Mode(0, 0);

		if (Crash_Detect())
		{
			printf("Bullet Flag #1 : %d\n", Bullet.move_flag);
			//Bullet.move_flag = 0;
			Erase_Map(Bullet.pos.x, Bullet.pos.y, Bullet.w, Bullet.h, Maps[0]);
			break;
		}

		//printf("Bullet area : %d %d\n", !AREA_CHECK_X(Bullet.pos.x), !AREA_CHECK_Y(Bullet.pos.y));
		if ( (!AREA_CHECK_X(Bullet.pos.x)) || (!AREA_CHECK_Y(Bullet.pos.y)) )
		{
			printf("Bullet Flag #2 : %d\n", Bullet.move_flag);
			Bullet.move_flag = 0;
			Erase_Map(Bullet.pos.x, Bullet.pos.y, Bullet.w, Bullet.h, Maps[0]);
			break;
		}
	}

	printf("bullet while out\n");
	// 유도탄
	if (Bullet.move_flag==2)
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

	if (Bullet.attack_flag)
	{
		if (dir == 0)
			Bullet.pos.y -= Bullet.speed_step;
		else if (dir == 1)
			Bullet.pos.x -= Bullet.speed_step;
		else if (dir == 2)
			Bullet.pos.y += Bullet.speed_step;
		else if (dir == 3)
			Bullet.pos.x -= Bullet.speed_step;

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, Bullets[dir]);
		Lcd_Set_Shape_Mode(0, 0);
	}
}

int Crash_Detect(void)
{
	int i;

	if (Bullet.move_flag)
	{
		//printf("Crash Detect In\n"); // 정상 작동
		for (i = 0; i<N; i++)
		{
			if (CRASH_X(i) && CRASH_Y(i))
			{
				printf("Shot!!!!\n"); // 정상 작동
				// 좀비 생명 기본 2
				--Zombie[i].life_flag;
				Bullet.move_flag = 0;

				// 좀비 생명 0일 때 소멸
				if (Zombie[i].life_flag == 0)
				{
					printf("Zombie die\n");
					Bullet.move_flag = 0;
					Erase_Map(Zombie[i].pos.x, Zombie[i].pos.y, Zombie[i].w, Zombie[i].h, Maps[0]);

					return 2;
				}

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

void Zombie_Detect(void)
{
	int i;

	for (i = 0; i < N; i++)
	{
		if(Zombie[0].timer == 0)
		{
			// Control time
			if (((Player.pos.x - Zombie[i].pos.x) < 50) && ((Player.pos.y - Zombie[i].pos.y) < 50))
			{
				//Uart_Printf("Zombie_Detect!\n");
				Run_Zombie();
			}
		}
	}
}


void Run_Zombie(void)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };
	int dir = Player.move_flag;
	int i;

	for(i=0;i<N;i++)
	{
		if (Zombie[0].timer == 0)
		{
			// x 좌표 감별
			if(Player.pos.x>Zombie[i].pos.x)
			{
				Zombie[i].pos.x += dx[dir] * (Player.speed_step + Zombie[i].speed_step/(i+1));
			}
			else
			{
				Zombie[i].pos.x -= dx[dir] * (Player.speed_step + Zombie[i].speed_step/(i+1));
			}

			// y 좌표 감별
			if(Player.pos.y>Zombie[i].pos.y)
			{
				Zombie[i].pos.y += dy[dir] * (Player.speed_step + Zombie[i].speed_step/(i+1));
			}
			else
			{
				Zombie[i].pos.y -= dy[dir] * (Player.speed_step + Zombie[i].speed_step/(i+1));
			}
			//Uart_Printf("ZB_RUN : %d %d\n", Zombie[0].pos.x,Zombie[0].pos.y);
		}
	}
}

void Draw_Zombie(void)
{
	int dy[] = { -1, 0, 1, 0 };
	int dx[] = { 0, -1, 0, 1 };
	int dir = Player.move_flag;
	int i;

	for (i = 0; i<N; i++)
	{
		if (ZB_AREA_FLAG(i))
		{

			if((Zombie[0].timer == 0) && (Zombie[i].life_flag))
			{
				Lcd_Set_Shape_Mode(1, 0xFFFE);
				Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, 2*Zombie[i].w, 2*Zombie[i].h, Maps[0]);

				if (dir<4)
				{
					if ((Zombie[i].pos.y<0) || (Zombie[i].pos.x<0))
					{
						Zombie[i].pos.x = Zombie[i].pos.old_x;
						Zombie[i].pos.y = Zombie[i].pos.old_y;
					}

					Zombie[i].pos.old_x = Zombie[i].pos.x;
					Zombie[i].pos.old_y = Zombie[i].pos.y;
					Zombie[i].pos.x += dx[dir] * Player.speed_step; // (Player.speed_step + ACCEL(i));
					Zombie[i].pos.y += dy[dir] * Player.speed_step;

					//Uart_Printf("ZB#%d : %d %d\n", i, Zombie[i].pos.x,Zombie[i].pos.y);
				}

//				else
//				{
//					Uart_Printf("else\n");
//					Zombie[i].pos.x = Zombie[i].pos.old_x;
//					Zombie[i].pos.y = Zombie[i].pos.old_y;
//					//Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, 2*Zombie[i].w, 2*Zombie[i].h, Maps[0]);
//				}

				//Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, 2*Zombie[i].w, 2*Zombie[i].h, Maps[0]);
				Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, arr_Zombie[i]);
				Lcd_Set_Shape_Mode(0,0);
			}
		}
		else
		{
			Zombie[i].pos.x = Zombie[i].pos.old_x;
			Zombie[i].pos.y = Zombie[i].pos.old_y;
		}
	}

	return;
}

int Count_Zombie(void)
{
	int i;
	int cnt = 0;

	for(i=0;i<N;i++)
	{
		if(Zombie[i].life_flag>0)
		{
			cnt ++;
		}
	}

	if(cnt == 0)
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

void Game_Out(int ver)
{
	// Game Over
	if (ver==0)
	{
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Printf(10,10,WHITE,BACK_COLOR,1,1,"Game Over");
	}
	// Win
	if (ver==1)
	{
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Printf(10,10,WHITE,BACK_COLOR,1,1,"!!! YOU WIN !!!");
	}
}

#endif


