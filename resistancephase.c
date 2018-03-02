#include <stdlib.h>
#include <math.h>
#include "gamedata.h"

int nofplayer;
int playerarray[8];
int nofhacker;
int selectiondone = 0;
int nodeposition = 0;
int playerposition = 0;
int missionarray[5];
int yesvotes = 0;
int node_rejected = 0;
int hacked = 0;
int hacker_score = 0;
int agent_score = 0;

int nodes [] = {
  2, 3, 2, 3, 3,
  2, 3, 4, 3, 4,
  2, 3, 3, 4/*2*/, 4,
  3, 4, 4, 5/*2*/, 5
};

volatile int* porte = (volatile int*) 0xbf886110;
volatile int* portd = (volatile int*) 0xbf8860D0;

char string[] = "Howdy!";
int mytime = 0x0000;

char nofplayerstring1[] = " Player Amount?";
char nofplayerstring2[] = "   5, 6, 7, 8";

char revealstring[] = "PLAYER  ";
char agentreveal[] = "You are a agent!";
char hackerreveal[] = "          ";
char arehackers[] = " are hackers!";

char talking_phase_string[] = "Talking Phase";

char useswitches[] = "Use switches";
char player_select_string[] = "PLAYER   SELECT";
char display_p_string[] = "               ";
char amount2select[] = "Select   players";

char votestring[] = "           by  ";
char acceptrefuse[] = "ACCEPT REFUSE";
char voteaccepted[] = "NODE ACCEPTED";
char voterejected[] = "NODE REJECTED";

char securehack[] = "SECURE HACK";
char securedmessage[] = "SECURED!";
char hackedmessage[] = "HACKED!";
char amountofhack[] = "  hacker found";

char agent_victory[] = "AGENTS VICTORY!";
char hacker_victory[] = "HACKERS VICTORY!";

int player_select_phase() { //phase 0
  reset_display();
  display_string(1, nofplayerstring1);
  display_string(2, nofplayerstring2);
  display_update();
  nofplayer = nofplayerselect();
  return 1;
}

int game_setup() { //phase 1
  /*making array for players*/
  //int player [nofplayer];
  //playerarray = player;

  /*assigning agents and hackers*/
  if (nofplayer > 6) {
    nofhacker = 3;
  } else {
    nofhacker = 2;
  }
  int n;

  /*for (n = 0; n < nofhacker; n++) {
  playerarray[n] = 1;
}*/

while (nofhacker > 0) {
  if (playerarray[n % nofplayer] == 0) {
    playerarray[n % nofplayer] = rand() % 2;
    if (playerarray[n % nofplayer] == 1) {
      nofhacker--;
    }
  }
  n++;
}

return 2;
}

int getnode(int n) {
  /*setting up node numbers*/
  return nodes[((nofplayer - 5) * 5) + n];
}

int player_reveal_phase() { //phase 2
  int n;
  for (n = 0; n < nofplayer; n++) {
    player_reveal(n);
  }

  return 3;
}

int talking_phase() { //phase 3
  reset_display();
  display_string(2, talking_phase_string);
  while(myclock((10 * nodeposition) + 20) != 0) { //can be changed
    display_update();
  }

  return 4;
}

int selection_phase() { //phase 4
  if (node_rejected > 4) {
    return 7;
  }
  int x = 0;
  int n = 0;

  reset_display();

  player_select_string_builder(playerposition);
  amount2select_builder(getnode(nodeposition));

  while (selectiondone != getnode(nodeposition)/* | myclock(60) != 0*/) {
    display_string(0, player_select_string);
    display_string(1, useswitches);
    display_string(2, amount2select);
    display_update();
    x = playerselection();
    selectiondone++;
    display_p_string_builder(x); //works
    n++;
    display_string(3, display_p_string);
    display_update();
    missionarray[selectiondone - 1] = x;
  }
  votestring_reset();
  votestring_builder();
  playerposition =  (playerposition + 1) % nofplayer;
  /*if (myclock(60) == 0) {
  selectiondone = 0;
}*/
//mytime = deci2hexa(60);
selectiondone = 0;

return 5;
}

int voting_phase() { //phase 5
  int n;
  int x = 0;
  for (n = 0; n < nofplayer; n++) {
    reset_display();
    revealstring_builder(n);
    display_string(2, revealstring);
    display_string(0, votestring);
    display_string(3, acceptrefuse);
    x = vote_selection();
    display_update();
    yesvotes += x;
  }
  reset_display();
  if (yesvotes > (nofplayer / 2)) {
    display_string(2, voteaccepted);
    while (invmyclock(5) != 0) {
      display_update();
    }
    node_rejected = 0;
    *porte = *porte & 0xF0;
    display_p_string_reset();
    display_update();
    yesvotes = 0;
    return 6; //go to mission phase
  } else {
    display_string(2, voterejected);
    while (invmyclock(5) != 0) {
      display_update();
    }
    node_rejected++;
    *porte += pow2(2, node_rejected - 1);
    display_p_string_reset();
    display_update();
    yesvotes = 0;
    return 4; //go to selection phase
  }

  //return 6;
}

int mission_phase() { //phase 6
  int n;
  for (n = 0; n < getnode(nodeposition); n++) {
    if (playerarray[missionarray[n] - 1] == 0) {
      hacked += mission_agent(missionarray[n]);
    } else {
      hacked += mission_hacker(missionarray[n]);
    }
  }

  int required_hack = 1;
  if ((nofplayer > 6) && (nodeposition == 3)) {
    required_hack = 2;
  }

  if (hacked >= required_hack) {
    reset_display();
    amountofhack_builder(hacked);
    display_string(1, hackedmessage);
    display_string(2, amountofhack);
    while (invmyclock(5) != 0) {
      display_update();
    }
    hacker_score++;
  } else {
    reset_display();
    display_string(1, securedmessage);
    while (invmyclock(5) != 0) {
      display_update();
    }
    agent_score++;
    *porte += pow2(2, 7 - nodeposition);
  }
  hacked = 0;

  if (agent_score == 3) {
    return 7;
  } else if (hacker_score == 3){
    return 7;
  } else {
    nodeposition++;
    return 3;
  }
}

int end() { //phase 7
  victory_defeat();
  while (invmyclock(20) != 0) {
    display_update();
  }
  return 8;
}
