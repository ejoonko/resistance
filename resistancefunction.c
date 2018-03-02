#include "gamedata.h"
#include <pic32mx.h>

/*Overall functions*/
int p_constant = 0;

int rand(void) {
  return (int) TMR3;
}

int pow2(int x, int y) {
  int n;
  if (y == 0) {
    return 1;
  }
  for (n = 0; n < y - 1; n++) {
    x = x * 2;
  }
  return x;
}

int abs(int x) {
  if (x < 0) {
    return x * -1;
  }
  return x;
}

int getsw( void ) {
  return ((*portd & 0xF00) >> 8);
}

int getbtns(void) {
  return ((*portd & 0xE0) >> 5);
}

/*void information_display() {//Displays information
display_image();
display_update();
}*/

int deci2hexa (int x) {
  int tempmytime = 0x0000;
  while (x >= 60) {
    tempmytime += 0x0100;
    x -= 60;
  }
  while (x >= 10) {
    tempmytime += 0x0010;
    x-= 10;
  }
  tempmytime += x;

  return tempmytime;
}

int myclock(int x) {
  mytime = deci2hexa(x);
  int count = 0;
  int n = 0;
  while (n < x + 1) {
    if (((IFS(0) & 0x100) >> 8) == 1) {
      IFSCLR(0) = 0x00000100;
      count++;
      if (count == 5) {
        time2string( string, mytime);
        tick( &mytime );
        display_string( 0, string );
        display_update();
        count = 0;
        n++;
      }
    }
  }
  return 0;
}

int invmyclock(int x) {
  mytime = deci2hexa(x);
  int count = 0;
  int n = 0;
  while (n < x + 1) {
    if (((IFS(0) & 0x100) >> 8) == 1) {
      IFSCLR(0) = 0x00000100;
      count++;
      if (count == 5) {
        tick( &mytime );
        display_update();
        count = 0;
        n++;
      }
    }
  }
  return 0;
}

void reset_display() {
  display_string( 0, "" );
  display_string( 1, "" );
  display_string( 2, "" );
  display_string( 3, "" );
  display_update();
}

/*Player select phase*/
void player_select_string_builder(int n) {
  player_select_string[7] = ((char) (n + 1 + 48));
}

void amount2select_builder(int n) {
  amount2select[7] = ((char) (n + 48));
}

void display_p_string_builder(int n) {
  display_p_string [0 + (p_constant * 3)] = ((char) (n + 48));
  display_p_string [1 + (p_constant * 3)] = ',';
  display_p_string [2 + (p_constant * 3)] = ' ';
  p_constant = (p_constant + 1) % getnode(nodeposition);
}

void display_p_string_reset() {
  int n;
  for (n = 0; n < 5; n++) {
    display_p_string [0 + (n * 3)] = (char) 0;
    display_p_string [1 + (n * 3)] = (char) 0;
    display_p_string [2 + (n * 3)] = (char) 0;
  }
}

int nofplayerselect() { //Select n of players
  int x = 0;
  while(1) {
    while(getbtns() == 1) { //confirm button(BTN2)
      if (getsw() > 4 && getsw() < 9) {
        x = getsw();
      }
    }
    if (x > 0) {
      return x;
    }
  }
}

/*Player reveal phase*/

void revealstring_builder(int n) {
  revealstring[7] = ((char) (n + 1 + 48));
}

void hackerreveal_builder() {
  int n;
  int x = 0;
  for (n = 0; n < nofplayer; n++) {
    if (playerarray[n] == 1) {
      hackerreveal[0 + (x * 3)] = ((char) (n + 1 + 48));
      hackerreveal[1 + (x * 3)] = ',';
      hackerreveal[2 + (x * 3)] = ' ';
      x++;
    }
  }
}

void player_reveal(int n) {//reveals player_n's role
reset_display();
revealstring_builder(n);
hackerreveal_builder();
int x = 0;
while(1) {
  display_string(1, revealstring);
  display_update();
  while(getbtns() == 1) { //confirm button(BTN2)
    x++;
    if (playerarray[n] == 1) {
      display_string(2, hackerreveal);
      display_string(3, arehackers);
      display_update();
    } else {
      display_string(2, agentreveal);
      display_update();
    }
  }
  if (x > 0) {
    return;
  }
}
}

/*Talking phase*/

/*Selection phase*/
int playerselection() { //Select n of players
  int x = 0;
  while(1) {
    while(getbtns() == 1) { //confirm button(BTN2)
      if (getsw() > 0 && getsw() <= nofplayer) {
        x = getsw();
      }
    }
    if (x > 0) {
      return x;
    }
  }
}


/*voting phase*/
int vote_selection() {
  int x = 2;
  while (1) {
    display_update();
    while (getbtns() == 1) {
      x = 0;
    }

    while (getbtns() == 2 ) {
      x = 1;
    }
    /*if (myclock(15) == 0) {
    return 1;
  }*/
  if (x < 2) {
    return x;
  }
}
}

void votestring_builder() {
  int n;
  for (n = 0; n < getnode(nodeposition); n++) {
    votestring [0 + (n * 2)] = ((char) (missionarray[n] + 48));
    votestring [1 + (n * 2)] = ',';
    //  votestring [2 + (n * 3)] = ' ';
  }
  votestring[14] = (char) (playerposition + 1 + 48);
}

void votestring_reset() {
  int n;
  for (n = 0; n < 5; n++) {
    votestring [0 + (n * 2)] = ' ';
    votestring [1 + (n * 2)] = ' ';
  }
}

/*mission phase*/
int mission_agent(int n) {
  int x = 0;

  revealstring_builder(n - 1);
  reset_display();
  display_string(1, revealstring);
  display_string(2, securehack);
  display_update();

  while (1) {
    while (getbtns() == 2) {
      x = 1;
    }

    if (x > 0) {
      return 0;
    }
  }
}

int mission_hacker(int n) {
  int x = 0;

  revealstring_builder(n - 1);
  reset_display();
  display_string(1, revealstring);
  display_string(2, securehack);
  display_update();

  while (1) {
    while (getbtns() == 2) {
      x = 1;
    }

    while (getbtns() == 1) {
      x = 2;
    }

    if (x > 0) {
      return x - 1;
    }
  }
}

amountofhack_builder(int n) {
  amountofhack[0] = (char) (n + 48);
}

/*victory/defeat title phase*/
void victory_defeat() {
    if (hacker_score == 3 | node_rejected == 5) {
      reset_display();
      display_string(1, hacker_victory);
      display_string(3, hackerreveal);
      display_update();
    } else {
      reset_display();
      display_string(1, agent_victory);
      display_update();
    }
}
