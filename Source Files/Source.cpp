////////////////////////////////////////////////////////////////////////////////////////////////
//	Castle Simulation																		  //
//	Made by :: SEM_TEAM_13																	  //
//																							  //
//	-> Ahmed Adel AbdElhalim																  //
//	-> Ramy Mohammed Saied																	  //
//	-> Hassan Osama Hassan																	  //
//	-> Hussein Youssif Hussein																  //
//																							  //
//	2016 / 12 / 21																			  //
////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "EnemyHandle.h"
#include <time.h>
using namespace std;

void Simulate();								// Phase 1 simulation (every second kills 4 random enemies from each region & prints their data).
void Print_List(Enemies_Lists*);				// Prints the passed Enemies_List.
void YouWin(castle* Castle);
void YouLose(castle* Castle);
void PrintInfo(castle* Castle,int Current_Dead_Size[]);

int main(){
	Simulate();				                    // Phase 1 simulation.
	return 0;
}

void Print_List(Enemies_Lists* tower_list){		// Prints the passed Enemies_List.
	enemy* temp = tower_list -> head;
	if(temp == NULL)
	{
		cout << "< The list is empty! >" << endl << endl;
		return;
	}
	cout << " (Sequence - Type - Arrival time - Health - Fire Power - Reload period) " << endl;
	while(temp != NULL)
	{
		cout << temp -> ID << "\t" << temp ->Type << "\t"  <<temp->Arrival_Time << "\t"  << temp ->Health << "\t"  << temp ->Fire_Power << "\t"  << temp ->Reload_Time << endl ;
		temp = temp->Next_Enemy;
	}
	cout << endl << endl;
}

int Choose_Mode(){		// For choosing the mode : 1 for interactive ,2 for step by step or any other input for silent mode.
	system("color 71");
	cout << "Choose the mode :\n1 for the interactive mode \n2 for step-by-step mode \nOr any other input for silent mode :" << endl;
	int choise;
	cin >> choise;
	system("cls");
	system("color 01");
	return choise;
}

void Simulate(){
	SetWindow();
	castle Castle;						// Make our struct for Castle.

	Castle.Xstrt = CastleXStrt , Castle.Ystrt = CastleYStrt , Castle.W = CastleWidth , Castle.L = CastleLength;

	Initialize(&Castle);				// Makes all heads & tails = NULL & list size = 0 & unpaved distance = 30 for all lists in all towers.  
	if(!ReadFromFile(&Castle))			// Stores the data which is read from input.txt.
		return;

	int Time = int(time(0)) ;			// Storing the time in a variable.
	int Time_Step = 0 ;					// Initializes the time for the game with 0.
	bool flag = true;					// When false exits the game.
	int Choise = Choose_Mode();
	
	while(flag){  
		
		if((Time != time(0) && Choise == 2) || Choise != 2)				// Time is unchanged while time(0) changes each sec.The condition is true every second (Time updates inside).
		{
			int Current_Dead_Size[4];
			for(int i=0 ; i<4; i++)
				Current_Dead_Size[i] = Castle.towers[i].Killed_List.size;

			for(int Tower_Index = 0; Tower_Index < 4; Tower_Index++)	// Loops through towers to 1-Activate enemies. 2-Kill them. 3-Print lists.
			{	
				
				if(All_Enemies_Killed(&Castle) || All_Towers_Destroyed(&Castle))
				{
					flag = false;							// End the game.
					Final_Results(&Castle);
					break;
				}
				Activate(&Castle,Time_Step,Tower_Index);	// Moves whose time arrival = game time from inactive list to active list.
				Towers_Attack_Enemies(&Castle,Tower_Index,Time_Step);
				Enemies_Attack_Towers(&Castle,Tower_Index,Time_Step);
				Update_Distance(&Castle,Tower_Index,Time_Step);
			}

			if(Choise == 1 || Choise == 2)					// Shows screen output only for interactive and step-by-step mode.
			{
				int Total_Size = 0;
				for(int Tower=0; Tower<4; Tower++)
					Total_Size += Castle.towers[Tower].Active_List.size;
	
				enemy** enemies = new enemy*[Total_Size];	// A list of all active enemies in all the towers.
				int index = 0;
				for(int Tower=0; Tower<4; Tower++)
				{
					enemy* ENEMY = Castle.towers[Tower].Active_List.head;
					while(ENEMY != NULL)
					{
						enemies[index] = ENEMY;
						ENEMY = ENEMY->Next_Enemy;
						index++;
					}
				}
				DrawCastle(Castle,Time_Step);
				DrawEnemies(enemies,Total_Size);
				PrintInfo(&Castle,Current_Dead_Size);	// Prints the info at the end of the screen.
			}

			Time_Step ++;				// Increases game time by 1 each second.
			if(Choise == 1)				// For interactive mode.
				system("pause");
			else if(Choise == 2)		// For step-by-step mode.
				Time = int(time(0));	// Updates Time to compare with time(0) to loop every second.
		}
	}

	if(Choise == 1 || Choise == 2)
	{
		if(All_Enemies_Killed(&Castle))		// You win when all enemies are killed (you are the castle).
			YouWin(&Castle);
		else								// You lose when all towers are destroyed (for phase 2).
			YouLose(&Castle);
	}
	Delete_All_Lists(&Castle);
}

void YouWin(castle* Castle){				// In case of winning prints you have won.
	system("cls");
	system("color 27");
	gotoxy(Castle->Xstrt,Castle->Ystrt-5);
	cout << "**************************";
	gotoxy(Castle->Xstrt,Castle->Ystrt-4);
	cout <<"|     You have won!      |";
	gotoxy(Castle->Xstrt,Castle->Ystrt-3);
	cout << "**************************\n\n\n\n\n\n\n";
}

void YouLose(castle* Castle){				// In case of losing prints you have lost.
	system("cls");
	system("color 47");
	gotoxy(Castle->Xstrt,Castle->Ystrt-5);
	cout << "**************************";
	gotoxy(Castle->Xstrt,Castle->Ystrt-4);
	cout <<"|     You have lost!     |";
	gotoxy(Castle->Xstrt,Castle->Ystrt-3);
	cout << "**************************\n\n\n\n\n\n\n";
}

void PrintInfo(castle* Castle,int Current_Dead_Size[]){		// Prints info at the end of the screen.
	cout << "Total current enemies  A =" << Castle->towers[0].Active_List.size << "\tB =" << Castle->towers[1].Active_List.size << "\tC =" << Castle->towers[2].Active_List.size << "\tD =" << Castle->towers[3].Active_List.size << " \n";
	cout << "Last time step kill    A =" << Castle->towers[0].Killed_List.size - Current_Dead_Size[0] << "\tB =" << Castle->towers[1].Killed_List.size - Current_Dead_Size[1] << "\tC =" << Castle->towers[2].Killed_List.size - Current_Dead_Size[2] <<  "\tD =" << Castle->towers[3].Killed_List.size - Current_Dead_Size[3] << "\n";
	cout << "Total killed by        A =" << Castle->towers[0].Killed_List.size << "\tB =" << Castle->towers[1].Killed_List.size << "\tC =" << Castle->towers[2].Killed_List.size << "\tD =" << Castle->towers[3].Killed_List.size << "\n";
	cout << "Unpaved distance       A =" << Castle->towers[0].Unpaved_Distance << "\tB =" << Castle->towers[1].Unpaved_Distance << "\tC =" << Castle->towers[2].Unpaved_Distance << "\tD =" << Castle->towers[3].Unpaved_Distance << " \n";
}
