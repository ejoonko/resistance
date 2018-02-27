#include "gamedata.h"

/*Overall functions*/

int rand(void) {
  return (myclock(120));
}

int pow(int x, int y) {
  if (y == 0) {
    return 1;
  }
  return x * pow(x, y - 1);
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

int myclock(int x) {
  mytime = 0x0000;
  int n;
  for (n = 0; n < x; n++){
    tick(&mytime);
    time2string(string, abs(mytime - x));
    display_string(0, string);
    display_update();
  }
  return 0;

}

/*Player select phase*/
void player_select_string_builder(int n) {
  char tempplayer_select_string [16];
  " SELECT";
  player_select_string = tempplayer_select_string;
}

void display_p_string_builder(int n) {
  display_p_string [0 + (3 * n)] = ((char) (n + 48));
  display_p_string [1 + (3 * n)] = ',';
  display_p_string [2 + (3 * n)] = ' ';
}

int nofplayerselect() { //Select n of players
  int btn;
  while(1) {
    if(getbtns() == 1) { //confirm button(BTN2)
      if (getsw() > 4 && getsw() < 9) {
        return getsw();
      }
    }
  }
}

/*Player reveal phase*/

void revealstring_builder(int n) {
  char temprevealstring [8] = "PLAYER ";
  temprevealstring[7] = ((char) (n + 1 + 48));
  revealstring = temprevealstring;
}

void hackerreveal_builder() {
  char temphackerreveal [nofhacker * 3 + 1];
  int n;
  for (n = 0; n < nofplayer; n++) {
    if (playerarray[n] == 1) {
      temphackerreveal[0 + (n * 3)] = ((char) (n + 1 + 48));
      temphackerreveal[1 + (n * 3)] = ',';
      temphackerreveal[2 + (n * 3)] = ' ';
    }
  }
  hackerreveal = temphackerreveal;
}

void player_reveal(int a_or_h) {//reveals player_n's role
revealstring_builder(a_or_h);
while(1) {
  display_string(1, revealstring);
  display_update();
  while(getbtns() == 1) { //confirm button(BTN2)
    if (playerarray[a_or_h] == 1) {
      display_string(2, hackerreveal);
      display_string(3, arehackers);
      display_update();
    } else {
      display_string(2, agentreveal);
      display_update();
    }
  }
}
}

/*Talking phase*/

/*Selection phase*/
int playerselection() { //Select n of players
  if(getbtns() == 1) { //confirm button(BTN2)
    if (getsw() > 0 && getsw() < (nofplayer + 1)) {
      return getsw();
    }
  }
  return -1;
}


/*voting phase*/
int vote_selection() {
  if (getbtns() == 2) {
    return 1;
  }
  if (getbtns() == 1) {
    return 0;
  }
  if (myclock(15) == 0) {
    return 1;
  }
}

void votestring_builder() {
  char tempvotestring [32] = "Vote for ";

  int n;
  for (n = 0; n < (sizeof(missionarray)/sizeof(missionarray[0])); n++) {
    tempvotestring[9 + (3 * n)] = ((char) (missionarray[n] + 48));
    tempvotestring[10 + (3 * n)] = ',';
    tempvotestring[11 + (3 * n)] = ' ';
  }
  tempvotestring[12 + (3 * n)] = ' ';
  tempvotestring[13 + (3 * n)] = 'b';
  tempvotestring[14 + (3 * n)] = 'y';
  tempvotestring[15 + (3 * n)] = ' ';
  tempvotestring[16 + (3 * n)] = ((char) (playerposition - 1 + 48));

  votestring = tempvotestring;
}

/*mission phase*/
int mission_agent() {
  display_string(2, securehack);
  display_update();
  while (1) {
    if (getbtns() == 2 | myclock(10) == 0) {
      return 0;
    }
  }
}

int mission_hacker() {
  display_string(2, securehack);
  display_update();

  while (1) {
    if (getbtns() == 2) {
      return 0;
    } else if (getbtns() == 1 | myclock(10) == 0) {
      return 1;
    }
  }
}

/*victory/defeat title phase*/
void victory_defeat() {
  while (myclock(20) != 0) {
    if (hacker_score == 3) {
      display_string(1, hacker_victory);
      display_update();
    } else {
      display_string(1, agent_victory);
      display_update();
    }
  }
}
