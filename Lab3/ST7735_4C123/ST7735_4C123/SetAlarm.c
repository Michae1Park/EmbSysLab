// SetTime.c
// Michael Park, Jack Zhao
// Date Created: 02/10/2016
// class for functions to set time
// Lab Number: 16340
// TA: Mahesh Srinivasan
// Last Revised: 02/15/16

#include <stdio.h>
#include <stdint.h>
#include "SetAlarm.h"
#include "../inc/tm4c123gh6pm.h"

volatile uint16_t alarm_hours, alarm_minutes;
volatile uint16_t alarm_flag;
//global: hour, min, alarmflag, displayflag, switchflag(for interrupt)

void setAlarmTimeInit(void)
{
//disable interrupts
}

//Assuming Time interrupt is disalbed
void setAlarmTimeBase(uint32_t choice)
{
    switch(choice) 
    {
      case 0:
        incrementAlarmHour();
        break;
      case 1:
        decrementAlarmHour();
        break;
      case 2:
        incrementAlarmMin();
        break;
      case 3:
        decrementAlarmMin();
        break;
      //default:
        //incrementHour();  //increment both for debugging
        //incrementMin();
		}

}

void incrementAlarmHour(void)
{
  if((++alarm_hours)>23) {alarm_hours = 0;} //if hour exceeds 23, reset to 0
  else {alarm_hours++;}
}

void decrementAlarmHour(void)
{
  if((--alarm_hours)<0) {alarm_hours = 23;} //if hour below 0, reset to 23
  else {alarm_hours--;}
}

void incrementAlarmMin(void)
{
  if((++alarm_minutes)>59) {alarm_minutes = 0;} //if min exceeds 59, reset to 0
  else {alarm_minutes++;}
}

void decrementAlarmMin(void)
{
  if((--alarm_minutes)<0) {alarm_minutes = 59;} //if min below 0, reset to 59
  else {alarm_minutes--;}
}
void AlarmToggleON(void){

		if(alarm_flag==0)
			alarm_flag =1;

	
	
}
void AlarmToggleOFF(void){
			if(alarm_flag==1)
			alarm_flag =0;
	
}