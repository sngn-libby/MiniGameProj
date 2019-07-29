LCD : 640x240

960x240 (LCD Frame)

1920x480(Virtual Frame)


Lcd_Select_Buffer_Mode ---> virtual mode로 진행해야 한다.

Lcd_Set_Virtual_Display_Start(xpos,ypos); // 보여지는 시작 위치(Virtual Image 전체에서)

VirtualFrame보다 더 크게 그리고싶다면

Lcd_Select_Display_Frame_Buffer(1); ---> Latch #1 프레임

Lcd_Select_Display_Frame_Buffer(0); ---> Latch #2 프레임

Lcd_Select_Display_Frame_Buffer(1); ---> Latch #3 프레임

Lcd_Select_Display_Frame_Buffer(0); ---> Latch #4 프레임

이런식으로



```c
void Main(void)
{

	MMU_Init();
	Led_Init(); // port 복원용 (key 저장)
	Key_Poll_Init();
	Uart_Init(115200);
	Timer0_Init();
	Timer3_Buzzer_Init();
	Timer4_Init();
	Lcd_Graphic_Init();
	Lcd_Clr_Screen(0x0000);

//	Uart_Printf("Welcome GBOX World!\n");
//	User_Main();
	Character_move();
//	Uart_Printf("Good Bye~\n");
}
```





```ㅊ
void Simple_Example(void)
{
	Game_Init(); // 화면을 검정색으로 전환시킨다 // Timer0_Repeat(20)으로시작
	Draw_Object(); // 

	for(;;)
	{
		Move_Object();
		collision_detect();
		Draw_Object();


	}

}
```

