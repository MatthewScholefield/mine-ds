bool addInventory (int blockID, int amount);
bool subInventory (int blockID, int amount);
int checkInventory (int blockID);
void clearInventory ();
void addInventory(int blockID);
#define NUM_INV_SPACES 30
typedef struct
{
	int blockId=0;
	int blockAmount =0;
}InvBlock;
typedef struct
{
	InvBlock blocks[32];
//	InvBlock armour[4];
	int hand;
}Inventory;
