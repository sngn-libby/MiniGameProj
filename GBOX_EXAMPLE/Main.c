/***********************************************************/
// 다음은 사용자 함수를 호출하는 루틴임 지우지 말것!
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

// available map area condition
#define AREA_FLAG (((Player.pos.x>160) && (Player.pos.x<690)) || ((Player.pos.y>120) && (Player.pos.y < 360))) ? 1:0
#define AREA_FLAG_1 (((Player.pos.x>85) && (Player.pos.x<160)) || ((Player.pos.x>690) && (Player.pos.x<765))) ? 1:0
#define AREA_FLAG_1_REV ((Player.pos.x>160) && (Player.pos.x<690)) ? 1:0
#define AREA_FLAG_2 (((Player.pos.y>50) && (Player.pos.y < 120)) || ((Player.pos.y>360) && (Player.pos.y < 420))) ? 1:0
#define AREA_FLAG_2_REV ((Player.pos.y>120) && (Player.pos.y < 360)) ? 1:0
#define PLY_AREA_FLAG ((Player.pos.x>85) && (Player.pos.x<765) && (Player.pos.y>50) && (Player.pos.y < 420)) ? 1:0

// Collision
#define CRASH_X(I) ((Bullet.pos.x >= Zombie[(I)].pos.x + (Zombie[(I)].w/3)) && (Bullet.pos.x<= Zombie[(I)].pos.x+(Zombie[(I)].w*2/3))) ? 1:0
#define CRASH_Y(I) ((Bullet.pos.y >= Zombie[(I)].pos.y + (Zombie[(I)].h/3)) && (Bullet.pos.y<= Zombie[(I)].pos.y+(Zombie[(I)].h*2/3))) ? 1:0

// Zombie
#define ACCEL(i) (Zombie(i).move_flag ? Zombie(i).speed_step * (i) : 0)

#include "./Image/surgery_480.h"
#include "./Image/road.h"

#include "./Image/man_01.h"
#include "./Image/man_02.h"
#include "./Image/man_03.h"
#include "./Image/man_04.h"

#include "./Image/bullet_01.h"

const unsigned short * Maps[] = {surgery_480, road};
const unsigned short * Man[] = {man_01, man_02, man_03,man_04};
const unsigned short * Bullets[] = {bullet_01,};

/*------------------------------------------------------------------------------*/

#define N (10)

typedef struct str{
	int x, y;
	int old_x, old_y;
	int init_x, init_y;
}POS;

// monster 만들기
typedef struct obj
{
	int timer;
	int move_flag;
	POS pos;
	short w,h;
	unsigned short color;
	int speed_step;
	int move_step;
	int attack_flag;
}OBJ;

OBJ Player={0,0,{130,100, 130,100, 130,100},0,0, BLUE, 3, 8, 0 };
OBJ Zombie[N]={0,1,{350,120, 350,120, 350,120},30,30, RED, 1, 3, 0 };
POS map_pos = {0,0, 0,0, 0,0};

OBJ Bullet={0,};

/*------------------------------------------------------------------------------*/

void Game_Init(void);
void Map_init(void);
void Character_init(void);
void Monster_init(void);

void Move(void);
void Draw_Character(int key);
void Draw_Map(int key);
void Draw_Monster(int key);
void Draw_Bullet(int key);
void explosion(void);
void Collision_Detect(void);
void Monster_Detect(void);


void Erase_Map(int x, int y, int w, int h, const unsigned short int *fp);


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

	Game_Init();
	//Timer0_Repeat(30);
	for(;;)
	{
		Move();
	}
//	Objects();

}

/*===============================================================================*/



//void Map_init(void);



void Game_Init(void)
{
	Lcd_Clr_Screen(BACK_COLOR);
	Timer0_Repeat(20);

	Map_init();
	Character_init();
	Monster_init();
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
	Lcd_Set_Virtual_Display_Start(0,0);
	map_pos.x = 0;
	map_pos.y = 0;
	//Timer4_Delay(1000);
}


void Character_init(void)
{
	Player.move_flag = 0;
	Player.w = Man[Player.move_flag][0];
	Player.h = Man[Player.move_flag][1];

	Player.pos.init_x = (320-Player.w)/2;
	Player.pos.init_y = (240-Player.h)/2;
	Player.pos.x = (320-Player.w)/2;
	Player.pos.y = (240-Player.h)/2;

	Lcd_Set_Shape_Mode(1,0xFFFE);
	Lcd_Draw_BMP(Player.pos.init_x, Player.pos.init_y, Man[Player.move_flag]);
	Lcd_Set_Shape_Mode(0,0);
}


void Monster_init(void)
{
	int i;

	for(i=0;i<N;i++)
	{
		int rand_x = srand() / LCD_XSIZE;
		int rand_y = srand() / LCD_YSIZE;

		Zombie[i].pos.init_x = rand_x;
		Zombie[i].pos.init_y = rand_y;
		Zombie[i].pos.x = Zombie[i].pos.init_x;
		Zombie[i].pos.y = Zombie[i].pos.init_y;
		Zombie[i].move_flag = 0;

		Lcd_Draw_Bar(Zombie[i].pos.init_x, Zombie[i].pos.init_y, Zombie[i].pos.init_x +Zombie[i].w, Zombie[i].pos.init_y +Zombie[i].h, Zombie[i].color);
		//Lcd_Draw_BMP(Player.pos.x, Player.pos.y, man_04);
	}
}


/*===============================================================================*/

void Move(void)
{
	int i;
	int key = Key_Get_Pressed()-1;

	if((key>=0)&&Timer0_Check_Expired())
	{
		Uart_Printf("Key: %d\n", Key_Get_Pressed());
		Uart_Printf("map_pos X =[%d], Y=[%d]\n", map_pos.x, map_pos.y);
		Uart_Printf("Player_pos X =[%d], Y=[%d]\n", Player.pos.x, Player.pos.y);

		//Collision_Detect(key);
		Draw_Map(key);
		Draw_Character(key);
		Draw_Monster(key);
	}

	return;
}


void Erase_Map(int x, int y, int w, int h, const unsigned short int *fp)
{
	register int map_w = fp[0];
	int i,j;

	for(i=y; i<y+h; i++)
	{
		for(j=x; j<x+w; j++)
		{
			Lcd_Put_Pixel(j, i, (int)fp[map_w*i+j+2]);
		}
	}
}

void Draw_Map(int key)
{
	int dy[] = {-1, 0, 1, 0};
	int dx[] = {0, -1, 0, 1};

	Uart_Printf("%d ", key);
	Uart_Printf("%d\n", AREA_FLAG);
	Uart_Printf("1: %d ", AREA_FLAG_1);
	Uart_Printf("2: %d\n", AREA_FLAG_2);

	if( (key<4) && AREA_FLAG )
	{
		// Area2 : x이동 y유지
		if(AREA_FLAG_1_REV && AREA_FLAG_2 )
		{
			Uart_Printf("area2\n");
			map_pos.old_x = map_pos.x;
			map_pos.x += dx[key]*Player.speed_step;

			map_pos.y = map_pos.old_y;
		}
		// Area1 : x유지 y이동
		else if( AREA_FLAG_1 && AREA_FLAG_2_REV )
		{
			Uart_Printf("area1\n");
			map_pos.x = map_pos.old_x;

			map_pos.old_y = map_pos.y;
			map_pos.y += dy[key]*Player.speed_step;

		}
		// Area3 : x이동 y이동
		else if( (!AREA_FLAG_1)&&(!AREA_FLAG_2) )
		//else if( AREA_FLAG_1_REV && AREA_FLAG_2_REV )
		{
			Uart_Printf("area3\n");
			map_pos.old_x = map_pos.x;
			map_pos.old_y = map_pos.y;
			map_pos.x += dx[key]*Player.speed_step;
			map_pos.y += dy[key]*Player.speed_step;
		}

		Lcd_Set_Virtual_Display_Start(map_pos.x, map_pos.y);
	}
	// Area4 : 맵 고정
	else
	{
		Uart_Printf("else\n");
		map_pos.x = map_pos.old_x;
		map_pos.y = map_pos.old_y;
	}

	//Lcd_Set_Virtual_Display_Start(map_pos.x, map_pos.y);
}


void Draw_Character(int key)
{
	int dy[] = {-1, 0, 1, 0};
	int dx[] = {0, -1, 0, 1};

	if(key<4)
	{
		// 캐릭터 방향 전환
		Player.move_flag = key;
//		Lcd_Set_Shape_Mode(1,0xFFFE);
//		Lcd_Draw_BMP(Player.pos.x, Player.pos.y, Man[Player.move_flag]);
//		Lcd_Set_Shape_Mode(0,0);

		// 이동
		if(PLY_AREA_FLAG)
		{
			Player.pos.old_x = Player.pos.x;
			Player.pos.old_y = Player.pos.y;
			Player.pos.x += dx[key]*Player.speed_step;
			Player.pos.y += dy[key]*Player.speed_step;

			Player.pos.x = Player.pos.x>0 ? Player.pos.x:Player.pos.old_x;
			Player.pos.y = Player.pos.y>0 ? Player.pos.y:Player.pos.old_y;
		}
		else
		{
			Erase_Map(Player.pos.x, Player.pos.y, Player.w, Player.h, Maps[0]);
			Player.pos.x = Player.pos.old_x;
			Player.pos.y = Player.pos.old_y;
		}

		Lcd_Set_Shape_Mode(1,0xFFFE);
		Erase_Map(Player.pos.old_x, Player.pos.old_y, Player.w, Player.h, Maps[0]);
		Lcd_Draw_BMP(Player.pos.x, Player.pos.y, Man[Player.move_flag]);
		Lcd_Set_Shape_Mode(0,0);
	}

}

void Draw_Bullet(int key)
{
	// init
	if(key == 5)
	{
		Bullet.attack_flag = 1;
		Bullet.pos.x = Player.pos.x+(Player.w/2);
		Bullet.pos.y = Player.pos.y;
		Bullet.speed_step = 30;
//		Bullet.pos.x = Bullet.pos.init_x;
//		Bullet.pos.y = Bullet.pos.init_y;

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, bullet_01);
		Lcd_Set_Shape_Mode(0, 0);
	}

	if(Bullet.attack_flag)
	{
		if(Player.move_flag == 0)
			Bullet.pos.y -= Bullet.speed_step;
		else if(Player.move_flag == 1)
			Bullet.pos.x -= Bullet.speed_step;
		else if(Player.move_flag == 2)
			Bullet.pos.y += Bullet.speed_step;
		else if(Player.move_flag == 3)
			Bullet.pos.x -= Bullet.speed_step;

		Lcd_Set_Shape_Mode(1, 0xFFFE);
		Lcd_Draw_BMP(Bullet.pos.x, Bullet.pos.y, bullet_01);
		Lcd_Set_Shape_Mode(0, 0);

		Collision_Detect();
	}
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

void Monster_Detect(void)
{
	int i;
	
	for (i = 0; i < N; i++)
	{
		if (((Player.pos.x - Zombie.pos.x) < 200) && ((Player.pos.y - Zombie.pos.y) < 150))
		{
			Zombie[i].move_flag = 1;
		}
	}
}

void Collision_Detect(void)
{
	int i;

	if(Bullet.attack_flag == 1)
	{
		for(i=0;i<N;i++)
		{
			if(CRASH_X(i) && CRASH_Y(i))
			{
				Bullet.attack_flag = 0;
				Erase_Map(Zombie[i].pos.x, Zombie[i].pos.y, Zombie[i].w, Zombie[i].h, Maps[0]);
			}
		}
	}

}

void Draw_Monster(int key)
{
	int dy[] = {-1, 0, 1, 0};
	int dx[] = {0, -1, 0, 1};
	int i;

	for(i=0;i<N;i++)
	{
		if((key<4)&&(PLY_AREA_FLAG))
		{
			if((Zombie[i].pos.y<0)||(Zombie[i].pos.x<0)){
				Zombie[i].pos.x = Zombie[i].pos.old_x;
				Zombie[i].pos.y = Zombie[i].pos.old_y;
			}

			Zombie[i].pos.old_x = Zombie[i].pos.x;
			Zombie[i].pos.old_y = Zombie[i].pos.y;
			Zombie[i].pos.x += dx[key]*(Player.speed_step + ACCEL(i));
			Zombie[i].pos.y += dy[key]*(Player.speed_step + ACCEL(i));


			//	for(i=0; i<(sizeof(Man)/sizeof(Man[1])); i++)
			//	{
			//		Lcd_Get_Info_BMP(&xtmp, &ytmp, Man[1]); // 그림크기설정
			//		Uart_Printf("X=[%d], Y=[%d]\n", xtmp, ytmp);
			//		Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), Man[]); // 불러올 위치 설정
			//		Timer4_Delay(1000);
			//	}
		}

		else
		{
			Erase_Map(Zombie[i].pos.x, Zombie[i].pos.y, Zombie[i].w, Zombie[i].h, Maps[0]);
			Zombie[i].pos.x = Zombie[i].pos.old_x;
			Zombie[i].pos.y = Zombie[i].pos.old_y;
		}

		Erase_Map(Zombie[i].pos.old_x, Zombie[i].pos.old_y, Zombie[i].w, Zombie[i].h, Maps[0]);
		Lcd_Draw_Bar(Zombie[i].pos.x, Zombie[i].pos.y, Zombie[i].pos.x+Zombie[i].w, Zombie[i].pos.y+Zombie[i].h, Zombie[i].color);
	}
}

#endif


