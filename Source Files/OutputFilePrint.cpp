#include <iostream>
#include <fstream>
#include "OutputFilePrint.h"
using namespace std;

void Final_Results(castle* Castle){
	int size = Castle ->towers[0].Killed_List.size + Castle ->towers[1].Killed_List.size +Castle ->towers[2].Killed_List.size +Castle ->towers[3].Killed_List.size;
	// Size of all killed enemies.
	enemy** All_Killed_List = new enemy*[size];
	
	enemy*A,*B,*C,*D;
	A = Castle->towers[0].Killed_List.head;
	B = Castle->towers[1].Killed_List.head;
	C = Castle->towers[2].Killed_List.head;
	D = Castle->towers[3].Killed_List.head;
	int Time = 0;
	int Main_Index = 0;
	while(Main_Index != size)	 // Traverses through ALL_KILLED_LIST and inserting in it the 4 killed lists sorted by Deat_time.
	{
		while(A !=NULL && A->Death_time == Time)
			{
				All_Killed_List[Main_Index]=A;
				Main_Index++;
				A=A->Next_Enemy;
			}
		while(B!=NULL && B->Death_time == Time)
			{
				All_Killed_List[Main_Index]=B;
				Main_Index++;
				B = B->Next_Enemy;
			}
		while(C!=NULL && C->Death_time == Time)
			{
				All_Killed_List[Main_Index]=C;
				Main_Index++;
				C = C->Next_Enemy;
			}
		while(D!=NULL && D->Death_time == Time)
			{
				All_Killed_List[Main_Index]=D;
				Main_Index++;
				D = D->Next_Enemy;
			}
		Time++;
	}

	Bubble_Sort(All_Killed_List, size);
	Print_To_File(Castle,All_Killed_List,size);
}

void Bubble_Sort(enemy** All_Killed_List, int size){	// For sorting the killed list by fight delay (in case enemies have the same death time).
	while(true)
	{
		int swapped = 0;

		for(int Main_Index = 0; Main_Index<size - 1; Main_Index ++)
		{
			if(All_Killed_List[Main_Index]->Death_time == All_Killed_List[Main_Index + 1]->Death_time && All_Killed_List[Main_Index]->Fight_Delay > All_Killed_List[Main_Index + 1]->Fight_Delay)
				{
					Swap(All_Killed_List, Main_Index, Main_Index+1);
					swapped = 1;
				}
		}
		if(swapped == 0)
			break;
	}

}

void Swap(enemy** All_Killed_List, int First_Index, int Last_Index){
	enemy* temp;
	temp = All_Killed_List[First_Index];
	All_Killed_List[First_Index] = All_Killed_List[Last_Index];
	All_Killed_List[Last_Index] = temp;
}


void Print_To_File(castle* Castle,enemy** All_Killed_List,int size){
	ofstream output;
	output.open("output.txt");
	if(!output.is_open()) {					// Checkes if the file doesn't exist at its location or an error occured.
		cout << "Error occured .. check if the file exists." << endl; 
		return; 
	}

	for(int index = 0 ; index < size; index++)
		output << All_Killed_List[index]->Death_time << " "<< All_Killed_List[index]->ID << " "<< All_Killed_List[index]->Fight_Delay << " "<< All_Killed_List[index]->Kill_Delay << " "<< All_Killed_List[index]->Fight_Time << endl; 

	for(int Tower=0; Tower<4; Tower++)
		output << Castle->Initial_Towers_Health - Castle->towers[Tower].Health << " ";

	output << endl;

	for(int Tower=0; Tower<4; Tower++)
		output << Castle->towers[Tower].Unpaved_Distance << " ";

	output << endl;

	int Total_Fight_Delay = 0;
	int Total_Kill_Delay = 0;
	for(int index = 0; index < size; index ++)
	{
		Total_Fight_Delay+= All_Killed_List[index]->Fight_Delay;
		Total_Kill_Delay+= All_Killed_List[index]->Kill_Delay;
	}

	if(all_Enemies_Killed(Castle))			// Case winning.
	{
		output << "Game is WIN." << endl;
		output << "Total Number Of Enemies : " << size << endl;							// Number of enemies.
		output << "Average Fight Delay : " << (float)Total_Fight_Delay/size << endl;
		output << "Average Kill Delay  : " << (float)Total_Kill_Delay/size << endl;		// Average Fight_Delay & Kill_Delay.
	}
	else									// Case losing.
	{
		output << "Game is LOSS." << endl;
		output << "Number of killed : " << size << endl;
		int Total_Alive_Num = 0;
		for(int Tower = 0; Tower < 4; Tower ++)
		{
			Total_Alive_Num += Castle->towers[Tower].Active_List.size;
			Total_Alive_Num += Castle->towers[Tower].InActive_List.size;
		}
		
		output << "Number of alive enemies : " << Total_Alive_Num << endl;
		output << "Average Fight Delay : " << (float)Total_Fight_Delay/size << endl;
		output << "Average Kill Delay  : " << (float)Total_Kill_Delay/size << endl;		// Average Fight_Delay & Kill_Delay.
	}
	output.close();
}

bool all_Enemies_Killed(castle* Castle){		// True if all enemies are killed (all inActive & Active lists are empty) & false otherwise.
	if(QIsEmpty(&Castle -> towers[0].InActive_List) && isEmpty(&Castle -> towers[0].Active_List) && QIsEmpty(&Castle -> towers[1].InActive_List) && isEmpty(&Castle -> towers[1].Active_List) && QIsEmpty(&Castle -> towers[2].InActive_List) && isEmpty(&Castle -> towers[2].Active_List) && QIsEmpty(&Castle -> towers[3].InActive_List) && isEmpty(&Castle -> towers[3].Active_List))
		return true;
	else
		return false;
}

bool isEmpty(Enemies_Lists* Enemies_List){		// Returns true if the list 'Enemies_Lists' is empty.
	if(Enemies_List -> head == NULL)
		return true;
	else 
		return false;
}
