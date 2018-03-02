#include <pic32mx.h>
#include "gamedata.h"
#define TMR2PERIOD ((80000000 / 256) / 10)

void spi_init() {
  SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
  SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
  while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
  OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
  while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
  SYSKEY = 0x0;  /* Lock OSCCON */

  /* Set up output pins */
  AD1PCFG = 0xFFFF;
  ODCE = 0x0;
  TRISECLR = 0xFF;
  PORTE = 0x0;

  /* Output pins for display signals */
  PORTF = 0xFFFF;
  PORTG = (1 << 9);
  ODCF = 0x0;
  ODCG = 0x0;
  TRISFCLR = 0x70;
  TRISGCLR = 0x200;

  /* Set up input pins */
  TRISDSET = (1 << 8);
  TRISFSET = (1 << 1);

  /* Set up SPI as master */
  SPI2CON = 0;
  SPI2BRG = 4;
  /* SPI2STAT bit SPIROV = 0; */
  SPI2STATCLR = 0x40;
  /* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
  /* SPI2CON bit MSTEN = 1; */
  SPI2CONSET = 0x20;
  /* SPI2CON bit ON = 1; */
  SPI2CONSET = 0x8000;
}


void display_init(void) {
  #define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
  #define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

  #define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
  #define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

  #define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
  #define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

  #define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
  #define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void labinit( void ) {
  //volatile int* trise = (volatile int*) 0xbf886100;
  //volatile int* portd = (volatile int*) 0xbf8860D0;
  TRISE &= 0;
  PORTD |= 0xFE0;
  PR2 = TMR2PERIOD;
  TMR2 = 0;
  IFSCLR(0) = 0x00000100;
  IECSET(0) = 0x00000100;
  T2CON = 0x8070;
  T3CON = 0x8000;

  spi_init();
  display_init();
  return;
}
