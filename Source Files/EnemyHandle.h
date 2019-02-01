#include "OutputFilePrint.h"

enemy* Take_From_Beginning(Enemies_Lists*);							// Removes the first enemy from the list and returns it.

void Activate(castle* Castle, int Time_Step ,int Tower_Index);		// Moves whose time arrival = game time from inactive list to active list.

float Calculate_Shielded_Priority(enemy*,float,float,float,int);	// Returns the priority for a shielded fighter.

bool IsEmpty(Enemies_Lists*);										// Returns true if the list 'Enemies_Lists' is empty.

void Hit_Enemy(castle* Castle , int Tower_Index,enemy* ENEMY,int Time_Step);		// Hits enemy.

void Delete_All_Lists(castle*);										// Deletes all the lists after the game ends.

bool All_Enemies_Killed(castle* Castle);							// True if all enemies are killed (all inActive & Active lists are empty) & false otherwise.

bool All_Towers_Destroyed(castle* Castle);							// True if all towers are destroyed (health of all towers = 0) & false otherwise.

void Towers_Attack_Enemies(castle* Castle,int Tower_Index, int Time_Step);			// Towers attack enemies.

void Enemies_Attack_Towers(castle* Castle,int Tower_Index,int Time_Step);			// Enemies attack towers.	

float Damage_Enemy_To_Tower(castle*,enemy*);						// Returns the damage made by an enemy to a tower.

float Damage_Tower_To_Enemy(castle*,enemy*);						// Returns the damage made by a tower to an enemy.

int Next_Tower_Index(castle* Castle, int Tower_Index);				// Returns the index of the next available (Not destroyed) tower.

void Active_Merge(castle* Castle,int Tower_Index,int Time_Step);	// Merges the active lists of the 2 towers (destroyed & next tower)

void Update_Distance(castle* Castle,int Tower_Index,int Time_Step);	// Handles enemies movement.

enemy * Delete_Enemy_From_list(enemy* ENEMY ,Enemies_Lists* );		// Deletes an enemy from a list and returns it.
