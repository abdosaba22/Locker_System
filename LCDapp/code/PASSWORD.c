
/*
 * functions.c
 *
 * Created: 12/6/2021 12:53:18 PM
 *  Author: Abdalrahman_Amin
 */
#include "header.h"
#include "LCD.h"
#include "EEPROM.h"
#include "KEYPAD.h"
#include "PASSWORD.h"

//global variable for n.of entered digits 
// 4 to compare with correct pass | 10 to compare with reset 10 zero
uint8_t global_digit=0;

/**************************
* Function Name: save_password
* Purpose      : save pass in EEprom
* Parameters   : correct_pass array of entered pass
* Return value : void
***************************/
void save_password(uint8_t *correct_pass){
	for(uint8_t i=0;i<4;i++){
		EEPROM_write(100+i,correct_pass[i]);
	}
}

/**************************
* Function Name: reset_password
* Purpose      : make pass 0000
* Parameters   : correct_pass array of saved pass
* Return value : void
***************************/
void reset_password(uint8_t *correct_pass){
	for(uint8_t i=0;i<4;i++){
		correct_pass[i]='0';
	}
	save_password(&correct_pass[0]);
	LCD_write_command(0x01);
	LCD_write_command(0x82);
	LCD_write_string("password_reset");
	_delay_ms(2000);
	LCD_write_command(0x01);
	
}

/**************************
* Function Name: compare_password
* Purpose      : detect if pass is correct or equal to Reset value
* Parameters   : correct_pass array of entered pass
*			   : check array of entered numbers from user
* Return value : uint8_t
***************************/
uint8_t compare_password(uint8_t *correct_pass , uint8_t *check){
	switch(global_digit){
		// entered 4 digits start compare with correct_pass
		case 4 :
			for(uint8_t i=0;i<4;i++){
				if(check[i]!=correct_pass[i]){wrong_password();return 0;}
			}
			correct_password();
			return 1;
			break;
		// entered 10 digits start compare with 10 zeros to reset pass	
		case 10 :
			for (uint8_t i=0;i<10;i++)
			{
				if (check[i]!='0'){wrong_password();return 0;}
			}
			reset_password(&correct_pass[0]);
			return 2;
			break;
		//  different n_of digit, wrong 	
		default:
			wrong_password();
			return 0;
			break;
	}						
}

/**************************
* Function Name: change_password
* Purpose      : change pass in one condition in EEprom
* Parameters   : correct_pass array of password
*              : check array of entered pass
* Return value : void
***************************/
void change_password(uint8_t *coreect_pass,uint8_t *check){
	LCD_write_command(0x01);
	LCD_write_command(0x82);
	LCD_write_string("set_password");
	LCD_write_command(0xC2);

	for(uint8_t i=0;i<4;i++){
		while(! KEYPAD_read());
		check[i]=KEYPAD_read();
		LCD_write_char(KEYPAD_read());
		while(KEYPAD_read());
	}
	while(! KEYPAD_read());
	
	if (KEYPAD_read()=='/')
	{   while(KEYPAD_read());
		LCD_write_command(0x01);
		for(uint8_t i=0;i<4;i++){
			coreect_pass[i]=check[i];
		}
		save_password(&coreect_pass[0]);
		LCD_write_string("password_saved");
		_delay_ms(2000);
		LCD_write_command(0x01);
	}
	else{
		LCD_write_command(0x01);
		LCD_write_string("pass_not_saved");
		_delay_ms(2000);
		while(KEYPAD_read());
		LCD_write_command(0x01);
	}
	
}
/**************************
* Function Name: read_entered_pass
* Purpose      : put the entered number from user in enterd_password array
* Parameters   : entered_pa array of number from user
* Return value : void
***************************/
void read_entered_pass(uint8_t *entered_pa){
	global_digit=0;//set global to initial value
	LCD_write_command(0xc0); 
	for (uint8_t i=0;i<10;i++)
	{   while(!KEYPAD_read());
		//entered 4 numbers then = go to compare		   | n.of entered digits 
		if(i==4 && KEYPAD_read()== '='){while(KEYPAD_read());global_digit=4;return;}
		// entered pass but not 4 digits to compare with correct_pass or 10 digit zero to reset.
		if(KEYPAD_read()== '='){while(KEYPAD_read()); global_digit=i;break;}
		entered_pa[i]=KEYPAD_read();
		SETBIT(PORTD,0);//buzzer
		LCD_write_char('*');
		LCD_write_command(0x06);
		_delay_ms(100);
		CLRBIT(PORTD,0);//buzzer
		while(KEYPAD_read());
		
	}
	global_digit=10; // 10 digits entered
		
} 
/**************************
* Function Name: wrong_password
* Purpose      : print on LCD try again used when entered pass wrong or not to reset 
* Parameters   : void
* Return value : void
***************************/
void wrong_password(void){
	
	LCD_write_command(0x01);
	LCD_write_command(0xc2);
	LCD_write_string("try_again");
	SETBIT(PORTD,0);
	_delay_ms(3000);
	CLRBIT(PORTD,0);
	LCD_write_command(0x01);
}
/**************************
* Function Name: correct_password
* Purpose      : print on LCD Locker opened used when entered pass correct 
* Parameters   : void
* Return value : void
***************************/
void correct_password(void){
	LCD_write_command(0x01);
	LCD_write_command(0xc2);
	LCD_write_string("Locker_opened");
	_delay_ms(2000);
	LCD_write_command(0x01);
}