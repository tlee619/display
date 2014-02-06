/*
 * kent_lcd_display.c
 *
 *  Created on: Sep 1, 2011
 *      Author: Kent
 */

//////////////////////////////////////////////////////////////////////////////
// Filename:          H:\Kent-ML402/drivers/PLB_lcd_char_v1_00_a/src/PLB_lcd_char_selftest.c
// Version:           1.00.a
// Description:       Contains a diagnostic self-test function for the PLB_lcd_char driver
// Date:              Thu Jun 28 13:56:29 2007 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "kent_lcd_display.h"
#include "msp430g2553.h"

/************************** Constant Definitions ***************************/


/************************** Variable Definitions ****************************/


/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PLB_LCD_CHAR instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */

void SendNibble(void){
	volatile unsigned int wait;
    P2OUT |=LCD_E; // sets E pin high
 	for(wait=0;wait<2;wait++);
    P2OUT &= ~LCD_E; // clears E pin , loads least significant data (falling edge of E latches data)
}

void LCD_Intialize(void){
	volatile int unsigned wait;
	P2DIR = 0XFF;// set port to output (not input)
	P2OUT = 3; // clr LCD_RS and LCD_E 0 and set LCD_E and nibble data to 3 to send 0x30
	for(wait=0;wait<60000;wait++);
	SendNibble();//first time
	for(wait=0;wait<15000;wait++);
	SendNibble();//second time
	for(wait=0;wait<15000;wait++);
	SendNibble();//third time
	for(wait=0;wait<15000;wait++);
	P2OUT = LCD_E + 2; // clr LCD_RS to 0 and set LCD_E to 1 and the nibble data to 2 to send 0x20
	SendNibble();
	// now do things normally
	LCD_CHAR_WriteCommand(0x0C);//set turnon display, curser and blink off
	LCD_CHAR_WriteCommand(0x28);//set datawidth=4, lines=2,font=5x7
	LCD_CHAR_WriteCommand(0x01);//clear display set address counter = 0
//	LCD_CHAR_WriteCommand(0x06);//7 incre direction,automatic
	LCD_CHAR_WriteCommand(0x0C);//turn on
}

void LCD_CHAR_WriteCommand(char command)
{
	P2OUT &= ~LCD_RS; // set to command mode, LCD_RS=0
	LCD_CHAR_WriteChar(command);
    P2OUT |= LCD_RS;// setting to character mode, LCD_RS=1
}

void LCD_CHAR_WriteChar(char data)
{
	volatile int unsigned wait;
	P2OUT = (P2OUT&0xF0)|((data >>4) &0XF);  // not changing high bits , setting low 4-bits of port to high 4 bits of data
	SendNibble();
	P2OUT = (P2OUT&0xF0)|((data)&0XF);//not changing port 2, setting low 4-bits of port with the low 4-bits of command
	SendNibble();
	for(wait=0;wait<100;wait++);
}

void LCD_WriteLine(char * string, char Line)
{
	char  i1,i2;
	LCD_CHAR_WriteCommand(Line);//dataram

	for(i1=0;((i1<LCD_SCREEN_WIDTH) && (string[i1]!=0));i1++)
		LCD_CHAR_WriteChar(string[i1]);

	if(i1<LCD_SCREEN_WIDTH)
		for(i2=i1;i2<LCD_SCREEN_WIDTH;i2++)
			LCD_CHAR_WriteChar(' ');
}
