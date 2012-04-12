#define TITLE 0
#define CALM 1
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
