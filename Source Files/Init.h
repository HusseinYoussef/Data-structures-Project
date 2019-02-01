#include <iostream>
#include "utility.h"
using namespace std;

bool ReadFromFile(castle*);						// Stores the data which is read from input.txt.

void Initialize(castle*);						// Makes all heads & tails = NULL, lists size = 0, unpaved distance = 30 for all lists in all towers.

void Determine_Enemy_Type(int,enemy*);			// Determines enemy type.

void Distribute_Enemy(char,castle*,enemy*);		// Sends the enemy to his list in his region (to Inactive_List).

void Insert_At_Location(enemy* ENEMY,enemy* pre,Enemies_Lists*);	// Insert enemy 'ENEMY' after the location 'pre' in 'Enemies_Lists'.

void Enqueue(Queue* InActive_List,enemy*);

enemy* Dequeue(Queue* InActive_List);

bool QIsEmpty(Queue* InActive_List);