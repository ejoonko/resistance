void labinit(void);

extern char title[];
extern int nofplayer; //Number of players
extern int playerarray[]; //array for storing player info
extern int nofhacker; // number of hackers
extern int selectiondone; // criteria for when the selection is done
extern int nodeposition; //the position of the relevant node being used
extern int playerposition; //position of the player selecting
extern int missionarray[]; //array for players selected for the mission
extern int yesvotes; //how many votes were accepts
extern int node_rejected; //number of nodes rejected
extern int hacked; //number of hacks in the mission
extern int hacker_score; //score for hackers
extern int agent_score; //score for agents
extern int p_constant;
extern int required_hack;

extern int nodes [];

/*LED*/
extern volatile int* porte;

/*Buttons and switches*/
extern volatile int* portd;

int getbtns(void);
int getsw(void);

/*Timer*/
void time2string( char *, int );
void display_string(int line, char *s);
void display_update(void);
void tick( unsigned int * timep );
extern char string[];
extern int mytime;

/*information display*/
extern char idnode[];
extern char idnode1[];
extern char idnode2[];
extern char idnode3[];
extern char idnode4[];

/*n of Player select phase*/
extern char nofplayerstring1[];
extern char nofplayerstring2[];
extern char amount2select[];

/*reveal printing*/
extern char revealstring[];
extern char agentreveal[];
extern char hackerreveal[];
extern char arehackers[];

/*Selection printing*/
extern char useswitches[];
extern char player_select_string[];
extern char display_p_string[];
extern char two_hacks_required[];
extern char lastnode[];


/*Vote printing*/
extern int a_constant;
extern int r_constant;
extern char votestring[];

extern char votestring1[];

extern char votestring2[];

extern char votestring3[];

extern char votestring4[];

void votestring_builder();
extern char acceptrefuse[];
extern char voteaccepted[];
extern char voterejected[];
extern char vote_accepted_string[];
extern char vote_rejected_string[];

extern char vote_accepted_string1[];
extern char vote_rejected_string1[];

extern char vote_accepted_string2[];
extern char vote_rejected_string2[];

extern char vote_accepted_string3[];
extern char vote_rejected_string3[];

extern char vote_accepted_string4[];
extern char vote_rejected_string4[];

/*mission printing*/
extern char securehack[];
extern char amountofhack[];

/*end phase printing*/
extern char agent_victory[];
extern char hacker_victory[];
