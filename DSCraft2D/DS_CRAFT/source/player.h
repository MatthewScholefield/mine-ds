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
void updateplayer(playerActor* player);
u16* playerGfx();
void playerCreateGfx();
