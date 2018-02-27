#include <stdio.h>
#include <stdlib.h>
int p = 0;

int main() {
  labinit();

  /*while (1) {
    myclock(60);
  }*/

  while(p != 8) {
    switch(p) {
      case 0: //player n select phase
      p = player_select_phase();
      break;

      case 1: //Game setup
      p = game_setup();
      break;

      case 2: //Player reveal phase
      p = player_reveal_phase();
      break;

      case 3: //Talking phase
      p = talking_phase();
      break;

      case 4: //selection phase
      p = selection_phase();
      break;

      case 5: //Voting phase
      p = voting_phase();
      break;

      case 6: //Mission phase
      p = mission_phase();
      break;

      case 7: //End phase
      p = end();
      break;
    }
  }
  return 0;
}
