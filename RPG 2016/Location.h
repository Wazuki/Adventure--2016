#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Enemy.h"
#include "Item.h"
class Player;
class Item;

class Location
{
public:
	Location();
	~Location();
	bool Read(std::ifstream &_file);
	int GetX();
	int GetY();
	void PrintDescription();
	void GetMonstersAtLocation();
	void AddEnemyToLocation(Enemy* _enemy);
	void DeleteEnemyFromlocation(Enemy* _enemy);
	bool EnemiesPresent();
	bool AttackMonstersAtLoc(Player* _player);
	void SetExplored(bool _explored);
	bool IsExplored();
	//Item Functions
	void GetItemsAtLocation();
	void AddItemToLocation(Item* _item);
	void DeleteItemFromLocation(Item* _item);
	bool ItemsPresent();
	void PassItemToPlayer(Player* _player);

private:
	std::vector<Enemy*> enemiesAtLoc;
	std::vector<std::string> npcs;
	std::vector<Item*> itemsAtLoc;
	bool explored;
	int x;
	int y;
	std::string description;
};
