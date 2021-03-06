#include <stdio.h>
#include <stdlib.h>
int p = -1;

int main() {
  labinit();
  reset_display();

  while(p != 8) {
    switch(p) {
      case -1: /*Showing title of game*/
      p = title_phase();
      break;

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
