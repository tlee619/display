/*
 * main.h
 *
 *  Created on: Oct 7, 2013
 *      Author: kent
 */

#ifndef MAIN_H_
#define MAIN_H_

#define GREEN_LED BIT6 // on port 1
#define RED_LED BIT0//on port 1

#define A2DCURRENTCHANNEL INCH_4 // Set to P1.4
#define A2DVOLTAGECHANNEL INCH_3 // Set to P1.3
#define A2DKNOBCHANNEL INCH_1 // Set to P1.1

#define set_flag(x) flags|=x
#define clr_flag(x) flags&=~x
#define tst_flag(x) (flags & x)

#define A2D_FLAG 0x1
#define TIMER0_FLAG 0x2
#define TIMER1_FLAG 0x4


#endif /* MAIN_H_ */
