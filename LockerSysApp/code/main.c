/*
 * LCDapp.c
 *
 * Created: 12/4/2021 2:43:05 PM
 * Author : Abdalrahman_Amin
 */ 

#include "header.h"
#include "LCD.h"
#include "EEPROM.h"
#include "KEYPAD.h"
#include "PASSWORD.h"


int main(void)
{
    /* Replace with your application code */
	SETBIT(DDRD,0);// pin for buzzer
	KEYPAD_init();
	LCD_init();
	
	uint8_t password_saved[4],Entered_pass[10];
	for(uint8_t i=0;i<4;i++)
	{
		password_saved[i]=EEPROM_read(100+i);
		//if (password_saved[i]==255){password_saved[i]='0';}
	}
	
	while (1) 
    {	
		LCD_write_command(0x80);
		LCD_write_string("Enter_Password:");
		LCD_write_command(0xC2);
		while(! KEYPAD_read());
		read_entered_pass(&Entered_pass[0]);			
		
		if(compare_password(&password_saved[0],&Entered_pass[0])==1)
		{	
			while(! KEYPAD_read());
			if (KEYPAD_read()=='/')
			{
				while(KEYPAD_read());
				change_password(&password_saved[0],&Entered_pass[0]);
			}
			while(! KEYPAD_read());
			// LOCK the system
			if(KEYPAD_read()=='*'){
				while(KEYPAD_read());
				LCD_write_command(0x01);//clear display
				LCD_write_command(0xC2);
				LCD_write_string("closed");
				_delay_ms(500);
				LCD_write_command(0x01);
			}
		}
	}
}



