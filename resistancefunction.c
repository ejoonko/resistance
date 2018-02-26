#include "gamedata.h"

/*Overall functions*/
void information_display() {//Displays information
  display_image();
  display_update();
}

/*Player select phase*/
int nofplayerselect() { //Select n of players
  int btn;
  while(1) {
    if(getbtns() == 1) { //confirm button(BTN2)
      if (getsw() > 4 && getsw() < 9) {
        return getsw();
      }
    }
  }
  return 0;
}

int getnode(int n) {
  /*setting up node numbers*/
  return nodes[(nofplayer - 5) * 5];
}

/*Player reveal phase*/
void player_reveal(int a_or_h) {//reveals player_n's role
while(1) {
    while(getbtns() == 1) { //confirm button(BTN2)
      if (playerarray[a_or_h] == 1) {
        display_image(hacker);
        display_update();
      } else {
        display_image(agent);
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
  if (ticktock() == 0) {
    return 1;
  }
}

/*mission phase*/
int mission_agent() {
  if (getbtns() == 2) {
    return 0;
  }

  if (ticktock() == 0) {
    return 0;
  }
}

int mission_hacker() {
  if (getbtns() == 2) {
    return 0;
  }
  if (getbtns() == 1) {
    return 1;
  }
  if (ticktock() == 0) {
    return 1;
  }
}

/*victory/defeat title phase*/
void victory_defeat() {
  display_image();
  display_update();
}
