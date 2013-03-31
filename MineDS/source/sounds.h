#define TITLE 1
#define CALM 2
#define HAL2 3
#define PIG_H 4
#define PLAYER_H 5
#define COW_H 6
#define ZOMBIE_H 7
void playSoundNiFi(int sound);
void playSound(int sound);
void playMusic(int music);
void stopMusic();
void initSounds();
void soundUpdate();
typedef struct{
int musictype;
int volumechanging;
int volume;
bool playing;
int frames;
}musicStruct;
