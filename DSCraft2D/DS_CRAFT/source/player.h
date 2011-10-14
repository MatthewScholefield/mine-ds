//A typedef struct is a structure and puts heaps of varibles in one place
typedef struct{
	int x;
	int y;
	int vy;
	int blockx;
	int blocky;
	int framenum;
	int playerseed; //For multiplayer
	bool pointingleft;
}playerActor;
//Function definations 
void updateplayer(playerActor* player);
u16* playerGfx();
void playerCreateGfx();
