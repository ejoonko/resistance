#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

volatile int* portd = (volatile int*) 0xbf8860D0;
int getsw( void ) {
  return ((*portd & 0xF00) >> 8);
}

int getbtns(void) {
  return ((*portd & 0xE0) >> 5);
}
