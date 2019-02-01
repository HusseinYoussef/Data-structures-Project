#include "Init.h"
#include <fstream>

bool ReadFromFile(castle* Castle){			// Stores the data which is read from input.txt.
	ifstream Read("input.txt");
	if(!Read.is_open()) {					// Checkes if the file doesn't exist at its location or an error occured.
		cout << "Error occured .. check if the file exists." << endl; 
		return false; 
	}
	
	int TowerHealth;	//Temporary variable to store the towers health (All have the same health) We need to compare it later to estimate damagaes.
	Read >> TowerHealth>> Castle -> Available_Enemies >> Castle -> Tower_Fire_Power;							//Reading the first 2 lines.
	Read >> Castle -> P_Constants.C1 >> Castle -> P_Constants.C2  >> Castle -> P_Constants.C3 ;					//
	Castle->Initial_Towers_Health = TowerHealth;
	for(int i=0; i<4; i++)						// Sets the health of all towers with the TowerHealth red from the file.
		Castle -> towers[i].Health = (float)TowerHealth;

	while(true){
		int ID, type;	char R;					// Variable ID to test the end of the file (if -1 break) , type & R are for enums.
		Read >> ID ;
		if(ID == -1)							// Ends reading from file when reaching -1 at the first of a line.
			break;

		enemy* ENEMY = new enemy;				// Making a new enemy (Struct).
		
		ENEMY -> ID = ID;
		Read >> type >> ENEMY->Arrival_Time >> ENEMY->Health >> ENEMY->Fire_Power >> ENEMY->Reload_Time >> R;

		ENEMY ->Next_Enemy = NULL;
		Determine_Enemy_Type(type,ENEMY);		// Determines the type of the enemy.

	    Distribute_Enemy(R,Castle,ENEMY);		// Adding the region to the enemy and adding to the inactive list (All enemies in the region).
	}

	Read.close();
	return true;
}

void Initialize(castle* Castle){		// Makes all heads & tails = NULL & list size = 0 & unpaved distance = 30 for all lists in all towers.
	for(int i=0; i<4; i++){
		Castle -> towers[i].Active_List.head=NULL;		// All heads = NULL.
		Castle -> towers[i].InActive_List.front=NULL;	//
		Castle -> towers[i].Killed_List.head=NULL;		//
		Castle -> towers[i].Active_List.size=0;			// All sizes = 0.
		Castle -> towers[i].InActive_List.size=0;		//
		Castle -> towers[i].Killed_List.size=0;			//
		Castle -> towers[i].Active_List.tail = NULL;	// All tails = NULL.
		Castle -> towers[i].InActive_List.rear = NULL;	//
		Castle -> towers[i].Killed_List.tail = NULL;	//
		Castle -> towers[i].Unpaved_Distance = 30;		// Unpaved distance for each tower initially = 30.
	}
}

void Determine_Enemy_Type(int type,enemy* ENEMY){		// Determines enemy type.
	switch (type)
		{
	    	case 0:
				ENEMY -> Type = PVR;					// Storing the enemy type.
				ENEMY -> Speed = 3;						// Storing the enemy speed.
				ENEMY -> First_Time_Shot = -1;
				ENEMY -> Distance = 60;					// When becomes active starts moving from distance 60.
			break;
	    	case 1:
				ENEMY -> Type = FITR;
				ENEMY -> Speed = 4;
				ENEMY -> First_Time_Shot = -1;
				ENEMY -> Distance = 60;					// When becomes active starts moving from distance 60.
			break;
	    	case 2:
				ENEMY -> Type = SHLD_FITR;
				ENEMY -> Speed = 2;
				ENEMY -> First_Time_Shot = -1;
				ENEMY -> Distance = 60;					// When becomes active starts moving from distance 60.
			break;
		 	case 3:
				ENEMY -> Type = CATAPULT;
				ENEMY -> Speed = 1;
				ENEMY -> First_Time_Shot = -1;
				ENEMY -> Fire_Power = ENEMY -> Fire_Power * 5;		// Catapult has a massive fire power. 	
				ENEMY -> Distance = 60;					// When becomes active starts moving from distance 60.
			break;
			case 4:
				ENEMY -> Type = ARCHER;
				ENEMY -> Speed = 0;
				ENEMY -> First_Time_Shot = -1;
				ENEMY -> Distance = 60;					// When becomes active starts moving from distance 60.
		}
}

void Distribute_Enemy(char R,castle* Castle,enemy* ENEMY){		// Sends the enemy to his list in his region (to Inactive_List).
	switch(R){
		case 'A':
			ENEMY -> Region = A_REG;							// Storing the region of the enemy.
			Enqueue(&Castle->towers[0].InActive_List,ENEMY);	// Adding the enemy to the end of the inactive list (sent the tail as pre).
			break;
		case 'B' : 
			ENEMY -> Region = B_REG;
			Enqueue(&Castle->towers[1].InActive_List,ENEMY);
			break;
		case 'C' : 
			ENEMY -> Region = C_REG;
			Enqueue(&Castle->towers[2].InActive_List,ENEMY);
			break;
		case 'D' :
			ENEMY -> Region = D_REG;
			Enqueue(&Castle->towers[3].InActive_List,ENEMY);
	}
}

void Insert_At_Location(enemy* ENEMY,enemy* pre,Enemies_Lists* Enemy_List){		// Insert enemy 'ENEMY' after the location 'pre' in 'Enemies_Lists'.
	if(pre == NULL)									// If wanted the last send the tail as pre , if wanted the first send NULL as pre.
	{
		if(Enemy_List -> head == NULL){								// If the list is empty.
			Enemy_List -> head = Enemy_List -> tail = ENEMY;
			ENEMY->Next_Enemy = NULL;
		}else{														// Inserts at the first of non empty list.
			ENEMY->Next_Enemy = Enemy_List -> head;
			Enemy_List -> head = ENEMY;
		}

		Enemy_List -> size++;
		return;
	}
	else if(pre->Next_Enemy == NULL)								// Insert at the end of the list.
			Enemy_List -> tail = ENEMY;
	
	ENEMY ->Next_Enemy = pre -> Next_Enemy;							// Attaching the pre with the enemy with the list.
	pre ->Next_Enemy = ENEMY;										//
	Enemy_List -> size++;
}

void Enqueue(Queue* InActive_List,enemy* ENEMY)	
{
	InActive_List->size++;
	if(QIsEmpty(InActive_List))
	{
		InActive_List ->front = ENEMY;
		InActive_List ->rear = ENEMY;
		return;
	}
	InActive_List->rear->Next_Enemy = ENEMY;
	InActive_List->rear = ENEMY;
}

enemy* Dequeue(Queue* InActive_List)
{
	enemy* temp = InActive_List->front;
	if(QIsEmpty(InActive_List))
		return NULL;

	InActive_List->size--;
	if(InActive_List->front == InActive_List->rear)		// Only one enemy in the queue.
	{
		InActive_List ->front = NULL;
		InActive_List ->rear = NULL;
		temp->Next_Enemy = NULL;
		return temp;
	}

	InActive_List ->front = InActive_List->front->Next_Enemy;
	temp->Next_Enemy = NULL;
	return temp;
}

bool QIsEmpty(Queue* InActive_List){
	if(InActive_List->front == NULL && InActive_List->rear == NULL)
		return true;
	return false;
}
