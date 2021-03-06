// ATimeDisplay.c
// Michael Park, Jack Zhao
// Date Created: 2/12/2016
// Includes analog and digital time display functions
// Lab Number: 16340
// TA: Mahesh Srinivasan
// Last Revised: 2/15/2016

#include <stdint.h>
#include <stdio.h>
#include "../Shared/tm4c123gh6pm.h"
#include "ST7735.h"
#include <math.h>
#include "TimeDisplay.h"
#include "Timer1.h"
#include "SetAlarm.h"
#include "Common.h"
#include "Systick.h"
#include "Stopwatch.h"

#define CENTER_X 64
#define CENTER_Y 80
#define LENGTH_S 45.0
#define LENGTH_M 45.0
#define LENGTH_H 45.0
#define PI 3.14159265358979323846

 
extern long StartCritical (void);    // previous I bit, disable interrupts
extern void EndCritical(long sr);    // restore I bit to previous value
void DelayWait10ms(uint32_t n);
void EraseSecond(void);

//extern volatile uint16_t Time_Minutes, Time_Hours; 
volatile uint16_t display_mode = 0;
//volatile double x, y;
volatile static double prevsx, prevsy, prevmx, prevmy, prevhx, prevhy;
extern volatile uint16_t Stopwatch_Triggered;
extern volatile uint16_t alarm_minutes, alarm_hours;
extern volatile uint16_t Time_minutes, Time_hours;
extern volatile uint16_t alarm_flag;
void DisplayAlarm(void)
{
	char AsciiArray[] = {'0','1','2','3','4','5','6','7','8','9'};
	char ch[5];
	ch[0] = AsciiArray[alarm_hours/10];
	ch[1] = AsciiArray[alarm_hours%10];
	ch[2] = ':';
	ch[3] = AsciiArray[alarm_minutes/10];
	ch[4] = AsciiArray[alarm_minutes%10];
	
	Output_Clear();
	ST7735_DrawCharS(0, 50, ch[0], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(25, 50, ch[1], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(50, 50, ch[2], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(75, 50, ch[3], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(100, 50, ch[4], ST7735_YELLOW, ST7735_BLACK, 4);
}

void DisplaySetTime(void)
{
	char AsciiArray[] = {'0','1','2','3','4','5','6','7','8','9'};
	char ch[5];
	ch[0] = AsciiArray[Time_Hours/10];
	ch[1] = AsciiArray[Time_Hours%10];
	ch[2] = ':';
	ch[3] = AsciiArray[Time_Minutes/10];
	ch[4] = AsciiArray[Time_Minutes%10];
	
	Output_Clear();
	ST7735_DrawCharS(0, 50, ch[0], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(25, 50, ch[1], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(50, 50, ch[2], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(75, 50, ch[3], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(100, 50, ch[4], ST7735_YELLOW, ST7735_BLACK, 4);
}

void DisplaySecond(void)
{
	volatile static double x, y;
	x = 0;
	y = 0;
	x = CENTER_X + LENGTH_S*cos(Time_Seconds*6*(PI/180)-1.58);
	y = CENTER_Y + LENGTH_S*sin(Time_Seconds*6*(PI/180)-1.58);
	prevsx = x;
	prevsy = y;
	
	ST7735_Line(CENTER_X, CENTER_Y, x, y, ST7735_WHITE);
}

void EraseSecond(void)
{
	if (((prevsx==prevmx)&&(prevsy==prevmy)) || ((prevsx==prevmx)&&(prevsy==prevmy))){}
	else{ ST7735_Line(CENTER_X, CENTER_Y, prevsx, prevsy, ST7735_BLACK);}
}
void DisplayMinute(void)
{
	volatile static double x, y;
	x = 0;
	y = 0;
	x = CENTER_X + LENGTH_M*cos(Time_Minutes*6*(PI/180)-1.58);
	y = CENTER_Y + LENGTH_M*sin(Time_Minutes*6*(PI/180)-1.58);
	prevmx = x;
	prevmy = y;
	
	ST7735_Line(CENTER_X, CENTER_Y, x, y, ST7735_WHITE);
}

void EraseMinute(void)
{
	if (((prevmx==prevhx)&&(prevmy==prevhy))){}
	else{ ST7735_Line(CENTER_X, CENTER_Y, prevmx, prevmy, ST7735_BLACK);}
}

void DisplayHour(void)
{
	volatile static double x, y;
	x = 0;
	y = 0;
	x = CENTER_X + LENGTH_H*cos(Time_Hours*30*(PI/180)-1.58);
	y = CENTER_Y + LENGTH_H*sin(Time_Hours*30*(PI/180)-1.58);
	prevhx = x;
	prevhy = y;
	
	ST7735_Line(CENTER_X, CENTER_Y, x, y, ST7735_WHITE);
}

void EraseHour(void)
{
	ST7735_Line(CENTER_X, CENTER_Y, prevhx, prevhy, ST7735_BLACK);
}

void ChooseMode(void)
{
	if (display_mode==1) {DisplayAnalog();}
	else if(display_mode==0) {DisplayDigital();}
	else {}
}

void DisplayAnalog(void)
{int32_t timeDisplay;
	timeDisplay=NVIC_ST_CURRENT_R;
		Output_Clear();
		ST7735_SetCursor(0, 0);
		ST7735_OutString("alarm:");
		ST7735_OutUDec(alarm_hours);
		ST7735_OutString(":");
		ST7735_OutUDec(alarm_minutes);
		
		ClockFace_Init();
		DisplaySecond();
		DisplayMinute();
		DisplayHour();
		timeDisplay=timeDisplay-NVIC_ST_CURRENT_R;
		while(active_In10s)
		{
			timeDisplay=NVIC_ST_CURRENT_R;
			if(Mode == 3)
			{
				break;
			}
			if(displayFlag == 0x01) 
			{
				displayFlag = 0xFF;
				EraseSecond();
				DisplaySecond();
			}
			if(displayFlag == 0x03) 
			{
				displayFlag = 0xFF;
				EraseSecond();
				DisplaySecond();
				EraseMinute();
				DisplayMinute();
			}
			if(displayFlag == 0x07) 
			{
				displayFlag = 0xFF;
				EraseSecond();
				DisplaySecond();
				EraseMinute();
				DisplayMinute();
				EraseHour();
				DisplayHour();
			}
			timeDisplay=NVIC_ST_CURRENT_R-timeDisplay;
			timeDisplay++;
		}	
}

void DisplayDigital(void)
{//int32_t timeDisplay;
	//timeDisplay=NVIC_ST_CURRENT_R;
	char AsciiArray[] = {'0','1','2','3','4','5','6','7','8','9'};
	char ch[5];
	
	ch[0] = AsciiArray[Time_Hours/10];
	ch[1] = AsciiArray[Time_Hours%10];
	ch[2] = ':';
	ch[3] = AsciiArray[Time_Minutes/10];
	ch[4] = AsciiArray[Time_Minutes%10];
	
	Output_Clear();
	ST7735_SetCursor(0, 0);
	ST7735_OutString("alarm:");
	ST7735_OutUDec(alarm_hours);
	ST7735_OutString(":");
	ST7735_OutUDec(alarm_minutes);

	ST7735_DrawCharS(0, 50, ch[0], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(25, 50, ch[1], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(50, 50, ch[2], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(75, 50, ch[3], ST7735_YELLOW, ST7735_BLACK, 4);
	ST7735_DrawCharS(100, 50, ch[4], ST7735_YELLOW, ST7735_BLACK, 4);
	//timeDisplay=timeDisplay-NVIC_ST_CURRENT_R;
	
	while(active_In10s)
	{
		//timeDisplay=NVIC_ST_CURRENT_R;
		if(Mode == 3)
		{
			break;
		}
		if(Mode==0){
			if((Time_Minutes+5)<60){
				alarm_minutes=Time_Minutes+5;
			alarm_hours=Time_Hours;
				alarm_flag=1;
			}

		}

			ch[0] = AsciiArray[Time_Hours/10];
			ch[1] = AsciiArray[Time_Hours%10];
			ch[2] = ':';
			ch[3] = AsciiArray[Time_Minutes/10];
			ch[4] = AsciiArray[Time_Minutes%10];
			
			if(displayFlag == 0x03) 
			{
				displayFlag = 0xFF;
				Output_Clear();
				ST7735_DrawCharS(0, 50, ch[0], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(25, 50, ch[1], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(50, 50, ch[2], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(75, 50, ch[3], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(100, 50, ch[4], ST7735_YELLOW, ST7735_BLACK, 4);
			}
			if(displayFlag == 0x07) 
			{
				displayFlag = 0xFF;
				Output_Clear();
				ST7735_DrawCharS(0, 50, ch[0], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(25, 50, ch[1], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(50, 50, ch[2], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(75, 50, ch[3], ST7735_YELLOW, ST7735_BLACK, 4);
				ST7735_DrawCharS(100, 50, ch[4], ST7735_YELLOW, ST7735_BLACK, 4);
			}
			//timeDisplay=timeDisplay-NVIC_ST_CURRENT_R;
			//timeDisplay++;
		}	
}

void ClockFace_Init(void)
{
    uint32_t r = 60;
    ST7735_DrawCircle(64, 80, r, ST7735_Color565(0, 0, 255));  // clock face
		ST7735_DrawCharS(57, 22, '1', ST7735_Color565(255, 0, 0), 0, 1);
    ST7735_DrawCharS(62, 22, '2', ST7735_Color565(255, 0, 0), 0, 1);
		ST7735_DrawCharS(117, 77, '3', ST7735_Color565(255, 128, 0), 0, 1);
    ST7735_DrawCharS(61, 132, '6', ST7735_Color565(255, 0, 0), 0, 1);
    ST7735_DrawCharS(6, 77, '9', ST7735_Color565(255, 0, 0), 0, 1);
    DelayWait10ms(1);     
}

// Subroutine to wait 10 msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWait10ms(uint32_t n)
{
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}
