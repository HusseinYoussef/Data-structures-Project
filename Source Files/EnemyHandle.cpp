#include <time.h>
#include "EnemyHandle.h"

float Calculate_Shielded_Priority(enemy* ENEMY,Priority_Constants* P_Constants , int Time_Step){ // Returns the priority for a shielded fighter.
	int Remaining_Reload_Time = ENEMY->Reload_Time - ((Time_Step - ENEMY ->Arrival_Time) % ENEMY ->Reload_Time);
	return (((float(ENEMY ->Fire_Power)/(ENEMY ->Distance)) * P_Constants -> C1 ) + (P_Constants -> C2 / (Remaining_Reload_Time + 1)) + (ENEMY ->Health * P_Constants -> C3));
}

enemy* Take_From_Beginning(Enemies_Lists* Enemies_List){		// Removes the first enemy from the list and returns it.
	if(IsEmpty(Enemies_List))									// If the list is empty return NULL.
		return NULL;
	enemy* temp = Enemies_List -> head;
	if(Enemies_List -> tail == Enemies_List -> head)			// If only one enemy in the list left.
		Enemies_List -> tail = NULL;
	Enemies_List -> head = Enemies_List -> head -> Next_Enemy;
	Enemies_List -> size--;
	temp ->Next_Enemy = NULL;
	return temp;
}

void Active_Merge(castle* Castle,int Tower_Index,int Time_Step)		// Merges the active lists of the 2 towers (destroyed & next tower)
{
	int Next_Tower = Next_Tower_Index(Castle,Tower_Index);
	if(Next_Tower == -1)		// All Towers are destroyed.
		return;

	Enemies_Lists* Destroyed_Active_List = &Castle -> towers[Tower_Index].Active_List;
	Enemies_Lists* Active_List = &Castle -> towers[Next_Tower].Active_List;

	while(!IsEmpty(Destroyed_Active_List))		// Traverses through the destroyed active list to add them into the other tower.
	{
		enemy * ENEMY=Take_From_Beginning(Destroyed_Active_List);
		switch (Next_Tower)
		{
	    	case 0:
				ENEMY -> Region = A_REG;
			break;
	    	case 1:
				ENEMY -> Region = B_REG;
			break;
	    	case 2:
				ENEMY -> Region = C_REG;
			break;
		 	case 3:
				ENEMY -> Region = D_REG;
		}

		int Distance;
		if(ENEMY ->Distance < Castle->towers[Next_Tower].Unpaved_Distance)
			Distance = Castle->towers[Next_Tower].Unpaved_Distance +1;
		else
			Distance = ENEMY->Distance;

		ENEMY ->Distance = Distance;


		bool Inserted = false;
		if(ENEMY ->Type == SHLD_FITR){			// Shielded Fighter.
			float Current_Priority = Calculate_Shielded_Priority(ENEMY,&Castle->P_Constants,Time_Step);	// Storing the priority of the current enemy.
			if(IsEmpty(Active_List) || Active_List -> head ->Type != SHLD_FITR || Current_Priority > Calculate_Shielded_Priority(Active_List -> head,&Castle->P_Constants,Time_Step))			// case empty list.
				Insert_At_Location(ENEMY,NULL,Active_List);
			else								// More than one enemy.
			{
				enemy* pre  = Active_List -> head;
				enemy* temp = Active_List -> head -> Next_Enemy;
				while((temp != NULL) && (temp ->Type == SHLD_FITR))		// Traverses through shielded enemies in active list (which are stored at the beginning).
				{														// First Enemy has been checked earlier.
					if(Current_Priority > Calculate_Shielded_Priority(temp,&Castle -> P_Constants,Time_Step))
					{
						Insert_At_Location(ENEMY,pre,Active_List);
						Inserted = true;		// If found a shielded with higher priority insert the enemy before him & skip the rest of the loop.
						break;
					}						
					pre = temp;                                               
					temp = temp ->Next_Enemy;
				}
				if(!Inserted)
					Insert_At_Location(ENEMY,pre,Active_List);		// If not found higher priority insert the enemy after the last shielded enemy.
			}
		}
		else	
			Insert_At_Location( ENEMY , Active_List -> tail , Active_List);	// If not shielded insert at the end of the list.
	}
}

void Activate(castle* Castle, int Time_Step ,int Tower_Index){				// Moves whose time arrival = game time from inactive list to active list.
	Enemies_Lists* Active_List = &Castle -> towers[ Next_Tower_Index(Castle,Tower_Index)].Active_List;
	while(!QIsEmpty(&Castle -> towers[Tower_Index].InActive_List) && Castle -> towers[Tower_Index].InActive_List.front ->Arrival_Time == Time_Step)
	{
		enemy * ENEMY=Dequeue(&Castle -> towers[Tower_Index].InActive_List);

		bool Inserted = false;
		if(ENEMY ->Type == SHLD_FITR){			// Shielded Fighter.
			float Current_Priority = Calculate_Shielded_Priority(ENEMY,&Castle -> P_Constants,Time_Step);	// Storing the priority of the current enemy.
			if(IsEmpty(Active_List) || Active_List -> head ->Type != SHLD_FITR || Current_Priority > Calculate_Shielded_Priority(Active_List -> head,&Castle -> P_Constants,Time_Step))			// case empty list.
				Insert_At_Location(ENEMY,NULL,Active_List);
			else								// More than one enemy.
			{
				enemy* pre  = Active_List -> head;
				enemy* temp = Active_List -> head -> Next_Enemy;
				while((temp != NULL) && (temp ->Type == SHLD_FITR))		// Traverses through shielded enemies in active list (which are stored at the beginning).
				{														// First Enemy has been checked earlier.
					if(Current_Priority > Calculate_Shielded_Priority(temp,&Castle -> P_Constants,Time_Step))
					{
						Insert_At_Location(ENEMY,pre,Active_List);
						Inserted = true;		// If found a shielded with higher priority insert the enemy before him & skip the rest of the loop.
						break;
					}						
					pre = temp;                                               
					temp = temp ->Next_Enemy;
				}
				if(!Inserted)
					Insert_At_Location(ENEMY,pre,Active_List);	// If not found higher priority insert the enemy after the last shielded enemy.
			}
		}
		else	
			Insert_At_Location( ENEMY , Active_List -> tail , Active_List);	// If not shielded insert at the end of the list.
	}
}

int Next_Tower_Index(castle* Castle, int Tower_Index){						// Returns the index of the next available (Not destroyed) tower.
	int Next_Tower_Index = Tower_Index;

	int i=0;
	while(Castle->towers[Next_Tower_Index].Health == 0 && i!=4)		// If the tower is destroyed move it to the next tower.
	{
		Next_Tower_Index = (Next_Tower_Index+1) % 4;				//
		i++;	
	}
	if(i == 4)				// All Towers are destroyed.
		return -1;

	return Next_Tower_Index;
}

bool IsEmpty(Enemies_Lists* Enemies_List){		// Returns true if the list 'Enemies_Lists' is empty.
	if(Enemies_List -> head == NULL)
		return true;
	else 
		return false;
}

void Delete_All_Lists(castle* Castle){									// Deletes all lists after the game ends.
	for(int Tower_Index = 0; Tower_Index < 4; Tower_Index++)			// Loops through towers.
	{
		enemy* temp = Castle ->towers[Tower_Index].Killed_List.head;	// Deleting killed lists.
		enemy* temp2;
		while(temp != NULL)
		{
			temp2 = temp;
			temp = temp->Next_Enemy;
			delete temp2;
		}
		
		temp = Castle ->towers[Tower_Index].InActive_List.front;		// Deleting inactive lists.
		while(temp != NULL)
		{
			temp2 = temp;
			temp = temp->Next_Enemy;
			delete temp2;
		}
		
		temp = Castle ->towers[Tower_Index].Active_List.head;			// Deleting active lists.
		while(temp != NULL)
		{
			temp2 = temp;
			temp = temp->Next_Enemy;
			delete temp2;
		}

	}
}

bool All_Enemies_Killed(castle* Castle){		// True if all enemies are killed (all inActive & Active lists are empty) & false otherwise.
	if(QIsEmpty(&Castle -> towers[0].InActive_List) && IsEmpty(&Castle -> towers[0].Active_List) && QIsEmpty(&Castle -> towers[1].InActive_List) && IsEmpty(&Castle -> towers[1].Active_List) && QIsEmpty(&Castle -> towers[2].InActive_List) && IsEmpty(&Castle -> towers[2].Active_List) && QIsEmpty(&Castle -> towers[3].InActive_List) && IsEmpty(&Castle -> towers[3].Active_List))
		return true;
	else
		return false;
}

bool All_Towers_Destroyed(castle* Castle){		// True if all towers are destroyed (health of all towers = 0) & false otherwise.
	for(int Tower_Index = 0; Tower_Index < 4; Tower_Index++)
		if(Castle -> towers[Tower_Index].Health != 0)
			return false;
	return true;
}

void Kill(enemy* ENEMY,Enemies_Lists* Killed_List){		// Insert the killed enemies to Killed_List sorted by KTS.
	if(Killed_List -> head == NULL)
		Insert_At_Location(ENEMY,NULL,Killed_List);		// Insert at first.
	else
		Insert_At_Location(ENEMY,Killed_List->tail,Killed_List);
}

void Towers_Attack_Enemies(castle* Castle,int Tower_Index, int Time_Step)		// Towers attack enemies.
{
	Enemies_Lists* Active_List = &Castle->towers[Tower_Index].Active_List;
	if(IsEmpty(Active_List))									// If Active list is empty return without hitting.
		return;
	else														// Active list is not empty.
	{
		enemy* ENEMY = Active_List->head;
		int i=0;
		while(i<Castle->Available_Enemies && ENEMY != NULL)		// Hit enemies to the end of the list or until hitting Available_Enemies.
		{
			if(ENEMY ->First_Time_Shot == -1)					// First time shot.
			{
				ENEMY->First_Time_Shot = Time_Step;
				ENEMY->Fight_Delay = ENEMY ->First_Time_Shot - ENEMY->Arrival_Time;
			}
					
			Hit_Enemy(Castle,Tower_Index,ENEMY,Time_Step);
	
			ENEMY = ENEMY->Next_Enemy;
			i++;
		}
	}
}
                                                          
void Hit_Enemy(castle* Castle , int Tower_Index,enemy* ENEMY,int Time_Step)		// Hits an enemy.
{
	ENEMY->Health -= Damage_Tower_To_Enemy(Castle,ENEMY);

	if(ENEMY->Health <= 0)
	{
		ENEMY -> Kill_Delay = Time_Step - ENEMY ->First_Time_Shot;	// Storing killing info
		ENEMY -> Fight_Time = Time_Step - ENEMY ->Arrival_Time;		//
		ENEMY -> Death_time = Time_Step;							//
		ENEMY -> Health=0;
		enemy* temp = Delete_Enemy_From_list(ENEMY,&Castle->towers[Tower_Index].Active_List);
		Kill(temp,&Castle->towers[Tower_Index].Killed_List);	
	}

}
  
float Damage_Tower_To_Enemy(castle* Castle,enemy* ENEMY)			// Returns the damage made by a tower to an enemy.
{
	int k;
	if(ENEMY->Type == SHLD_FITR)
		k = 2;
	else
		k = 1;

	return float((1.0/ENEMY->Distance) * Castle->Tower_Fire_Power * (1.0/k));
}


enemy* Delete_Enemy_From_list(enemy* ENEMY ,Enemies_Lists* list)	// Deletes an enemy from a list and returns it.
{
	if(list->head==ENEMY)
	{
		return Take_From_Beginning(list);
	}

	enemy *scan = list -> head -> Next_Enemy;
	enemy *pre  = list -> head;

	while(scan!=NULL)
	{
		if(scan==ENEMY)
		{
			list -> size--;
			pre  -> Next_Enemy = scan -> Next_Enemy;
			if(scan->Next_Enemy == NULL)
				list -> tail = NULL;
			scan -> Next_Enemy=NULL;
			return scan;
		}
		pre  = scan;
		scan = scan -> Next_Enemy;
	}
	return NULL;
}

void Enemies_Attack_Towers(castle* Castle,int Tower_Index,int Time_Step)		// Enemies attack towers.
{
	Enemies_Lists* Active_List = &Castle->towers[Tower_Index].Active_List;

	enemy* ENEMY = Active_List->head;

	while(ENEMY!=NULL && Castle->towers[Tower_Index].Health > 0)				// While there're enemies && tower isn't destroyed. 	
	{
		if(ENEMY->Type != PVR )													// All enemies attack their tower except pavers.
			{
				if(ENEMY ->Type == CATAPULT && ENEMY->Distance > 31)	
				{
					ENEMY = ENEMY->Next_Enemy;
					continue;
				}
				Castle->towers[Tower_Index].Health -= Damage_Enemy_To_Tower(Castle,ENEMY);	// Decrease its health by the damage from the enemy.
			}
		ENEMY = ENEMY->Next_Enemy;
	}

	if(Castle->towers[Tower_Index].Health <= 0)			// If destroyed.
	{
		Castle -> towers[Tower_Index].Health = 0;
		Active_Merge(Castle,Tower_Index,Time_Step);		// Merge its killed list with the next tower killed list.
	}
}

float Damage_Enemy_To_Tower(castle* Castle,enemy* ENEMY)			// Returns the damage made by an enemy to a tower.
{
	int k;
	if(ENEMY->Type == SHLD_FITR)
		k = 2;
	else
		k = 1;

	return (((float)k/ENEMY->Distance) * ENEMY->Fire_Power);
}

void Update_Distance(castle* Castle,int Tower_Index,int Time_Step)	// Handles enemies movement.
{
	enemy* temp = Castle->towers[Tower_Index].Active_List.head;
	while(temp != NULL)
	{
		if(temp -> Distance - temp->Speed > 2)						// Any enemy can't get closer than 2 meters.
		{
			if(temp ->Type != PVR)
			{
				if(temp ->Type == CATAPULT && temp->Distance == 31) // When Catapult reaches 30 m stops.
				{
					temp = temp->Next_Enemy;
					continue;
				}
			}
			else						// If the enemy is a paver.
			{
				if((Time_Step - temp->Arrival_Time) % temp->Reload_Time == 0)		// When he paves.
				{
					int Unpaved_Decrease = int(temp->Fire_Power - (temp->Distance - Castle->towers[Tower_Index].Unpaved_Distance));
					if(Unpaved_Decrease > 0)
						if(Castle->towers[Tower_Index].Unpaved_Distance - Unpaved_Decrease < 2)		// He can't pave closer than 2 m.
							Castle->towers[Tower_Index].Unpaved_Distance = 2;
						else
							Castle->towers[Tower_Index].Unpaved_Distance -= Unpaved_Decrease;
				}
			}

			if(temp->Distance - temp->Speed -1 > Castle->towers[Tower_Index].Unpaved_Distance)		// Enemies can't enter unpaved area.
				temp -> Distance-= temp->Speed;
			else
				temp -> Distance =  Castle->towers[Tower_Index].Unpaved_Distance +1;
		}
		else		// If the enemy moved he will get closer than 2 m.
		{
			temp -> Distance = Castle->towers[Tower_Index].Unpaved_Distance + 1;
		}

		temp = temp->Next_Enemy;
	}

}
