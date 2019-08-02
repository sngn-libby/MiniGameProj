/***********************************************************/
// 다음은 사용자 함수를 호출하는 루틴임 지우지 말것!
/***********************************************************/
#if 01

#include <stdlib.h>
#include "device_driver.h"
#include "2440addr.h"

//#include "myCharacter.c"

enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
enum note{N16, N8, N4, N2, N1};
const int song[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2},{0xff, 0xff}};

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

#define printf 	Uart_Printf

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


/*================================================================================================================================================
=================================================================================================================================================*/



// AREA_CONDITION_CHECK
// available map area condition
#define AREA_FLAG ((((Player.pos.x>160) && (Player.pos.x<690)) || ((Player.pos.y>120) && (Player.pos.y < 360))) ? 1:0)
//#define AREA_FLAG_1 (((Player.pos.x>85) && (Player.pos.x<160)) || ((Player.pos.x>690) && (Player.pos.x<765))) ? 1:0
//#define AREA_FLAG_1_REV ((Player.pos.x>160) && (Player.pos.x<690)) ? 1:0
//#define AREA_FLAG_2 (((Player.pos.y>50) && (Player.pos.y < 120)) || ((Player.pos.y>360) && (Player.pos.y < 420))) ? 1:0
//#define AREA_FLAG_2_REV ((Player.pos.y>120) && (Player.pos.y < 360)) ? 1:0
#define AREA_FLAG_1 (((Player.pos.y>120) && (Player.pos.y<(360-Player.h))) ? 1:0)
#define AREA_FLAG_2 (((Player.pos.x>160) && (Player.pos.x<(690-Player.w))) ? 1:0)
#define MAP_AREA_FLAG ((((Player.pos.x>160) && (Player.pos.x<(Back.w - 160 - Player.w))) && ((Player.pos.y>120) && (Player.pos.y < (Back.h - 120 - Player.h)))) ? 1:0 )
#define PLY_AREA_FLAG (((Player.pos.x>85) && (Player.pos.x<(765 - Player.w)) && (Player.pos.y>50) && (Player.pos.y < (420 - Player.h))) ? 1:0)
#define ZB_AREA_FLAG(I) (((Zombie[(I)].pos.x>85) && (Zombie[(I)].pos.x< (765 - Zombie[(I)].w))\
						&& (Zombie[(I)].pos.y>50) && (Zombie[(I)].pos.y < (420 - Zombie[(I)].h))) ? 1 : 0)
#define AREA_CHECK(obj) (((obj.pos.x>85) && (obj.pos.x< (765 - obj.w))\
						&& (obj.pos.y>50) && (obj.pos.y < (420 - obj.h))) ? 1 : 0)
#define AREA_CHECK_X(X) (((X).pos.x>50) && ((X).pos.x<765-(X).w))
#define AREA_CHECK_Y(Y) (((Y).pos.y>50) && ((Y).pos.y<420-(Y).h))
#define ABS(x) ((x)<0 ? -(x) : (x))


// SYSTEM
#include "./Image/cover_01.h"
#include "./Image/cover_02.h"
#include "./Image/ending_01.h"
#include "./Image/ending_02.h"

// MAP
#include "./Image/surgery.h"
//#include "./Image/road.h"
#include "./Image/office.h"
//#include "./Image/office_intro.h"
#include "./Image/gate.h"
//#include "./Image/eps.h"

// PLAYER
#define PLY_SPEED (3)
#define COOLTIME (10)
#define PLY_LIFE (30)

#include "./Image/man_01.h"
#include "./Image/man_02.h"
#include "./Image/man_03.h"
#include "./Image/man_04.h"


// BULLET
#define BL_SPEED (6)

#include "./Image/bullet_01_0.h"
#include "./Image/bullet_01_1.h"
#include "./Image/bullet_01_2.h"
#include "./Image/bullet_01_3.h"
#include "./Image/flash_01_0.h"
#include "./Image/flash_01_1.h"
#include "./Image/flash_01_2.h"
#include "./Image/flash_01_3.h"

// ZOMBIE
#define N (3)
#define DIST (30)
#define ZB_SPEED (2)
#define ZB_KILL2WIN (20)
int ZB_KILL = ZB_KILL2WIN;
int ZB_ACCEL = 0;

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

// EFFECT
#include "./Image/flash_02.h"
#include "./Image/blood_01.h"
#include "./Image/blood_02.h"
#include "./Image/blood_03.h"
#include "./Image/palm_01.h" // When Player is attacked

const unsigned short * Bloods[] = { blood_01, blood_02, blood_03, blood_02 };
const unsigned short * Cover[] = { cover_01, cover_02 };
const unsigned short * Maps[] = { gate, surgery, office };//, eps, road };
const unsigned short * Man[] = { man_01, man_02, man_03, man_04 };
const unsigned short * Bullets[] = { bullet_01_0, bullet_01_1, bullet_01_2, bullet_01_3 };
const unsigned short * Flashes[] = {flash_01_0,flash_01_1,flash_01_2,flash_01_3};
const unsigned short * arr_Zombie[] = { zombie_01_0, zombie_02_0, zombie_03_0\
										, zombie_01_1, zombie_02_1, zombie_03_1\
										, zombie_01_2, zombie_02_2, zombie_03_2\
										, zombie_01_3, zombie_02_3, zombie_03_3 };
const unsigned short * Endings[] = {ending_01, ending_02};


/*------------------------------------------------------------------------------*/


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
void Attack_Zombie(void);
void Erase_Map(int x, int y, int w, int h, const unsigned short int *fp);

int Count_Zombie(void);
int Game_Out(int ver);
void User_Main(void);

/*------------------------------------------------------------------------------*/

typedef struct str{
	int x, y;
	int old_x, old_y;
	int init_x, init_y;
	short conv2_x, conv2_y;
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

OBJ Player = { 0, 0, { 130, 100, 130, 100, 130, 100 }, 0, 0, BLUE, 3, 8, 0};
OBJ Zombie[N];
OBJ Bullet = { 0, };


MP Back; // Background Map
MP Flash; // Bullet Effect
MP Blood; // Attack Effect

/*===============================================================================*/

void Main(void)
{
	// System Init
	MMU_Init();
	Led_Init();
	Key_Poll_Init();
	Uart_Init(115200);
	Timer0_Init();
	Timer3_Buzzer_Init();
	Timer4_Init();
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0x0000);


	for (;;)
	{

		User_Main();

	}
}

/*===============================================================================*/

void User_Main(void)
{

	Uart_Printf("Welcome Zombie Escape Game\n");

	Lcd_Clr_Screen(BACK_COLOR);
	Timer0_Repeat(500);

	// INTRO
	for(;;)
	{
		if(Timer0_Check_Expired())
		{
			Lcd_Draw_BMP(0, 0, Cover[0]);
		}
		Timer0_Repeat(1000);
		Lcd_Draw_BMP(0,0, Cover[1]);

		if(Key_Get_Pressed())
			break;
	}

	Game_Init();

	// GAME
	for (;;)
	{

		Move();
		if (Count_Zombie()) break; // Call Game_Out() // OUTRO

	}

}

/*===============================================================================*/

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
	Back.map_num = 1;
	Back.w = Maps[Back.map_num][0];
	Back.h = Maps[Back.map_num][1];

	Lcd_Draw_BMP(Back.pos.init_x,Back.pos.init_y, Maps[Back.map_num]);
	Lcd_Set_Virtual_Display_Start(Back.pos.init_x, Back.pos.init_y);
}

void Player_init(void)
{
	Player.move_flag = 3;
	Player.speed_step = PLY_SPEED;
	Player.w = Man[Player.move_flag][0];
	Player.h = Man[Player.move_flag][1];
	Player.life_flag = PLY_LIFE;

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
		Zombie[i].pos.init_x = rand() % 320 + 160;
		Zombie[i].pos.init_y = rand()*i % 240 + 120;
		Zombie[i].pos.x = Zombie[i].pos.init_x;
		Zombie[i].pos.y = Zombie[i].pos.init_y;
		Zombie[i].pos.old_x = Zombie[i].pos.init_x;
		Zombie[i].pos.old_y = Zombie[i].pos.init_y;
		Zombie[i].time_step = rand() % 20 + 10;
		Zombie[i].timer = 0;
		Zombie[i].move_flag = 2;
		Zombie[i].attack_flag = 0;
		Zombie[i].life_flag = 2;
		Zombie[i].speed_step = ZB_SPEED*(rand()%3+1);
		Zombie[i].w = arr_Zombie[(Zombie[i].move_flag * 3 + i)%12][0];
		Zombie[i].h = arr_Zombie[(Zombie[i].move_flag * 3 + i)%12][1];

		Zombie[i].pos.init_x = (Zombie[i].pos.init_x <= 85? Zombie[i].pos.init_x: 100);
		Zombie[i].pos.init_y = (Zombie[i].pos.init_y <= 50? Zombie[i].pos.init_y: 100);
	}
}


/*===============================================================================*/

void Move(void)
{
	int key;
	int i;

	if(Timer0_Check_Expired())  // 20
	{
		key = Key_Get_Pressed() - 1;

		if (key >= 0)
		{
			Player.timer ++;

			//Detect_Crash();
			Move_Map(key);
			Move_Player(key);
			Draw_Map(key);
			Draw_Player(key);
			Move_Bullet(key);
		}

		for (i = 0; i<N; i++)
		{
			if (Zombie[i].life_flag > 0)
			{
				if (Zombie[i].timer == 0)
					Zombie[i].timer = Zombie[i].time_step;
				else
					Zombie[i].timer--;
			}
			if ( ZB_KILL%5 == 0 )
			{
				Zombie[i].time_step -= 5;
				Zombie[i].time_step = (Zombie[i].time_step>=5?Zombie[i].time_step:5);
			}
		}

		Detect_Zombie();  // 사람과 좀비 거리 계산
		Move_Zombie();    // 좀비 그려주기
		Detect_Crash();
		Draw_Zombie();
		Attack_Zombie();
	}

	return;
}


/*--- MAP --------------------------------------------------------------------*/

//void Stage_Changer();

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

	/*
	// MAP CONVERT_MAP1toMAP0
	if ((Back.map_num == 1) && (Player.move_flag == 0) && ((Player.pos.x>460 && Player.pos.x<560) && (Player.pos.y>0 && Player.pos.y<50)))
	{
		Back.map_num = 0;
		Back.w = Maps[Back.map_num][0];
		Back.h = Maps[Back.map_num][1];
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Select_Buffer_Mode(VIRTUAL_MODE);
		Lcd_Select_Display_Frame_Buffer(0);
		Lcd_Select_Draw_Frame_Buffer(0);
		Lcd_Select_Display_Frame_Buffer(1);
		Lcd_Select_Draw_Frame_Buffer(1);

		Call_Map(Back.pos.x - Back.w/2, Back.pos.y, Back.w, Back.h, Maps[Back.map_num]);

		Back.pos.conv2_x = Back.pos.x;
		Back.pos.conv2_y = Back.pos.y;
		Lcd_Set_Virtual_Display_Start(Back.pos.init_x, Back.pos.init_y);
	}

	// MAP CONVERT_MAP1toMAP0
	if ((Back.map_num == 0) && (Player.move_flag == 2) && ((Player.pos.x>110 && Player.pos.x<210) && (Player.pos.y>190 && Player.pos.y <240)))
	{
		Back.map_num = 1;
		Back.w = Maps[Back.map_num][0];
		Back.h = Maps[Back.map_num][1];
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Select_Display_Frame_Buffer(0);
		Lcd_Select_Draw_Frame_Buffer(0);
		Lcd_Select_Display_Frame_Buffer(1);
		Lcd_Select_Draw_Frame_Buffer(1);
//		Call_Map(Back.pos.conv2_x, Back.pos.init_y, Back.w, Back.h, Maps[Back.map_num]);
//		Lcd_Set_Virtual_Display_Start(Back.pos.conv2_x, Back.pos.init_y);
		Lcd_Set_Address(Maps[Back.map_num]);
		Map_init();

		Back.pos.conv2_x = Back.pos.x;
		Back.pos.conv2_y = Back.pos.y;
	}
	*/

	// MAP_1 Option
	if ((Player.pos.x>460) && (Player.pos.x <= 463) && (Player.move_flag == 3))
	{
		Call_Map(Back.pos.x, Back.pos.init_y, Back.w - Back.pos.x, Back.h, Maps[Back.map_num]);
	}
	else if ((Player.pos.x>460) && (Player.pos.x <= 463) && (Player.move_flag == 1))
	{
		Lcd_Draw_BMP(Back.pos.init_x, Back.pos.init_y, Maps[Back.map_num]);
	}

	// 구역별 맵 움직임 제어 // 딱 맞음
	Back.pos.old_x = Back.pos.x;
	Back.pos.old_y = Back.pos.y;

	if((key<4) && PLY_AREA_FLAG)
	{
		// MAP ROLLING
		if (MAP_AREA_FLAG)
		{
			// 디버깅
			printf("	area3\n");

			Back.pos.x += dx[key] * Player.speed_step;
			Back.pos.y += dy[key] * Player.speed_step;

			Back.pos.x = Back.pos.x>0 ? Back.pos.x : Back.pos.old_x;
			Back.pos.y = Back.pos.y>0 ? Back.pos.y : Back.pos.old_y;

			Back.pos.x = Back.pos.x<690 ? Back.pos.x : Back.pos.old_x;
			Back.pos.y = Back.pos.y<360 ? Back.pos.y : Back.pos.old_y;
		}
		else if (AREA_FLAG_1 && (!AREA_FLAG_2))
		{
			// 디버깅
			printf("	area1\n");

			Back.pos.y += dy[key] * Player.speed_step;

			Back.pos.x = Back.pos.x>0 ? Back.pos.x : Back.pos.old_x;
			Back.pos.y = Back.pos.y>0 ? Back.pos.y : Back.pos.old_y;
		}
		else if (AREA_FLAG_2 && (!AREA_FLAG_1))
		{
			// 디버깅
			printf("	area2\n");

			Back.pos.x += dx[key] * Player.speed_step;

			Back.pos.x = Back.pos.x>0 ? Back.pos.x : Back.pos.old_x;
			Back.pos.y = Back.pos.y>0 ? Back.pos.y : Back.pos.old_y;
		}
		else
		{
			// 디버깅
			printf("	area0\n");

			Back.pos.x = Back.pos.old_x;
			Back.pos.y = Back.pos.old_y;
		}
	}
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

	// BATTLE MODE
	if ((key == 4) && (Player.timer >= COOLTIME))
	{
		Uart_Printf("Key: %d\n", key);

		Player.timer = 0;
		Bullet.attack_flag = 2;
		Bullet.weapon_mode = 1; // 기본총알
	}
	if (key == 5 && (Player.timer >= COOLTIME))
	{
		Player.timer = 0;
		Bullet.attack_flag = 2;
		Bullet.weapon_mode = 2; // 유도탄
	}

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
	if (Bullet.attack_flag == 2 )
	{
		Bullet.attack_flag--;

		Bullet.w = Bullets[dir][0];
		Bullet.h = Bullets[dir][1];
		Bullet.speed_step = BL_SPEED;
		Bullet.timer = 0;

		//Timer3_Buzzer_Beep(C2,N16);

		if (dir==0)
		{
			Bullet.pos.init_x = Player.pos.x + Player.w/3 + Bullet.w;
			Bullet.pos.init_y = Player.pos.y - 2*Bullet.h/3;


//			Bullet.pos.init_x = Player.pos.x + Player.w/2 - Bullet.w;
//			Bullet.pos.init_y = Player.pos.y - Bullet.h;
		}
		else if (dir==1)
		{
			Bullet.pos.init_x = Player.pos.x - 2*Bullet.h/3;
			Bullet.pos.init_y = Player.pos.y - 2*Bullet.h;
//			Bullet.pos.init_x = Player.pos.x - Bullet.w;
//			Bullet.pos.init_y = Player.pos.y - Player.w/2;
		}
		else if (dir==2)
		{
			Bullet.pos.init_x = Player.pos.x - Bullet.w;
			Bullet.pos.init_y = Player.pos.y + Player.h + Bullet.h;
//			Bullet.pos.init_x = Player.pos.x + Player.w/2 - Bullet.w;
//			Bullet.pos.init_y = Player.pos.y + Player.h + Bullet.h;
		}
		else if (dir==3)
		{
			Bullet.pos.init_x = Player.pos.x + Player.w + Bullet.w/2;
			Bullet.pos.init_y = Player.pos.y + Player.h - 3*Bullet.h;
//			Bullet.pos.init_x = Player.pos.x + Player.h + Bullet.w;
//			Bullet.pos.init_y = Player.pos.y + Player.w/2;
		}

		Flash.w = Flashes[Player.move_flag][0];
		Flash.h = Flashes[Player.move_flag][1];

		Bullet.pos.x = Bullet.pos.init_x;
		Bullet.pos.y = Bullet.pos.init_y;
		Bullet.pos.old_x = Bullet.pos.x;
		Bullet.pos.old_y = Bullet.pos.y;

		explosion();
	}


	// Bullet Move
	while (Bullet.attack_flag > 0)
	{
		Bullet.timer++;

		if (Detect_Crash()) // 벽 or 좀비 맞았을 때
		{
			//Timer3_Buzzer_Beep(C1_,N16);

			Bullet.pos.old_x = Bullet.pos.x;
			Bullet.pos.old_y = Bullet.pos.y;
			// explosion 지우기
			Erase_Map(Bullet.pos.init_x, Bullet.pos.init_y, Bullet.w, Bullet.h, Maps[Back.map_num]);
			// 좀비 맞은거 지우기
			//Erase_Map(Bullet.pos.old_x, Bullet.pos.old_y, Bullet.w, Bullet.h, Maps[Back.map_num]);

			break;
		}

		Bullet.pos.x += (Bullet.speed_step * dx[dir]);
		Bullet.pos.y += (Bullet.speed_step * dy[dir]);

		if(Bullet.timer >= COOLTIME)
		{
			Bullet.timer = 0;

			Erase_Map(Bullet.pos.old_x, Bullet.pos.old_y, Bullet.w, Bullet.h, Maps[Back.map_num]);
		}
	}

	// WEAPON MODE : 기본 총알
	if (Bullet.weapon_mode % 3 == 1)
	{
		if(cnt_1==0)
		{
			cnt_1 = 5;
			Player.timer = -10;
		}

		cnt_1 --;
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
			// printf("Wall ");

			//Lcd_Set_Shape_Mode(1, BLACK);
//			Lcd_Set_Shape_Mode(1, 0xFFFC);
//			Lcd_Draw_BMP(Bullet.pos.old_x, Bullet.pos.old_y, flash_02);
//			Lcd_Set_Shape_Mode(0,0);
			Bullet.attack_flag = 0;

			return 1;
		}

		// 좀비 //한발당 좀비 하나 죽으니까 여기에 for문 위치하는 것이 맞다.
		for (i=0;i<N; i++)
		{
			if ((Zombie[i].timer == 0) && Bullet.attack_flag )
			{
				// 생명 0일 때 소멸 (Move_Zombie에서 위치업데이트 없음)
				if (Zombie[i].life_flag == 0)
				{
					// 디버깅
					printf("%d Zombie die ", i); // 정상작동

					//Timer3_Buzzer_Beep(D1,N16);

					//Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y-2*Zombie[i].h, Zombie[i].w, Zombie[i].h, Maps[Back.map_num]);
					Erase_Map(Zombie[i].pos.x, Zombie[i].pos.y-Zombie[i].h, Zombie[i].w, 2*Zombie[i].h, Maps[Back.map_num]);
					ZB_KILL--;
					Bullet.attack_flag = 0;

					Zombie[i].life_flag = 2;
					Zombie[i].pos.init_x = rand()*(i+1) % 320 + 150;
					Zombie[i].pos.init_y = rand()*i % 240 + 100;
					Zombie[i].pos.x = Zombie[i].pos.init_x;
					Zombie[i].pos.y = Zombie[i].pos.init_y;
					Zombie[i].pos.old_x = Zombie[i].pos.x;
					Zombie[i].pos.old_y = Zombie[i].pos.y;

					Zombie[i].pos.init_x = Zombie[i].pos.init_x < 100? Zombie[i].pos.init_x: 100;
					Zombie[i].pos.init_y = Zombie[i].pos.init_y < 100? Zombie[i].pos.init_y: 100;

					return 2;
				}

				// dir 0
				if ( (Player.move_flag == 0) && (Flash.pos.init_x - Player.w/2 < Zombie[i].pos.x+Zombie[i].w/2 )\
						&& (Flash.pos.init_x + Player.w/2 > Zombie[i].pos.x+Zombie[i].w/2)\
						&& (Flash.pos.init_y + Player.h > Zombie[i].pos.y))
				{
					// 디버깅
					printf("Shot!!!! %d\n", i);

					--Zombie[i].life_flag;
					Zombie[i].pos.y -= 30;
					Zombie[i].pos.y = Zombie[i].pos.y>0 ? Zombie[i].pos.y: Zombie[i].pos.y+30;
					Bullet.attack_flag = 0;

					//Timer3_Buzzer_Beep(C1,N16);

					// 좀비 맞은거 표시
					Lcd_Set_Shape_Mode(1, WHITE);
					Erase_Map(Zombie[i].pos.x-Zombie[i].w/2, Zombie[i].pos.y-Zombie[i].h, 2*Zombie[i].w, 2*Zombie[i].h, Maps[Back.map_num]);
					Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, Bloods[Player.move_flag]);
					Lcd_Set_Shape_Mode(0,0);
					return 1;
				}

				// dir 1
				else if ((Player.move_flag == 1) && (Flash.pos.init_x > Zombie[i].pos.x )\
						&& (Flash.pos.init_y - Player.w/2 < Zombie[i].pos.y+Zombie[i].h/2)\
						&& (Flash.pos.init_y + Player.w/2 > Zombie[i].pos.y+Zombie[i].h/2))
				{
					printf("Shot!!!! %d\n", i);
					--Zombie[i].life_flag;

					Zombie[i].pos.x -= 30;
					Zombie[i].pos.x = Zombie[i].pos.x>0 ? Zombie[i].pos.x : Zombie[i].pos.x+30;
					Bullet.attack_flag = 0;

					//Timer3_Buzzer_Beep(C1,N16);

					// 좀비 맞은거 표시
					Lcd_Set_Shape_Mode(1, WHITE);
					Erase_Map(Zombie[i].pos.x-Zombie[i].w/2, Zombie[i].pos.y-Zombie[i].h, 2*Zombie[i].w, 2*Zombie[i].h, Maps[Back.map_num]);
					Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, Bloods[Player.move_flag]);
					Lcd_Set_Shape_Mode(0,0);

					return 1;
				}

				// dir 2
				else if ((Player.move_flag == 2) && (Flash.pos.init_y < Zombie[i].pos.y )\
						&& (Flash.pos.init_x - Player.w/2 < Zombie[i].pos.x+Zombie[i].w/2)\
						&& (Flash.pos.init_x + Player.w/2 > Zombie[i].pos.x+Zombie[i].w/2))
				{
					printf("Shot!!!! %d\n", i);
					--Zombie[i].life_flag;
					Zombie[i].pos.y += 30;
					Zombie[i].pos.y = Zombie[i].pos.y>0 ? Zombie[i].pos.y: Zombie[i].pos.y-30;
					Bullet.attack_flag = 0;

					//Timer3_Buzzer_Beep(C1,N16);

					// 좀비 맞은거 표시
					Lcd_Set_Shape_Mode(1, WHITE);
					Erase_Map(Zombie[i].pos.x-Zombie[i].w/2, Zombie[i].pos.y-Zombie[i].h, 2*Zombie[i].w, 2*Zombie[i].h, Maps[Back.map_num]);
					Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, Bloods[Player.move_flag]);
					Lcd_Set_Shape_Mode(0,0);

					return 1;
				}

				// dir 3
				else if ((Player.move_flag == 3) && (Flash.pos.init_x < Zombie[i].pos.x )\
						&& (Flash.pos.init_y - Player.w/2 < Zombie[i].pos.y+Zombie[i].h/2)\
						&& (Flash.pos.init_y + Player.w/2 > Zombie[i].pos.y+Zombie[i].h/2))
				{
					printf("Shot!!!! %d\n", i);
					--Zombie[i].life_flag;
					Zombie[i].pos.x += 30;
					Zombie[i].pos.x = Zombie[i].pos.x>0 ? Zombie[i].pos.x : Zombie[i].pos.x-30;
					Bullet.attack_flag = 0;

					//Timer3_Buzzer_Beep(C1,N16);

					// 좀비 맞은거 표시
					Lcd_Set_Shape_Mode(1, WHITE);
					Erase_Map(Zombie[i].pos.x-Zombie[i].w/2, Zombie[i].pos.y-Zombie[i].h, 2*Zombie[i].w, 2*Zombie[i].h, Maps[Back.map_num]);
					Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, Bloods[Player.move_flag]);
					Lcd_Set_Shape_Mode(0,0);

					return 1;
				}
			}
		}
	}

	return 0;
}


void explosion(void)
{

	Flash.pos.init_x = Bullet.pos.init_x - Bullet.w;
	Flash.pos.init_y = Bullet.pos.init_y - Bullet.h/2;

	Lcd_Set_Shape_Mode(1, BLACK);
	Lcd_Draw_BMP(Flash.pos.init_x, Flash.pos.init_y, Flashes[Player.move_flag]);
	Timer4_Delay(100);
	Erase_Map(Flash.pos.init_x, Flash.pos.init_y, Flash.w, Flash.h,Maps[Back.map_num]);
	Lcd_Set_Shape_Mode(0,0);
}


/*--- ZOMBIE --------------------------------------------------------------------*/

void Detect_Zombie(void)
{
	int i;

	for (i = 0; i < N; i++)
	{
		if ((Zombie[i].timer == 0) && (Zombie[i].life_flag > 0))
		{
			printf("hi ");
			// Control time
			if ((ABS(Player.pos.x - Zombie[i].pos.x) < DIST) && (ABS(Player.pos.y - Zombie[i].pos.y) < DIST))
			{
				//Uart_Printf("Detect_Zombie!\n"); // 정상작동
				Zombie[i].speed_step = Zombie[i].speed_step*(i*3);
				Zombie[i].attack_flag = 0;

				// 디버그
				Uart_Printf("ZB#%d : %d %d\n", i, Zombie[i].pos.x,Zombie[i].pos.y);

			}
			if ((ABS(Player.pos.x - Zombie[i].pos.x) <= 50) && (ABS(Player.pos.y - Zombie[i].pos.y) <= 50))
			{
				Zombie[i].speed_step = ZB_SPEED;
				Zombie[i].attack_flag = 1;

			}
			else
			{
				Zombie[i].speed_step = ZB_SPEED;
				Zombie[i].attack_flag = 0;
			}

			// 디버깅
			if(Zombie[i].attack_flag)
				printf("ZB %d is ATTACK", i);
		}
	}
}


void Move_Zombie(void)
{
	int dir = Player.move_flag;
	int i;

	for (i = 0; (i<N) && (dir<4) ; i++)
	{

		if (ZB_AREA_FLAG(i) && (Zombie[i].timer == 0) && (Zombie[i].life_flag>0))
		{
			Zombie[i].pos.old_x = Zombie[i].pos.x;
			Zombie[i].pos.old_y = Zombie[i].pos.y;

			if(Zombie[i].pos.x < Player.pos.x)
			{
				Zombie[i].move_flag = 3;
				Zombie[i].pos.x += Zombie[i].speed_step;

			}
			else if (Zombie[i].pos.x == Player.pos.x)
			{
				Zombie[i].move_flag = Zombie[i].pos.y < Player.pos.y ? 2:0;
			}
			else
			{
				Zombie[i].move_flag = 1;
				Zombie[i].pos.x -= Zombie[i].speed_step;
			}

			if(Zombie[i].pos.y < Player.pos.y)
			{
				Zombie[i].move_flag = 2;
				Zombie[i].pos.y += Zombie[i].speed_step;
			}
			else if (Zombie[i].pos.y == Player.pos.y)
			{
				Zombie[i].move_flag = Zombie[i].pos.x < Player.pos.x ? 3:1;
			}
			else
			{
				Zombie[i].move_flag = 0;
				Zombie[i].pos.y -= Zombie[i].speed_step;
			}
		}

		else if ((Zombie[i].timer == 0) && (Zombie[i].life_flag>0))  // 조건
		{
			Zombie[i].pos.x = Zombie[i].pos.old_x;
			Zombie[i].pos.y = Zombie[i].pos.old_y;
		}


	}

	return;
}

void Attack_Zombie(void)
{
	int i;

	for(i=0;i<N;i++)
	{
		if(Zombie[i].attack_flag && (Zombie[i].timer == 0) && (Zombie[i].life_flag > 0))
		{
			printf("Player Life : %d\n", Player.life_flag);

			Zombie[i].attack_flag = 0;
			Player.life_flag--;
			//Timer3_Buzzer_Beep(C2_,N16);

			Lcd_Set_Shape_Mode(1, 0xFFFE);
			Lcd_Draw_BMP(Player.pos.x-blood_01[0]/2, Player.pos.y-blood_01[1]/2, palm_01);
			Timer4_Delay(100);
			Erase_Map(Player.pos.x-blood_01[0]/2, Player.pos.y-blood_01[1]/2-30, blood_01[0]+10, blood_01[1]+30, Maps[Back.map_num]);
			Lcd_Set_Shape_Mode(0, 0);
		}
	}
}

void Draw_Zombie(void)
{
	int i;

	Lcd_Set_Shape_Mode(1, 0xFFFE);
	for(i=0;i<N;i++)
	{
		if((Zombie[i].timer == 0) && (Zombie[i].life_flag > 0))
		{
			Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, Zombie[i].w+20, Zombie[i].h, Maps[Back.map_num]);
			Lcd_Draw_BMP(Zombie[i].pos.x, Zombie[i].pos.y, arr_Zombie[(Zombie[i].move_flag * 3 + i)%12]);
		}
	}
	Lcd_Set_Shape_Mode(0, 0);
}


int Count_Zombie(void)
{

	Erase_Map(Back.pos.old_x, Back.pos.old_y, 120, 80, Maps[Back.map_num]);

	Lcd_Set_Shape_Mode(1, BLACK);
	Lcd_Printf(Back.pos.x + 10,Back.pos.y+10,RED,BLACK, 1,1, "LIFE   %d", Player.life_flag);
	Lcd_Printf(Back.pos.x + 10,Back.pos.y+25,WHITE,BLACK, 1,1, "REMAIN %d", ZB_KILL);
	Lcd_Set_Shape_Mode(0, 0);

	if (ZB_KILL == 0)
	{
		Game_Out(1); // WIN
		return 1; // 우선은 --> 추후 다음스테이지로 넘어가게 수정
	}

	else if (Player.life_flag <= 0)
	{
		Game_Out(0);
		return 1;
	}

	return 0;
}


/*===============================================================================*/

int Game_Out(int ver)
{
	// Game Over
	if (ver == 0)
	{
		Erase_Map(Player.pos.old_x, Player.pos.old_y, Player.w, 2 * Player.h, Maps[Back.map_num]);
		Timer4_Delay(5000);
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Draw_BMP(0, 0, Endings[ver]);
		Lcd_Set_Virtual_Display_Start(0,0);
		//Lcd_Draw_BMP(Back.pos.x, Back.pos.y, Endings[ver]);

		Key_Wait_Key_Pressed();

		return 1;

	}
	// Win
	if (ver == 1)
	{
		int i;

		for(i=0;i<N;i++)
		{
			Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, Zombie[i].w+20, Zombie[i].h, Maps[Back.map_num]);
		}
		Timer4_Delay(5000);
		Lcd_Clr_Screen(BACK_COLOR);
		Lcd_Set_Shape_Mode(1, 0xFFFE);
		//Lcd_Draw_BMP(Back.pos.x, Back.pos.y, Endings[ver]);
		Lcd_Draw_BMP(0, 0, Endings[ver]);
		Lcd_Set_Virtual_Display_Start(0,0);
		Lcd_Set_Shape_Mode(0,0);

		Key_Wait_Key_Pressed();

		return 1;
	}
	return 0;
}

#endif


