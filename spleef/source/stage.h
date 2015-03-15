#define STAGE_WIDTH 16 //Screen Size
#define STAGE_HEIGHT 12 //Screen Size
typedef struct
{
	int block[STAGE_WIDTH][STAGE_HEIGHT];
}Stage;
void generateNormalStage(Stage* a,int b);
void stageInit();
void renderStage(Stage* a);
