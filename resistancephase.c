#include <math.h>
#include "gamedata.h"
#include "mipslab.h"

int player_select_phase() { //phase 0
  nofplayer = nofplayerselect();
  return 1;
}

int game_setup() { //phase 1
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
    if (playerarray[n % 5] == 0) {
      playerarray[n % 5] = rand() % 2;
      if (playerarray[n]) {
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
  for (int n = 0; n < nofplayer; n++) {
    player_reveal(n);
  }

  return 3;
}

int talking_phase() { //phase 3
  while(ticktock != 0) {
    display_image();
  }

  return 4;
}

int selection_phase() { //phase 4
  if (node_rejected > 4) {
    return 7;
  }

  int [nodes[nodeposition]] tempmissionarray;

  while (selectiondone != sizeof(tempmissionarray)) {
    while (selectiondone != sizeof(tempmissionarray) | ticktock() != 0) {
      display_image(playerposition);
      display_update();
      tempmissionarray[selectiondone] = playerselection();
      selectiondone++;
    }
    playerposition =  (playerposition + 1) % 5;
    if (ticktock == 0) {
      selectiondone = 0;
    }
    //reset clock
  }
  selectiondone = 0;
  missionarray = tempmissionarray;

  return 5;
}

int voting_phase() { //phase 5
  for (int n = 0; n < nofplayer; n++) {
    display_image();
    vote_selection();
    yesvotes += vote_selection();
  }
  if (yesvotes > (nofplayer / 2)) {
    node_rejected = 0;
    *porte = porte & 0xF0;
    return 6; //go to mission phase
  } else {
    node_rejected++;
    *porte += pow(2, node_rejected - 1);
    return 4; //go to selection phase
  }

  //return 6;
}

int mission_phase() { //phase 6
  for (int n = 0; n < getnode(nodeposition)) {
    if (playerarray[missionarray[n] - 1] == 0) {
      hacked += mission_agent();
    } else {
      hacked += mission_hacker();
    }
  }

  int required_hack = 1;
  if ((nofplayer > 6) && (nodeposition == 3)) {
    required_hack = 2;
  }

  if (hacked >= required_hack) {
    //display hacked
    //display how many people hacked node
    hacker_score++;
  } else {
    //display secured
    //display how many people hacked node
    agent_score++;
    *porte += pow(2, 7 - nodeposition);
  }
  hacked = 0;

  if (agent_score == 3) {
    return 7;
  } else if (hacker_score == 3){
    return 7;
  } else {
    return 3;
    nodeposition++;
  }
}

int end() { //phase 7
  victory_defeat();
  return 8;
}
