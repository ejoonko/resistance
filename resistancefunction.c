#include "gamedata.h"
#include <pic32mx.h>

/*Overall functions*/
int p_constant = 0;
int a_constant = 0;
int r_constant = 0;

int rand(void) {  //Pesudo-random function
  return (int) TMR3;
}

int pow2(int x, int y) {  //Math-power function, not using library pow
  int n;
  if (y == 0) {
    return 1;
  }
  for (n = 0; n < y - 1; n++) {
    x = x * 2;
  }
  return x;
}

int abs(int x) {  //Calculate absolute value
  if (x < 0) {
    return x * -1;
  }
  return x;
}

int getsw( void ) { /*Swtich value*/
  return ((*portd & 0xF00) >> 8);
}

int getbtns(void) { /*Button value*/
  return ((*portd & 0xE0) >> 5);
}
/*Copy function for node history shown in information_display*/
void copier (char x[], char y[]) {
  int n;
  for (n = 0; n < 15; n++) {
    y[n] = x[n];
  }
}

//information display node builder
void idnode_builder() {
  switch (getsw()) {
    case 8://if switch is 8
    idnode[5] = (char) (1 + 48);
    break;

    case 4://if switch is 4
    idnode[5] = (char) (2 + 48);
    break;

    case 2://if switch is 2
    idnode[5] = (char) (3 + 48);
    break;

    case 1://if switch is 1
    idnode[5] = (char) (4 + 48);
    break;
  }


}
/*idnode_builder for showing node secured or hacked*/
void idnode_builder2() {

  idnode[5] = (char) (nodeposition + 1 + 48); /*Shows which node*/

  if (hacked >= required_hack) { //Shows hacked or secure based on result
    idnode[9] = 'H';
    idnode[10] = 'A';
    idnode[11] = 'C';
    idnode[12] = 'K';
    idnode[13] = 'E';
    idnode[14] = 'D';
  } else {
    idnode[9] = 'S';
    idnode[10] = 'E';
    idnode[11] = 'C';
    idnode[12] = 'U';
    idnode[13] = 'R';
    idnode[14] = 'E';
  }
}

/*"tab" function, to see history of past passed nodes*/
void information_display() {
  int x = 0;
  while (1) { /*While fourth button is pressed show history for node, indicated by switch value*/
    while (getbtns() == 4) { /*Fourth button, the left one, to show information*/
      switch(getsw()) { /*Display history for node depending on case*/
        case 8: /*History of node 1*/
        idnode_builder();
        display_string(0, idnode1);
        display_string(1, votestring1);
        display_string(2, vote_accepted_string1);
        display_string(3, vote_rejected_string1);
        display_update();
        x = 1;
        break;

        case 4: /*History of node 2*/
        idnode_builder();
        display_string(0, idnode2);
        display_string(1, votestring2);
        display_string(2, vote_accepted_string2);
        display_string(3, vote_rejected_string2);
        display_update();
        x = 1;
        break;

        case 2: /*History of node 3*/
        idnode_builder();
        display_string(0, idnode3);
        display_string(1, votestring3);
        display_string(2, vote_accepted_string3);
        display_string(3, vote_rejected_string3);
        display_update();
        x = 1;
        break;

        case 1: /*History of node 4*/
        idnode_builder();
        display_string(0, idnode4);
        display_string(1, votestring4);
        display_string(2, vote_accepted_string4);
        display_string(3, vote_rejected_string4);
        display_update();
        x = 1;
        break;
      }
    }
    if (x > 0) {
      return;
    }
  }
}

/*Decimal to Hexa conversion*/
int deci2hexa (int x) {
  int tempmytime = 0x0000; //temp time initialized as 0
  while (x >= 60) { //checks if input is greater than a minute
    tempmytime += 0x0100;
    x -= 60;
  }
  while (x >= 10) { //checks if input after previous while loop is greater than 10 seconds
    tempmytime += 0x0010;
    x-= 10;
  }
  tempmytime += x; //Adds remaining seconds to the temp timer

  return tempmytime; //returns value to mytime
}

int myclock(int x) {
  mytime = deci2hexa(x);
  int count = 0;
  int n = 0;
  while (n < x + 1) {
    if (((IFS(0) & 0x100) >> 8) == 1) { //Using timer from chipkit
      IFSCLR(0) = 0x00000100; //Reset flag
      count++;
      if (count == 10) { // aritificial timer for 1 second
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
//Invisible clock
int invmyclock(int x) {
  mytime = deci2hexa(x);
  int count = 0;
  int n = 0;
  while (n < x + 1) {
    if (((IFS(0) & 0x100) >> 8) == 1) {
      IFSCLR(0) = 0x00000100;
      count++;
      if (count == 10) {
        tick( &mytime );
        display_update();
        count = 0;
        n++;
      }
    }
  }
  return 0;
}

void reset_display() { // Reseting all display to empty
  display_string( 0, "" );
  display_string( 1, "" );
  display_string( 2, "" );
  display_string( 3, "" );
  display_update();
}

//title clock
int titlemyclock(int x) {
  mytime = deci2hexa(x);
  int count = 0;
  int n = 0;
  while (n < x + 1) {
    reset_display();
    display_string(1, title);
    display_update();
    if (((IFS(0) & 0x100) >> 8) == 1) {
      IFSCLR(0) = 0x00000100;
      count++;
      if (count == 10) {
        tick( &mytime );
        count = 0;
        n++;
      }
    }
  }
  return 0;
}

/*Player select phase functions*/
void player_select_string_builder(int n) {
  player_select_string[7] = ((char) (n + 1 + 48));
}

void amount2select_builder(int n) {
  amount2select[7] = ((char) (n + 48));
}
/*Selects players for node to secure or hack*/
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
/*Selects amount of players depending on swtich value*/
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

/*Player reveal phase function*/

void revealstring_builder(int n) {
  revealstring[11] = ((char) (n + 1 + 48));
}

void hackerreveal_builder() { //Builds a string for all hackers
  int n;
  int x = 0;
  for (n = 0; n < nofplayer; n++) {
    if (playerarray[n] == 1) {//sees if the player is a hacker
      hackerreveal[4 + (x * 3)] = ((char) (n + 1 + 48));
      hackerreveal[5 + (x * 3)] = ',';
      hackerreveal[6 + (x * 3)] = ' ';
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
    if (playerarray[n] == 1) { //displays the following if player is hacker
      display_string(2, hackerreveal);
      display_string(3, arehackers);
      display_update();
    } else { //otherwise display agent
      display_string(2, agentreveal);
      display_update();
    }
  }
  if (x > 0) {//return when player is finished pressing
    return;
  }
}
}

/*Talking phase function*/
//empty lol

/*Selection phase functions*/
int playerselection() { //Select n of players
  int x = 0;
  int y = 0;
  int n;
  while(1) { /*information_display-function*/
    while(getbtns() == 4) { /*BTN4 is pressed show information_display*/
      information_display();
      y = 1;
    }
    /*Display information once BTN4 is released*/
    if (y == 1) {
      reset_display();
      display_string(0, player_select_string);
      if (nofplayer > 6 & nodeposition == 3) {  /*Case 1*/
        display_string(1, two_hacks_required);
      } else if (nodeposition == 4){            /*Case 2*/
        (display_string(1, lastnode));
      } else {                                  /*Case 3*/
        display_string(1, useswitches);
      }
      display_string(2, amount2select);
      display_update();
      y = 0;
    }

    while(getbtns() == 1) { //confirm button(BTN2)
      if (getsw() > 0 && getsw() <= nofplayer) { //Get the value from switches which equals a player
        x = getsw();
      }
    }
    if (x > 0) {
      return x;
    }
  }
}


/*voting phase*/
int vote_selection() {//determines the player's vote
int x = 2;
int y = 0;
while (1) {
  while(getbtns() == 4) { /*BTN4 is pressed show id*/
    information_display();
    y = 1;
  }

  if (y == 1) { /*While BTN4 is pressed, show history*/
    reset_display();
    display_string(2, revealstring);
    display_string(0, votestring);
    display_string(3, acceptrefuse);
    y = 0;
  }

  display_update();
  while (getbtns() == 1) { //return 0 if player rejects
    x = 0;
  }

  while (getbtns() == 2 ) { //return 1 if player accepts
    x = 1;
  }

  if (x < 2) {
    return x;
  }
}
}

void votestring_builder() {
  int n;
  for (n = 0; n < getnode(nodeposition); n++) {//builds string for the selected and who selected it
    votestring [0 + (n * 2)] = ((char) (missionarray[n] + 48));
    votestring [1 + (n * 2)] = ',';
  }
  votestring[14] = (char) (playerposition + 1 + 48);
}

void vote_accepted_string_builder(int n) {//builds string for the accepts
  vote_accepted_string [0 + a_constant] = ((char) (n + 1 + 48));
  vote_accepted_string [1 + a_constant] = ' ';
  vote_rejected_string [nofplayer - 1 - a_constant] = ' '; /*Accepted constant based on players accepted*/
  a_constant = (a_constant + 1);
}

void vote_rejected_string_builder(int n) {//builds string for the rejects
  vote_rejected_string [0 + r_constant] = ((char) (n + 1 + 48));
  vote_rejected_string [1 + r_constant] = ' ';
  vote_accepted_string [nofplayer - 1 - r_constant] = ' '; /*Rejected constant based on players rejected*/
  r_constant = (r_constant + 1);
}

void votestring_reset() {//reset votestring
  int n;
  for (n = 0; n < 5; n++) {
    votestring [0 + (n * 2)] = ' ';
    votestring [1 + (n * 2)] = ' ';
  }
}

/*mission phase function*/
int mission_agent(int n) {//mission for agent
  //agents cant hack at all
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
  //hackers can secure or hack
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

amountofhack_builder(int n) { //shows how many hacks were detected
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
