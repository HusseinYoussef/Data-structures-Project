#include <iostream>
using namespace std;
#include <windows.h>

// The four regions: A, B, C , and D.
enum REGION {	
	A_REG,
	B_REG,
	C_REG,
	D_REG
};

// Enemy types: Paver, Fighter, and Shielded Fighter.
enum Etype {
	PVR,
	FITR,
	SHLD_FITR,
	CATAPULT,			// Mangonel.
	ARCHER				// Archer. 
};

// Constants.
#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3
//#define EnemyShape		219		// ASCII code of enemy char shape.

struct enemy
{
	int ID;						// Each enemy has a unique ID (sequence number).
	Etype Type;					// PVR, FITR, SHLD_FITR.
	int Arrival_Time;
	float Health;				// Enemy health.
	float Fire_Power;
	int Reload_Time;
	REGION Region;				// Region of this enemy.
	int Distance;				// Distance to the castle.
	enemy* Next_Enemy;
	int Speed;					// 1 for shielded & catapult , 2 for fighter & healer , 3 for paver.
	int First_Time_Shot;		// Initially = -1.
	int Fight_Delay;			//
	int Kill_Delay;				//
	int Fight_Time;				// Initially = -1. (First time the enemy gets shot by the tower)
	int Death_time;
};

struct Enemies_Lists{			// Changed the name from Tower_Lists. (Active & Killed)
	int size;
	enemy* head;
	enemy* tail;				// Makes us don't need traverse to insert at the end. 
};

struct Queue{					// For Inactive list.
	int size;
	enemy* front;
	enemy* rear;
};

//Tower
struct Tower
{
	int TW;							// Tower width.
	int TL;							// Tower Height.
	float Health;
	int Unpaved_Distance;			// Initially = 30.
	Enemies_Lists Active_List;
	Queue InActive_List;
	Enemies_Lists Killed_List;
};

struct Priority_Constants{	// For all shielded enemies.
	float C1;
	float C2;
	float C3;
};

struct castle
{
	// Starting x,y.
	int Xstrt;
	int Ystrt;
	int W;					// Width.
	int L;					// Height.
	int Available_Enemies;	// Number of enemies they can attack at a time.
	int Tower_Fire_Power;
	int Initial_Towers_Health;	// To determine the total damage.
	Tower towers[4];		// Castle has 4 towers
	Priority_Constants P_Constants;
};

//Functions Prototype

/*A function to set the position of cursor on the screen*/
void gotoxy(int x, int y);

/*A function to set the command window lenght and height for the game specification*/
void SetWindow();

/*A function to color the cmd text*/
void color(int thecolor);

/*A function to partition the castle into regions (A,B,C,D)*/
void DrawRegions(const castle & C);

/*A function to draw the castle and the towers*/
void DrawCastle(const castle & C,int SimulationTime);

/*A function to draw a single enemy using its distance from the castle*/
void DrawEnemy(const enemy& E, int Ypos=0);

/*A function to draw a list of enemies exist in all regions and ensure there is no overflow in the drawing*/
void DrawEnemies(enemy* enemies[],int size);

/*Student use this function to print his/her message*/
void PrintMsg(char*msg);

