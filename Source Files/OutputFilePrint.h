#include "Init.h"

void Final_Results(castle* Castle);

void Bubble_Sort(enemy** All_Killed_List, int size);

void Swap(enemy** All_Killed_List, int First_Index, int Last_Index);

void Print_To_File(castle* Castle,enemy** All_Killed_List,int size);

bool all_Enemies_Killed(castle* Castle);							// True if all enemies are killed (all inActive & Active lists are empty) & false otherwise.

bool isEmpty(Enemies_Lists*);										// Returns true if the list 'Enemies_Lists' is empty.
