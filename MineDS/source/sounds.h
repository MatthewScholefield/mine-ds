#define TITLE 1
#define CALM 2
#define PIANO 3
#define PIG_H 4
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
