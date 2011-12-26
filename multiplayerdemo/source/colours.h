#define ITEMS_IN_ARRAY(x) sizeof(x)/sizeof(*(x))
#define maxColours 18
int colours[]={
	RGB15(31,31,31),//0
	RGB15(31,31,0),//1
	RGB15(31,24,0),//2
	RGB15(31,12,0),//3
	RGB15(28,15,19),//4
	RGB15(26,4,18),//5
	RGB15(0,25,31),//6
	RGB15(0,0,31),//7
	RGB15(0,0,16),//8
	RGB15(6,27,6),//9
	RGB15(0,31,0),//10
	RGB15(0,16,0),//11
	RGB15(31,27,4),//12
	RGB15(31,8,8),//13
	RGB15(31,0,0),//14
	RGB15(16,0,0),//15
	RGB15(18,0,28),//16
	RGB15(16,8,2),
	RGB15(0,0,0)};//17
char colournames[][20]={
	"White",
	"Yellow",
	"Light Orange",
	"Dark Orange",
	"Pink",
	"Dark Pink",
	"Light Blue",
	"Blue",
	"Dark Blue",
	"Light Green",
	"Green",
	"Dark Green",
	"Gold",
	"Light Red",
	"Red",
	"Dark Red",
	"Purple",
	"Brown",
	"Black"};
	
