/* mipslabwork.c

This file written 2015 by F Lundevall
Updated 2017-04-21 by F Lundevall

This file should be changed by YOU! So you must
add comment(s) here with your name(s) and date(s):

This file modified 2017-04-31 by Ture Teknolog

For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#define TMR2PERIOD ((80000000 / 256) / 10)

int mytime = 0x5957;
int timeoutcount = 0;
int prime = 1234567;
//int SW2 = PIC32_IRQ_INT2;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */

/* Lab-specific initialization goes here */
void labinit( void ) {
  volatile int* trise = (volatile int*) 0xbf886100;
  volatile int* portd = (volatile int*) 0xbf8860D0;
  *trise &= 0;
  *portd |= 0xFE0;
  PR2 = TMR2PERIOD;
  TMR2 = 0;
  T2CON = 0x8070;
  IFSCLR(0) = 0x00000000;
  IECSET(0) = 0x00000900;
  IPCSET(2) = 0x0A00000D;
  //INTCON = 0x4;

  enable_interrupt();
  return;
}

void user_isr( void )
{
      //display_image(96, icon);
  if (((IFS(0) & 0x100) >> 8) == 1) {
          timeoutcount++;
          IFSCLR(0) = 0x00000100;
      if (timeoutcount == 10) {
        time2string( textstring, mytime );
        tick( &mytime );
        if (((IFS(0) & 0x800) >> 11) == 1) {
          tick( &mytime );
          tick( &mytime );
          tick( &mytime );
          IFSCLR(0) = 0x00000900;
        }
        display_string( 3, textstring );
        display_update();
        volatile int* porte = (volatile int*) 0xbf886110;
        *porte += 1;
        timeoutcount = 0;
      }
  }
  return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
  //delay( 1000 );

  if (getbtns() == 1) {
    mytime = (mytime & 0xFF0F) | (getsw() << 4);
  }

  if (getbtns() == 2) {
    mytime = (mytime & 0xF0FF) | (getsw() << 8);
  }

  if (getbtns() == 4) {
    mytime = (mytime & 0x0FFF) | (getsw() << 12);
  }
}
