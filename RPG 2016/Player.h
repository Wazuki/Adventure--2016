#pragma once
#include <map>
#include <vector>
#include <Windows.h>
#include "Console.h"
#include "Character.h"

class Item;
class Enemy;
class Location;
class Armor;

class Player : public Character
{
public:
	Player();
	~Player();
	void Intro(HANDLE _handle);
	void Victory(HANDLE _handle, int _monstersSlain);
	void Lose(HANDLE _handle, int _monstersSlain, int _steps);
	void generateStats();
	void chooseRace();
	void chooseClass();
	int statMod(int stat);
	std::string statModAdjust(int stat);
	bool attack(Enemy* _enemy);
	int weaponDamage(int _minDmg, int _maxDmg);
	void displayStats();
	void gainXP(int gainedXP);
	void levelUp();
	void takeDamage(int _dmgTaken);
	int GetAC();
	int GetHP();
	void pickUpItem(Item* _item);
	void dropItem(Location* _world[][20]);
	void useItem(Location* _world[][20]);
	void seeInventory();
	void sellItems();
	void heal(int _healAmount);
	void adjustGold(int _gold);
	int getGold();
	Armor* getEquippedArmor();
	void EquipArmor(Armor* _armor, int _invLoc);
	int GetLevel();
	std::string GetName();
private:
	static const int MAX_WEAPONS = 3;
	int str, dex, mind;
	int ac, hp, xp, level, xpTNL;
	int gold;	
	int physical, subterfuge, knowledge, communication;
	std::string playerClass;
	std::string playerRace;
	std::string playerName;
	std::string weapons[MAX_WEAPONS];
	//std::string armor;
	std::string shield;
	std::vector<Item*> inventory;
	std::map<std::string, std::string> weaponTypes;
	std::map<int, int> weaponSelector;
	//std::map < std::string, int> armorTypes;
	Armor* equippedArmor;
};

