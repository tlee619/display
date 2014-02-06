/*
 * kent_lcd_display.h
 *
 *  Created on: Sep 1, 2011
 *      Author: Kent
 ********************************************************************************************************************
 *			MAKE SURE DISPLAY IS NOT BUSY BEFORE ACCESSING.
 *			This can be done with PLB_LCD_CHAR_ReadBusy(BaseAddress)
 *			For the more common functions there are macros or funtions that check first
 *******************************************************************************************************************/



#ifndef KENT_LCD_DISPLAY_H_
#define KENT_LCD_DISPLAY_H_
//////////////////////////////////////////////////////////////////////////////
// Filename:          H:\Kent-ML402/drivers/PLB_lcd_char_v1_00_a/src/PLB_lcd_char.h
// Version:           1.00.a
// Description:       PLB_lcd_char Driver Header File
// Date:              Thu Jun 28 13:56:29 2007 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 */

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a PLB_LCD_CHAR register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PLB_LCD_CHAR device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PLB_LCD_CHAR_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
/**
 *
 * Read a value from a PLB_LCD_CHAR register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PLB_LCD_CHAR device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 PLB_LCD_CHAR_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */

#define LCD_SCREEN_WIDTH (20)
#define LCD_FIRST_LINE 0x80
#define LCD_SECOND_LINE 0xC0
#define LCD_THIRD_LINE (0x80 +LCD_SCREEN_WIDTH)
#define LCD_FOURTH_LINE (0xC0 + LCD_SCREEN_WIDTH)

//set port pins
#define LCD_RS BIT5
#define LCD_E BIT4


/************************** Function Prototypes ****************************/
void LCD_WriteLine(char * string, char Line);
void LCD_CHAR_WriteCommand(char command);
void LCD_CHAR_WriteChar(char data);
void LCD_Intialize(void );


#endif /* KENT_LCD_DISPLAY_H_ */

