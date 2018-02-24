int nofplayer; //Number of players
int* playerarray; //array for storing player info
int nofhacker; // number of hackers
int selectiondone = 0;
int nodeposition = 0; //the position of the relevant node being used
//int playerposition = 0;
int yesvotes = 0; //how many votes were accepts
int node_rejected = 0; //number of nodes rejected
int hacked = 0; //number of hacks in the mission
int hacker_score = 0; //score for hackers
int agent_score = 0; //score for agents
#define NEXTPLAYER (++playerposition % nofplayer)
#define PLAYER (playerposition)
int [] nodes = {
  2, 3, 2, 3, 3,
  2, 3, 4, 3, 4,
  2, 3, 3, 4, 4,
  3, 4, 4, 5, 5
};
