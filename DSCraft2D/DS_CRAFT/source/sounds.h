#define GRASS_A 0
#define GRASS_B 1
#define GRASS_C 2
#define GRASS_D 3
#define STONE_A 4
#define STONE_B 5
#define STONE_C 6
#define STONE_D 7
#define GRAVEL_A 8
#define GRAVEL_B 9
#define GRAVEL_C 10
#define GRAVEL_D 11
#define HURT 12
#define WOOD_A 13
#define WOOD_B 14
#define WOOD_C 15
#define WOOD_D 16
#define SAND_A 17
#define SAND_B 18
#define SAND_C 19
#define SAND_D 20
#define PIG_A 21
void playSound(int sound);
void initSounds();
void soundUpdate();
typedef struct{
int musictype;
int volumechanging;
int volume;
bool playing;
int frames;
}musicStruct;
