- **LCD SIZE : 320X240**

- void **Basic_Graphics_Test**(void) : key pressed로 순차진행

  - Clear Screen
  - Draw Lines (수직, 수평, 대각선) _ 
    - Lcd_Draw_Hline(y, x시작,x끝, Colour)
    - Lcd_Draw_Hline(x, y시작,y끝, Colour)
  - Lcd_Draw_Rect(x1,x2,y1,y2,colour) 사각형 아웃라인
  - Lcd_Draw_Bar(x1,x2,y1,y2,colour) 색칠된 사각형
  - Lcd_Draw_BMP(x1, y1, 불러올 이미지)  화면에 출력될 그림 크기
    

- void **Special_Mode_Test**(void)

  - Lcd_Set_Trans_Mode(0/1) 켜고 끄기

    ```C
    Lcd_Set_Trans_Mode(1);
    Lcd_Draw_Bar(0,0,100,100,RED);
    Lcd_Draw_Bar(50,50,200,200,BLUE);
    Lcd_Set_Trans_Mode(0);
    
    ```

  - Lcd_Set_Shape_Mode(1,0xFFFE); (ON/OFF, 바꿀 색)

    ```C
    Lcd_Set_Shape_Mode(1,0xFFFE);
    Lcd_Clr_Screen(GREEN);
    // 0xFFFE(White, target Colour)를 GREEN으로 바꾸겠다.
    Lcd_Get_Info_BMP(&xtmp, &ytmp, shape); // 그림 크기 설정
    Lcd_Draw_BMP((160-(xtmp/2)), (120-(ytmp/2)), shape); // 사진을 불러올 위치 설정
    Lcd_Set_Shape_Mode(0,0); // 다음 그림에 적용되지 않도록 OFF
    ```

  - Uart_Printf("키보드 누르면 종료\n"); Tera Term에 글자 출력

  - Lcd_Printf(x,y,글자색,배경색,가로비율,세로비율,"윌텍 임베디드 그룹"); LCD에 글자 출력

  - Lcd_Put_Pixel(x, y, BLUE);

    ```C
    for(i=0; i<10; i++)
    	{
    		for(j=0; j<10; j++)
    		{
    			Lcd_Put_Pixel(50+i, 80+j, BLUE);
    		}
    	}
    ```

- void **Brightness_Test**(void)

  - b = Uart_GetIntNum(); 밝기 조절 숫자 입력받음
  - Lcd_Control_Brightness(b); 밝기 출력

- void **Buzzer_Test**(void) 

  - 0xff는 Buzzer 종료 주소

  - key (음) note(박자) --> {key, note} 배열로 만들어서 사용

    ```C
    	while(song[i][0] != 0xff)
    	{
    		Timer3_Buzzer_Beep(song[i][0], song[i][1]);
    		Uart_Printf("."); // 음이 바뀔때마다 테라텀에 .출력하기
    		i++;
    	}
    ```

- void **Key_Test**(void)

  - Key_Wait_Key_Pressed_Multi(); 

    ```C
    int Key_Wait_Key_Pressed(void)
    {
    	int k;
    	
    	while((k=Key_Get_Pressed())==0);
    	return k;
    }
    
    void Key_Wait_Key_Released(void)
    {
    	while(Key_Get_Pressed()); 
        // (0 - default / 키를 입력할 때 Interrupt되는 방식)
    }
    ```

  - Key_Get_Pressed(); 

  - Key_Get_Pressed_Multi(); --- 거의 사용하기 힘듦

    - 방향키 1,2,3,4 : 0x1,0x2,0x4,0x8
    - 5,6,7,8, : 0x10,0x20,0x40,0x80
    - 9 : 0x100
    - 조합하면 더해져서 나옴

  - Key_Wait_Key_Released_Multi(); 키가 release될 때까지 다른 키입력을 받지 않는다.
      

- void **Timer_Test**(void)

  - **1초 = 1000ms**

  - wait 주기 : 

    - `while(!Uart_Get_Pressed());` // 키보드 입력 받는것
    - `while(!Key_Get_Pressed());` // 조이스틱 입력 받는것

  - Timer1_Stopwatch_Run();   //스탑워치 시작

  - Timer1_Stopwatch_Stop(); // 스탑워치 끝

  - Timer 주기적으로 동작시키기

    ```C
    Timer0_Repeat(500); // 600ms마다 Timer를 갱신시켜준다 (계속 실행된다)
    // 0~1300ms 내의 범위로 설정한다.
    // Repeat주기를 바꿔주고 싶으면 Timer0_Change_Delay(100*(i+1));등으로 지연시켜준다.
    	disp = 0xf;
    
    	for(i=1;;)
    	{
    		if(Timer0_Check_Expired()) // Timer0_Repeat가 만료되면 자동으로 1이 Tiemr0_Check_Expired()에 들어간다.
    		{
    			Led_Display(disp = ~disp);
    			Uart_Printf("[%d]\n", i);
    			i++;
    			if(i>10) break;
    		}
    	}
    ```

- Timer(3조)

  - <Timer_Test>

    void Timer0_Init(void)

    - Timer0 초기화

    void Timer4_Init(void)

    - Timer4 초기화


    ============================================================

    

    void Timer0_Repeat(int time)

    - 시간을 밀리초 단위로 체크

    - 1.3초를 넘어가면 문제 발생


    void Timer0_Change_Delay(int msec)

    - 타이머 딜레이를 바꾸어줌
    - rTCNTB0: 타이머 반복 단위를 바꾸어줌
      rTCMPB0: 

    

    int Timer0_Check_Expired(void)

    - 타이머 반복 단위가 될 때마다 1 리턴, 그 외에는 0 리턴

    

    

    void Timer0_Stop(void)

    - 타이머 종료

    -----------------------------------------------------------

    void Timer0_ISR_Enable(int en) // 토글된다.

    ===========================================================

    

    void Timer1_Stopwatch_Run(void)

    - 0부터 시작해서 증가

    

    int Timer1_Stopwatch_Stop(void)

    - 100만분의 일초 단위로 리턴

    

    ===========================================================

    

    void Timer4_Delay(unsigned int msec)

    - ms단위로 전체 프로그램 일시정지