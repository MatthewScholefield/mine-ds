#define TITLE 1
#define CALM 2
#define HAL 3
#define PIG_H 4
#define PLAYER_H 5
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
