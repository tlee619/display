//                       -----------------
//                      |VCC          GND |-
//               RED LED|P1.0         XIN |
//                  KNOB|P1.1         XOUT|-
//                   PWM|P1.2/A1      TEST|
//	             VOLTAGE|P1.3         RST |
// 	             CURRENT|P1.4         P1.7|
//   	         	    |P1.5         P1.6|GREEN LED
//  DB4 - DISPLAY PIN 11|P2.0         P2.5|DISPLAY PIN 4  - RS
// 	DB5 - DISPLAY PIN 12|P2.1         P2.4|DISPLAY PIN 6  - E
// 	DB6 - DISPLAY PIN 13|P2.2         P2.3|DISPLAY PIN 14 - DB7
// 			            |             GND |DISPLAY PIN 5  - R/W
// 			            |                 |DISPLAY GND
//      	            |                 |DISPLAY VCC
//			             -----------------
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <msp430.h>
#include "msp430g2553.h"
#include "main.h"
#include "kent_lcd_display.h"
#include <stdio.h>
#include <stdlib.h> //added for integer to string function 11/06


unsigned int current_count;
unsigned int voltage_count;
unsigned int voltage_value;
unsigned int knob_count_low_pass_filter;
unsigned int knob_count;
int count;
int flags=0;
char topstring[LCD_SCREEN_WIDTH+1];
char scdstring[LCD_SCREEN_WIDTH+1];
char voltage[5];



int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
  BCSCTL1 = XT2OFF | CALBC1_1MHZ;//turn off XT2, SET ACLK to divide by 8, set MCLK (CPU) to 1, 8, 12, or 16 MHz
  DCOCTL = CALDCO_1MHZ;
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = A2DKNOBCHANNEL;                       // input A1
  ADC10AE0 |= BIT1 | BIT3 | BIT4;                         // PA.1 ADC option select
  P1DIR |= GREEN_LED;                            // Set P1.0 to output direction RED LED

  P1DIR |= BIT2 | RED_LED; // P1.2 to output
  P1SEL |= BIT2; // P1.2 to TA0.1

  //setup timer A0, which runs the pulse width modulator and data sampling
  //counter counts from 0 to TACCR0 and then starts over
  TA0CCR0 = 32768-1; // PWM Period
  TA0CCTL1 = OUTMOD_7; // CCR1 reset/set
  TA0CTL = TASSEL_2 + MC_1 + ID_0; // SMCLK divide by 1, up mode
  TA0CCTL0 = CCIS_2 | CCIE; // compare mode CCIS_2 sets GND as input => less current?; enable interrupt

  //setup timer A1, which runs the display
  TA1CCR0 = 0x800; // PWM Period
  TA1CCTL0 = OUTMOD_3; //
  TA1CTL = TASSEL_2 + MC_2 + ID_3; // SMCLK divide by 8, counts up continues mode
  TA1CCTL0 =  CCIE; // compare mode CCIS_2 sets GND as input => less current?; enable interrupt

  /*
  //int power[20];
 unsigned int power[5];
  char * temp;

  int i ;


 for( i = 0; i <= 4; i++ ){

	  	  power[i] = i;

	  	  sprintf(temp, "%d",  power[i] );
	  	  LCD_Intialize();
  		  LCD_WriteLine(temp, LCD_FIRST_LINE);// \0 inserts 0 value (not 0 character)  into string=c convention for end of line

	  	  if( i == 1){
	  		  LCD_WriteLine(temp, LCD_FIRST_LINE);// \0 inserts 0 value (not 0 character)  into string=c convention for end of line
	  	  }
	  	  else if( i == 2 ){
		  	  LCD_WriteLine( temp, LCD_SECOND_LINE);
	  	  }
	  	  else if( i == 3 ){
	  		  LCD_WriteLine(temp, LCD_THIRD_LINE);
	  	  }
	  	  else{
	  		  LCD_WriteLine(temp, LCD_FOURTH_LINE);
	  	  }
 }
*/
	  LCD_Intialize();
	  sprintf(topstring,"TIME         VOLTAGE\0");
	  LCD_WriteLine(topstring, LCD_FIRST_LINE);
  for (;;)
  {

    __bis_SR_register(CPUOFF + GIE);// Turn off CPU pending interrupt=LPM0 this line is only necessary to save power
	  if(tst_flag(TIMER1_FLAG)){ //4 high is some event ex. 0x4 -> 0x00100
		  clr_flag(TIMER1_FLAG);
		  voltage_count = voltage_count>>3;
		  voltage_value = voltage_count<<4;
		  voltage_value += voltage_count<<3;
		  voltage_value += voltage_count<<2;
		  sprintf(voltage,"%d", voltage_value);
		  sprintf(scdstring,"                %c.%cV\0", voltage[0],voltage[1]);
		  LCD_WriteLine(scdstring, LCD_SECOND_LINE);
	  }
	  if(tst_flag(A2D_FLAG)){
		  clr_flag(A2D_FLAG);
		  knob_count_low_pass_filter+= (knob_count - (knob_count_low_pass_filter>>5));
		  CCR1 = knob_count_low_pass_filter;
		 // sprintf(topstring,"Testing = %d\0", CCR1);

		  //LCD_WriteLine(topstring, LCD_SECOND_LINE);
	  }
  }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	switch (ADC10CTL1 & INCH_15)
	{
	case A2DKNOBCHANNEL:
		ADC10CTL0 &= ~ENC;// CLEAR ADC10CTL0 W/ COMPLEMENT
		ADC10CTL1 = A2DVOLTAGECHANNEL;
		knob_count = ADC10MEM;
		ADC10CTL0 |= ENC + ADC10SC;
	break;
	case A2DVOLTAGECHANNEL:
		ADC10CTL0 &= ~ENC;
		ADC10CTL1 = A2DCURRENTCHANNEL;
		voltage_count = ADC10MEM;
		ADC10CTL0 |= ENC + ADC10SC;
	break;
	default:
		ADC10CTL0 &= ~ENC;
		ADC10CTL1 = A2DKNOBCHANNEL;
		current_count = ADC10MEM;
		set_flag(A2D_FLAG);
	  __bic_SR_register_on_exit(CPUOFF);// Clear CPUOFF bit from 0(SR) IE cause CPU to stay on after interrupt
	}
}

#pragma vector =  TIMER0_A0_VECTOR       // timer0 is main 1 s timer
__interrupt void TIMERA0_ISR(void)
{
//  ENABLE_ADC;         // give Vref time to settle down need 30 uS (clock cycles) before conversion
	P1OUT ^= GREEN_LED;                       // Clear P1.0 LED off
	ADC10CTL0 |= ENC + ADC10SC;	// Sampling and conversion start
	set_flag(TIMER0_FLAG);
}

// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TimerA1_ISR (void)
{
	P1OUT ^= RED_LED;
	set_flag(TIMER1_FLAG);
	TA1CCTL1&=~CCIFG;//clear interrupt (present) flag
	TA1CCR0+=0x7000;
	__bic_SR_register_on_exit(CPUOFF);// Clear CPUOFF bit from 0(SR) IE cause CPU to stay on after interrupt
}

