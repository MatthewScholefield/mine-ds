//A block has Two Functions
void DOOR_CLOSED_render(int a, int b); //Render the block
void DOOR_CLOSED_setup(); //And copy the blocks graphics
void DOOR_colision(playerActor* player,worldObject* world,int bx,int by,int result);
void ACTUAL_DOORUpdate(worldObject* world,int x,int y);
void DOOR_TOP_update(worldObject* world,int x,int y);
void DOOR_mine(worldObject* world,int* mine_time,int x,int y);
