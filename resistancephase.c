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
int required_hack;

int nodes [] = {
  2, 3, 2, 3, 3,//5 player
  2, 3, 4, 3, 4,//6 player
  2, 3, 3, 4/*2*/, 4,//7 player
  3, 4, 4, 5/*2*/, 5//8 player
};

volatile int* porte = (volatile int*) 0xbf886110; /*LED's*/
volatile int* portd = (volatile int*) 0xbf8860D0; /*Buttons and switches*/

char title[] = "   RESISTANCE";//title string
char string[] = "Howdy!";//string for the clock
int mytime = 0x0000;//clock initialization

char idnode[] = "Node           ";//node string in tab

char idnode1[] = "Node 1       ";//individual node copying off idnode
char idnode2[] = "Node 2       ";//individual node copying off idnode
char idnode3[] = "Node 3       ";//individual node copying off idnode
char idnode4[] = "Node 4       ";//individual node copying off idnode

char nofplayerstring1[] = " Player Amount?";//String for asking player amount
char nofplayerstring2[] = "   5, 6, 7, 8";//String showing the amount

char revealstring[] = "    PLAYER  ";//String used for player reveal
char agentreveal[] = "You are a agent!";//String showing agent
char hackerreveal[] = "              ";//String showing all hackers
char arehackers[] = "  are hackers!";//String showing hacker

char talking_phase_string[] = " Talking Phase";

char useswitches[] = "Use switches";
char player_select_string[] = "PLAYER   SELECT";
char display_p_string[] = "               ";
char amount2select[] = "Select   players";
char two_hacks_required[] = "2 hacks required";
char lastnode[] = "   Last Node!";

char votestring[] = "           by  ";

char votestring1[] = "           by  "; //for history

char votestring2[] = "           by  "; //for history

char votestring3[] = "           by  "; //for history

char votestring4[] = "           by  "; //for history

char acceptrefuse[] = " ACCEPT REFUSE";
char voteaccepted[] = " NODE ACCEPTED";
char voterejected[] = " NODE REJECTED";

char vote_accepted_string[] = "         accept";
char vote_rejected_string[] = "         reject";

char vote_accepted_string1[] = "         accept"; //for history
char vote_rejected_string1[] = "         reject"; //for history

char vote_accepted_string2[] = "         accept"; //for history
char vote_rejected_string2[] = "         reject"; //for history

char vote_accepted_string3[] = "         accept"; //for history
char vote_rejected_string3[] = "         reject"; //for history

char vote_accepted_string4[] = "         accept"; //for history
char vote_rejected_string4[] = "         reject"; //for history

char securehack[] = "  SECURE HACK";
char securedmessage[] = "    SECURED!";
char hackedmessage[] = "    HACKED!";
char amountofhack[] = "  hack detected";

char agent_victory[] = "AGENTS VICTORY!";
char hacker_victory[] = "HACKERS VICTORY!";

int title_phase() {//Shows title
  titlemyclock(5);//Goes on for 5 seconds
}

int player_select_phase() { //phase 0. The phase for selecting players
  reset_display();
  display_string(1, nofplayerstring1);
  display_string(2, nofplayerstring2);
  display_update();
  nofplayer = nofplayerselect(); /*Select players from binary number from swtiches*/
  return 1;
}

int game_setup() { //phase 1
  /*making array for players*/
  //Based on the player's selection set the game up

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
/*Going through array of number of players, give random players the role of hacker*/
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
  //function for getting the value of a certain node
  return nodes[((nofplayer - 5) * 5) + n];
}

int player_reveal_phase() { //phase 2
  int n;
  for (n = 0; n < nofplayer; n++) {
    player_reveal(n); /*Reveals if player is hacker, and teammates, or agent*/
  }

  return 3;
}

int talking_phase() { //phase 3
  //Primary phase for players to do conversation
  reset_display();
  display_string(2, talking_phase_string);
  while(myclock((10 * nodeposition) + 20) != 0) { //can be changed
    display_update();
  }

  return 4;
}

int selection_phase() { //phase 4
  if (node_rejected > 4) {
    return 7;               /*Hackers win if rejected nodes > 4*/
  }
  int x = 0; //x is the selection done by the switches

  reset_display();

  player_select_string_builder(playerposition);
  amount2select_builder(getnode(nodeposition));     /*Amount of players to selct from predetermined nodeposition-values*/

  while (selectiondone != getnode(nodeposition)) { /*Once choosen player selected required amount of players then break out*/
    display_string(0, player_select_string);
    if (nofplayer > 6 & nodeposition == 3) {    /*Case 1*/
      display_string(1, two_hacks_required);
    } else if (nodeposition == 4){              /*Case 2*/
      (display_string(1, lastnode));
    } else {                                    /*Case 3*/
      display_string(1, useswitches);
    }
    display_string(2, amount2select);
    display_update();
    x = playerselection();
    selectiondone++;
    display_p_string_builder(x); //works
    display_string(3, display_p_string);
    display_update();
    missionarray[selectiondone - 1] = x; //Puts the selection in the missionarray
  }
  votestring_reset();
  votestring_builder();

  switch(nodeposition) {//Puts the votestring value into the copies for the tab
    case 0:
    copier(votestring, votestring1);
    break;

    case 1:
    copier(votestring, votestring2);
    break;

    case 2:
    copier(votestring, votestring3);
    break;

    case 3:
    copier(votestring, votestring4);
    break;
  }

  playerposition =  (playerposition + 1) % nofplayer; //Rotation for the selector
  selectiondone = 0; //Reset the selectiondone for next use

  return 5; //go to next phase
}

int voting_phase() { //phase 5
  //After selection, players will now vote on whether they agree or disagree
  int n;
  int x = 0;
  a_constant = 0;
  r_constant = 0;
  for (n = 0; n < nofplayer; n++) { //Rotate so that all players can vote
    reset_display();
    revealstring_builder(n);
    display_string(2, revealstring);
    display_string(0, votestring);
    display_string(3, acceptrefuse);
    x = vote_selection();
    if (x == 1) { //For each accept or refuse, build a string
      vote_accepted_string_builder(n);
    } else {
      vote_rejected_string_builder(n);
    }
    display_update();
    yesvotes += x; //Add the votes
  }
  reset_display();
  if (yesvotes > (nofplayer / 2)) { //If majority accepts, then the mission is good to go.
    display_string(1, voteaccepted);
    display_string(2, vote_accepted_string);
    display_string(3, vote_rejected_string);
    while (invmyclock(5) != 0) {
      display_update();
    }
    node_rejected = 0; // Reset node rejected
    *porte = *porte & 0xF0; // Reset LED representing node rejected
    display_p_string_reset();
    display_update();

    /*Node history for information-display function, for IF-scenario*/
    switch(nodeposition) {
      case 0:
      copier(vote_accepted_string, vote_accepted_string1);
      copier(vote_rejected_string, vote_rejected_string1);
      break;

      case 1:
      copier(vote_accepted_string, vote_accepted_string2);
      copier(vote_rejected_string, vote_rejected_string2);
      break;

      case 2:
      copier(vote_accepted_string, vote_accepted_string3);
      copier(vote_rejected_string, vote_rejected_string3);
      break;

      case 3:
      copier(vote_accepted_string, vote_accepted_string4);
      copier(vote_rejected_string, vote_rejected_string4);
      break;
    }

    yesvotes = 0;
    return 6; //go to mission phase

  } else { //Otherwise go back to selection with a new selector
    display_string(1, voterejected);
    display_string(2, vote_accepted_string);
    display_string(3, vote_rejected_string);
    while (invmyclock(5) != 0) {
      display_update();
    }
    node_rejected++; //When node rejected = 5, agents lose.
    *porte += pow2(2, node_rejected - 1); /*Lits LED for each rejected score, from the right*/
    display_p_string_reset();
    display_update();

    /*information_display for ELSE-scenario*/
    switch(nodeposition) {
      case 0:
      copier(vote_accepted_string, vote_accepted_string1);
      copier(vote_rejected_string, vote_rejected_string1);
      break;

      case 1:
      copier(vote_accepted_string, vote_accepted_string2);
      copier(vote_rejected_string, vote_rejected_string2);
      break;

      case 2:
      copier(vote_accepted_string, vote_accepted_string3);
      copier(vote_rejected_string, vote_rejected_string3);
      break;

      case 3:
      copier(vote_accepted_string, vote_accepted_string4);
      copier(vote_rejected_string, vote_rejected_string4);
      break;
    }

    yesvotes = 0;
    return 4; //go to selection phase
  }

  //return 6;
}

int mission_phase() { //phase 6
  int n;
  for (n = 0; n < getnode(nodeposition); n++) {
    if (playerarray[missionarray[n] - 1] == 0) {
      hacked += mission_agent(missionarray[n]); //Function for agent if player is agent
    } else {
      hacked += mission_hacker(missionarray[n]); //Function for hacked if player is hacker
    }
  }

  required_hack = 1;
  if ((nofplayer > 6) && (nodeposition == 3)) {//If 7,8 man game and its node 4, the node requires 2 hacks
    required_hack = 2;
  }
  /*Increase score for hackers if requirement is met*/
  if (hacked >= required_hack) { //if Node is hacked or not
    reset_display();
    amountofhack_builder(hacked);
    display_string(1, hackedmessage);
    display_string(2, amountofhack);
    while (invmyclock(5) != 0) {
      display_update();
    }
    hacker_score++;//increment the hacker score
  } else {  /*ELSE increase score for agents*/
    reset_display();
    amountofhack_builder(hacked);
    display_string(1, securedmessage);
    display_string(2, amountofhack);
    while (invmyclock(5) != 0) {
      display_update();
    }
    agent_score++;//increment the agent score
    *porte += pow2(2, 7 - nodeposition); /*Lits LED for each agent score(SECURE), from the left*/
  }
  idnode_builder2(); /*Builds node for information_display with SECURE or HACKED*/
  switch (nodeposition) {
    case 0:
    copier(idnode, idnode1);
    break;

    case 1:
    copier(idnode, idnode2);
    break;

    case 2:
    copier(idnode, idnode3);
    break;

    case 3:
    copier(idnode, idnode4);
    break;
  }
  hacked = 0;

  if (agent_score == 3) {     /*Agents win*/
    return 7;
  } else if (hacker_score == 3){    /*Hackers win*/
    return 7;
  } else {
    nodeposition++;//change the node position
    return 3;//go back to talking phase
  }
}

int end() { //phase 7
  victory_defeat();//Display the victor
  while (invmyclock(20) != 0) {
    display_update();
  }
  return 8;
}
