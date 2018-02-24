#include <math.h>
#include "gamedata.h"

void player_select_phase() {
  nofplayerselect();
  nofplayer = nofplayerselect();
}

int getnode(int n) {
  /*setting up node numbers*/
  return nodes[(nofplayer - 5) * 5];
}

void game_setup() {
  /*making array for players*/
  int [nofplayer] player;
  playerarray = player;

  /*assigning agents and hackers*/
  if (nofplayer > 6) {
    nofhacker = 3;
  } else {
    nofhacker = 2;
  }
  int n = 0;
  while (nofhacker > 0) {
    if (playerarray[n] == 0) {
      playerarray[n] = rand() % 2;
      if (playerarray[n]) {
        nofhacker--;
      }
    }
    n++;
  }
}

void player_reveal_phase() {
  for (int n = 0; n < nofplayer; n++) {
    player_reveal(n);
  }
}

void talking_phase() {
  while(ticktock != 0) {
    display_image();
  }
}

void selection_phase() {
  while(selectiondone != getnode(nodeposition)) {
    while((selectiondone != getnode(nodeposition)) & (ticktock != 0)) {
      display_image(PLAYER);
      for (int n = 0; n < getnode(nodeposition); n++) {
        playerselection();
        selectiondone++;
      }
    }
  }
}

void voting_phase() {
  for (int n = 0; n < nofplayer; n++) {
    display_image();
    vote_selection();
    yesvotes += vote_selection();
  }
  if (yesvotes > (nofplayer / 2)) {
    //go to mission phase
    node_rejected = 0;
  } else {
    //go to selection phase
    node_rejected++;
  }
}

void mission_phase() {
  for (int n = 0; n < getnode(nodeposition)) {
    if (playerarray[PLAYER] == 0) {
      hacked += mission_agent();
    } else {
      hacked += mission_hacker();
    }
  }
  if (hacked > 0) {
    //display hacked
    //display how many people hacked node
    hacker_score++;
  } else {
    //display secured
    agent_score++;
  }

  if (agent_score == 3) {
    //end game agent victory
  } else if (hacker_score == 3){
    //end game hacker victory
  } else {
    //go to talking_phase
    nodeposition++;
  }
}

void end() {
  victory_defeat();
}
