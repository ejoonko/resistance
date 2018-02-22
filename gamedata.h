int nofplayer;
int* playerarray;
int nofhacker;
int selectiondone = 0;
int nodeposition = 0;
int playerposition = 0;
#define NEXTPLAYER (++playerposition % nofplayer)
#define PLAYER (playerposition)
int [] nodes = {
  2, 3, 2, 3, 3,
  2, 3, 4, 3, 4,
  2, 3, 3, 4, 4,
  3, 4, 4, 5, 5
};
