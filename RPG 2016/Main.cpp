#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <Windows.h>
#include <map>
#include <cstdlib>
#include <conio.h>
#include "Location.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"

using std::cout;
using std::cin;
using std::string;
using std::ios;
using std::ifstream;

const int LOGO_LINES = 9;
const int WORLD_MAX_X = 20;
const int WORLD_MAX_Y = 20;
const int MAX_ENEMIES = 8;
const int ROAM_CHANCE = 25;
const int MAX_SPAWNED_ENEMIES = 40;
const int STARTING_ENEMY_COUNT = 4;
const int MAX_ITEMS = 6;
const int MAX_SPAWNED_ITEMS = 20;
const int MAX_WEAPONS = 10;
const int MAX_ARMORS = 12;

const int STEPS_TO_VICTORY = 100;
int stepsTaken = 0;

std::vector<Enemy*> activeEnemies;
//int spawnedEnemies = 0; Legacy
int choice;

int initialSpawnLocations[STARTING_ENEMY_COUNT][2]{
	{ 2, 1 }, //Orc Fortress
	{ 17, 1 }, //Goblin Cave
	{ 1, 18 }, //Undead Lair
	{ 18, 18 } //Kobold Lair
};

int CityLocations[3][2] {
	{ 9, 4 }, //Mountain city
	{ 3, 10 }, //Lake city
	{ 11, 15 } //Main city
};

void Menu(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player, Enemy* _enemy[MAX_ENEMIES], Item* _item[MAX_ITEMS], Weapon* _weapons[MAX_WEAPONS], Armor* _armor[MAX_ARMORS]);
void ValidMoveDirection(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player);
void ShowMap(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player);
void InitialSpawns(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Enemy* _enemy[MAX_ENEMIES]);
void InitialItems(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Item* item[MAX_ITEMS]);
//void Update(Enemy* _enemies[MAX_ENEMIES]);
void UpdateMonsters(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player);
void SpawnNewEnemies(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Enemy* _enemy[MAX_ENEMIES], Player* _player);
bool PlaceToShop(int x, int y);
void Shop(Player* _player, Item* _item[MAX_ITEMS], Weapon* _weapons[MAX_WEAPONS], Armor* _armor[MAX_ARMORS]);

string logo[LOGO_LINES];

HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int monstersSlain = 0;

int main()
{
	
	SetConsoleTextAttribute(hConsole, 15);
	ifstream logo_file("logo.txt", ios::in);
	for (int logoline = 0; logoline < LOGO_LINES; logoline++)
	{
		std::getline(logo_file, logo[logoline]);
	}

	/*
	for (int logoline = 0; logoline < 7; logoline++)
	{
		cout<<logo[logoline];
		cout << "\n";
	}
	*/


	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	int spacer = abs(csbi.srWindow.Top - LOGO_LINES);

	for (int h = 0; h < abs(spacer / 2); h++)
		cout << "\n";

	for (int logoline = 0; logoline < LOGO_LINES; logoline++)
	{
		spacer = (logo[logoline].size() - csbi.srWindow.Right);
		spacer = abs(spacer);

		for (int i = 0; i < spacer / 2; i++)
			cout << " ";

		cout << logo[logoline];

		for (int i = 0; i < spacer / 2; i++)
			cout << " ";

		cout << "\n";
	}

	SetConsoleTextAttribute(hConsole, 0);
	system("pause");
	clrscr();
	SetConsoleTextAttribute(hConsole, 15);


	string line_label;
	int num_locations;
	int num_enemies;
	int num_items;
	int num_weapons;
	int num_armors;
	bool read_result;
	Location* new_location;
	Enemy* new_enemy;
	Item* new_item;
	Weapon* new_weapon;
	Armor* new_armor;

	//Declaring world, enemy and item matricies
	Location* world[WORLD_MAX_X][WORLD_MAX_Y];
	Enemy* enemies[MAX_ENEMIES];
	Item* items[MAX_ITEMS];
	Weapon* weapons[MAX_WEAPONS];
	Armor* armors[MAX_ARMORS];

	//Declaring world an enemy matricies pointer to null
	for (int x = 0; x < WORLD_MAX_X; x++)
	{
		for (int y = 0; y < WORLD_MAX_Y; y++)
		{
			world[x][y] = nullptr;
		}
	}

	for (int e = 0; e < MAX_ENEMIES; e++)
	{
		enemies[e] = nullptr;
	}

	for (int i = 0; i < MAX_ITEMS; i++)
	{
		items[i] = nullptr;
	}

	for (int i = 0; i < MAX_WEAPONS; i++)
	{
		weapons[i] = nullptr;
	}

	for (int i = 0; i < MAX_ARMORS; i++)
	{
		armors[i] = nullptr;
	}

	//Reading locations to world matrix
	ifstream location_file("locations.txt", ios::in);
	
	if (location_file.is_open() == false)
	{
		cout << "Could not open locations.txt.\n";
		system("pause");
		return 1;
	}
	location_file >> line_label >> num_locations;
	cout << "Loading " << num_locations << " locations: ";

	for (int i = 0; i < num_locations; i++)
	{
		
		new_location = new Location();
		read_result = new_location->Read(location_file);

		if (read_result == false)
		{
			cout << "Could not read location " << i << ".\n";
			system("pause");
			return 1;
		}
		world[new_location->GetX()][new_location->GetY()] = new_location;
		cout << "|";
	}

	cout << "\nLocations loaded!\n";

	//Reading enemies to enemy matrix
	ifstream enemy_file("enemy.txt", ios::in);

	if (enemy_file.is_open() == false)
	{
		cout << "Could not open enemy.txt.\n";
		system("pause");
		return 1;
	}

	enemy_file >> line_label >> num_enemies;
	cout << "Loading " << num_enemies << " enemies: ";

	for (int i = 0; i < num_enemies; i++)
	{
		
		new_enemy = new Enemy;
		read_result = new_enemy->Read(enemy_file);
		//cout << "Now reading enemy " << i;
		if (read_result == false)
		{
			cout << "Could not read enemy " << i << ".\n";
			system("pause");
			return 1;
		}
		enemies[i] = new_enemy;
		
		cout << "|";
	}
	cout << "\nEnemies loaded!\n";

	//Reading items to item matrix
	ifstream item_file("items.txt", ios::in);

	if (item_file.is_open() == false)
	{
		cout << "Could not open items.txt.\n";
		system("pause");
		return 1;
	}

	item_file >> line_label >> num_items;
	cout << "Loading " << num_items << " items: ";

	for (int i = 0; i < num_items; i++)
	{

		new_item = new Item;
		read_result = new_item->Read(item_file);
		//cout << "Now reading item " << i;
		if (read_result == false)
		{
			cout << "Could not read item " << i << ".\n";
			system("pause");
			return 1;
		}
		items[i] = new_item;

		cout << "|";
	}
	cout << "\n";

	//Read weapons to the weapons matrix!
	ifstream weapon_file("weapons.txt", ios::in);

	if (weapon_file.is_open() == false)
	{
		cout << "Could not open weapons.txt.\n";
		system("pause");
		return 1;
	}

	weapon_file >> line_label >> num_weapons;
	cout << "Loading " << num_weapons << " weapons: ";

	for (int i = 0; i < num_weapons; i++)
	{

		new_weapon = new Weapon;
		read_result = new_weapon->Read(weapon_file);
		//cout << "Now reading item " << i;
		if (read_result == false)
		{
			cout << "Could not read weapon " << i << ".\n";
			system("pause");
			return 1;
		}
		weapons[i] = new_weapon;

		cout << "|";
	}
	cout << "\n";

	//Read Armors to the Armor matrix!
	ifstream armor_file("armor.txt", ios::in);

	if (armor_file.is_open() == false)
	{
		cout << "Could not open armor.txt.\n";
		system("pause");
		return 1;
	}

	armor_file >> line_label >> num_armors;
	cout << "Loading " << num_armors << " armors: ";

	for (int i = 0; i < num_armors; i++)
	{

		new_armor = new Armor;
		read_result = new_armor->Read(armor_file);
		//cout << "Now reading item " << i;
		if (read_result == false)
		{
			cout << "Could not read armor " << i << ".\n";
			system("pause");
			return 1;
		}
		armors[i] = new_armor;

		cout << "|";
	}
	cout << "\n";

	cout << "All data loaded! Press Enter to begin character generation!\n";

	SetConsoleTextAttribute(hConsole, 0);
	system("pause");
	clrscr();
	SetConsoleTextAttribute(hConsole, 15);

	Player* player = new Player();
	player->pickUpItem(weapons[0]);
	player->pickUpItem(weapons[1]);
	player->adjustGold(75);

	//Spawn initial enemies
	//cout << "Spawning initial enemies.\n";
	InitialSpawns(world, enemies);
	InitialItems(world, items);
	player->Intro(hConsole);
	Menu(world, player, enemies, items, weapons, armors);


	system("pause");
	return 0;
}

void ValidMoveDirection(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player)
{
	//cout << "You are currently at X" << _player->GetX() << " Y" << _player->GetY() << "\n";
	cout << "Commands: ";

	if (_world[_player->GetX()][(_player->GetY()-1)] != nullptr)
	{
		cout << "N to move north ";
	}

	if (_world[_player->GetX()][(_player->GetY()+1)] != nullptr)
	{
		cout << "S to move south ";
	}

	if (_world[_player->GetX()+1][_player->GetY()] != nullptr)
	{
		cout << "E to move east ";
	}

	if (_world[_player->GetX()-1][_player->GetY()] != nullptr)
	{
		cout << "W to move west ";
	}
	//cout << "\n";
}

void InitialSpawns(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Enemy* _enemy[MAX_ENEMIES])
{

	for (int p = 0; p < STARTING_ENEMY_COUNT; p++)
	{
		//cout << "Spawning enemy " << p + 1 << "\n";
		//Enemy* spawned_enemy = new Enemy;
		Enemy* spawned_enemy = new Enemy(*_enemy[p]);
		spawned_enemy->SetXY(initialSpawnLocations[p][0], initialSpawnLocations[p][1]);
		_world[spawned_enemy->GetX()][spawned_enemy->GetY()]->AddEnemyToLocation(spawned_enemy);
		activeEnemies.push_back(spawned_enemy);
		//spawnedEnemies++; - Legacy
	}
}


void Menu(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player, Enemy* _enemy[MAX_ENEMIES], Item* _item[MAX_ITEMS], Weapon* _weapons[MAX_WEAPONS], Armor* _armor[MAX_ARMORS])
{
	char move;
	do {
	bool moved = false;

	if (_player->GetHP() <= 0)
	{
		_player->Lose(hConsole, monstersSlain, stepsTaken);
		return;
	}

	/* If not explored yet, mark as explored.
	if (!_world[_player->GetX()][_player->GetY()]->IsExplored())
	{
		_world[_player->GetX()][_player->GetY()]->SetExplored(true);
	}
	*/

	//C-style comment above this, don't delete it! Explored is currently unimplemented due to drawing of walls/visual distance being unimplemented. Come back to this at a later date?

	_world[_player->GetX()][_player->GetY()]->PrintDescription();
	_world[_player->GetX()][_player->GetY()]->GetMonstersAtLocation();
	_world[_player->GetX()][_player->GetY()]->GetItemsAtLocation();
	if (_player->GetX() == CityLocations[2][0] && _player->GetY() == CityLocations[2][1])
	{
		cout << "King greets you. ";
		SetConsoleTextAttribute(hConsole, 2);
		cout << "\n\"Welcome back "<<_player->GetName()<<"! You've watched the city for " << stepsTaken << " turns. We need you to keep watch for another " << STEPS_TO_VICTORY - stepsTaken << ". Stay safe!\"\n";
		SetConsoleTextAttribute(hConsole, 15);
	}
	//cout << "\n";
	_player->takeDamage(0); // Calling TakeDamage with 0 to simply return the player's current/max HP

		ValidMoveDirection(_world, _player);
		cout << "Q to Quit\n";
		cout << "I to examine inventory, U to use an item, D to drop an item, M for to see the map, C to display stats\n";

		if (_world[_player->GetX()][_player->GetY()]->EnemiesPresent() == true)
		{
			cout<<"Enter A to attack ";
		}

		if (_world[_player->GetX()][_player->GetY()]->ItemsPresent() == true)
		{
			cout << "Enter P to pickup an item ";
		}

		if (PlaceToShop(_player->GetX(), _player->GetY()))
		{
			cout << "Enter V to visit the shop\n";
			cout << "You also spend some time at the temple, recovering, and heal " << _player->GetLevel() * 10 << " hit points.\n";
			_player->heal(_player->GetLevel() * 10);
		}

		cin >> move;
		switch (move)
		{
		case 'C':
		case 'c':
			_player->displayStats();
			break;
		case 'V':
		case 'v':
			if (!PlaceToShop(_player->GetX(), _player->GetY()))
			{
				cout << "There is no shop here!\n";
			}
			else
			{
				Shop(_player, _item, _weapons, _armor);
				moved = true;
			}
			break;
		case 'M':
		case 'm':
			cout << "You take out your map.\n";
			ShowMap(_world, _player);
			break;
		case 'D':
		case 'd':
			_player->dropItem(_world);
			break;
		case 'I':
		case 'i':
			_player->seeInventory();
			break;
		case 'U':
		case 'u':
			_player->useItem(_world);
			moved = true;
			break;
		case'P':
		case'p':
			if (_world[_player->GetX()][_player->GetY()]->ItemsPresent() == false)
			{
				cout << "There are no items here!\n";
			}
			else
			{
				_world[_player->GetX()][_player->GetY()]->PassItemToPlayer(_player);
				moved = true;
			}
			break;
		case'A':
		case'a':
			if (_world[_player->GetX()][_player->GetY()]->EnemiesPresent() == false)
			{
				cout << "There is nothing to attack here!\n";
			}
			else
			{
				bool didAttack = _world[_player->GetX()][_player->GetY()]->AttackMonstersAtLoc(_player);
				if (didAttack == true)
				{
					moved = true;
				}

			}
			break;
		case 'N':
		case 'n':
			if (_world[_player->GetX()][(_player->GetY() - 1)] != nullptr)
			{
				cout << "Moving north.\n";
				_player->MoveNorth();
				moved = true;
			}
			else
			{
				cout << "Invalid direction. Try again.\n";
			}
			break;
		case 'S':
		case 's':
			if (_world[_player->GetX()][(_player->GetY() + 1)] != nullptr)
			{
				cout << "Moving south.\n";
				_player->MoveSouth();
				moved = true;

			}
			else
			{
				cout << "Invalid direction. Try again.\n";
			}
			break;
		case 'E':
		case 'e':
			if (_world[_player->GetX() + 1][_player->GetY()] != nullptr)
			{
				cout << "Moving east.\n";
				_player->MoveEast();
				moved = true;
			}
			else
			{
				cout << "Invalid direction. Try again.\n";
			}
			break;
		case 'W':
		case 'w':
			if (_world[_player->GetX() - 1][_player->GetY()] != nullptr)
			{
				cout << "Moving west.\n";
				_player->MoveWest();
				moved = true;
			}
			else
			{
				cout << "Invalid direction. Try again.\n";
			}
			break;
		case 'Q':
		case 'q':
			cout << "Quitting.\n";
			break;
		default:
			cout << "Invalid option. Try again.\n";
			move = ' ';
		}
		if (moved == true)
		{
			SpawnNewEnemies(_world, _enemy, _player);
			UpdateMonsters(_world, _player);
			stepsTaken++;
			if (stepsTaken >= STEPS_TO_VICTORY)
			{
				_player->Victory(hConsole, monstersSlain);
				break;
			}
		}
		
	} while (move != 'Q' && move != 'q');
}

void UpdateMonsters(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player)
{
	/* LEGACY - Does not work in this way due to iterator/vector weirdness.
	std::vector<Enemy*>::iterator itr;
	for (itr = activeEnemies.begin(); itr < activeEnemies.end(); ++itr)
	{
		//Check to see if the monster is dead before doing anything else.
		if ((*itr)->isDead(_player))
		{
			//_world[(*itr)->GetX()][(*itr)->GetY()]->DeleteEnemyFromlocation((*itr));
			itr = activeEnemies.erase(itr);
			activeEnemies.shrink_to_fit();
		}
		else
		{
			(*itr)->MonsterRoam(_world, _player);
		}
	}
	*/

	//Hey, there's a C-style comment above this. Don't remove that 

	//cout << "There are currently " << activeEnemies.size() << " active enemies.\n"; // Debug
	for (unsigned int e = 0; e < activeEnemies.size(); e++)
	{
		if (activeEnemies[e]->isDead(_player))
		{
			_world[activeEnemies[e]->GetX()][activeEnemies[e]->GetY()]->DeleteEnemyFromlocation(activeEnemies[e]);
			//cout << "Deleting enemy " << e << ": " << activeEnemies[e]->GetName() << " because it has " << activeEnemies[e]->GetHP() <<" \n";
			activeEnemies.erase(activeEnemies.begin() + e);
			activeEnemies.shrink_to_fit();
			monstersSlain++;
		}
		else if (_player->GetX() == activeEnemies[e]->GetX() && _player->GetY() == activeEnemies[e]->GetY())
		{
			activeEnemies[e]->AttackPlayer(_player);
		}
		else
		{
			activeEnemies[e]->MonsterRoam(_world, _player);
		}
	}
}

void SpawnNewEnemies(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Enemy* _enemy[MAX_ENEMIES], Player* _player)
{
	int x = 0;
	std::vector<Enemy*>::iterator itr;
	for (itr = activeEnemies.begin(); itr < activeEnemies.end(); ++itr)
	{
		x++;
	}

	if (x < MAX_SPAWNED_ENEMIES)
	{
		std::random_device rd;
		std::default_random_engine gen(rd());
		std::uniform_int_distribution<int> spawnChance(1, MAX_SPAWNED_ENEMIES);
		std::uniform_int_distribution<int> spawnedEnemy(0, MAX_ENEMIES - 1);
		std::uniform_int_distribution<int> spawnLocationRand(0, 3);
		int chance = spawnChance(gen);
		if (chance > x)
		{
			Enemy* new_enemy = new Enemy(*_enemy[spawnedEnemy(gen)], _player);
			//Enemy* new_enemy = new Enemy;
			//new_enemy = _enemy[spawnedEnemy(gen)];
			int spawnLoc = spawnLocationRand(gen);
			new_enemy->SetXY(initialSpawnLocations[spawnLoc][0], initialSpawnLocations[spawnLoc][1]);
			_world[new_enemy->GetX()][new_enemy->GetY()]->AddEnemyToLocation(new_enemy);
			activeEnemies.push_back(new_enemy);
			//cout << "Spawned a " << new_enemy->GetName() << "at " << new_enemy->GetX() << "," << new_enemy->GetY() << "\n";
		}
	}
}


void InitialItems(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Item* item[MAX_ITEMS])
{
	//Planned code:
	//Spawn items equal to the max number of items that can exist in the world
	//When spawning, make sure it's spawning to a valid location
	//If invalid, try again. Otherwise, add the item to the Location's item vector

	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> spawnItem(0, MAX_ITEMS-1);
	std::uniform_int_distribution<int> randLoc(0, 19); //Max world size
	int newItemX = 0;
	int newItemY = 0;

	//Generate items equal to MAX_SPAWNED_ITEMS
	for (int i = 0; i < MAX_SPAWNED_ITEMS; i++)
	{
		//Spawn item to be placed in world
		Item* new_item = new Item(*item[spawnItem(gen)]);
		bool spawnSuccess = false;

		while (spawnSuccess != true)
		{
			//Attempt to spawn to a valid world location
			newItemX = randLoc(gen);
			newItemY = randLoc(gen);
			//cout << "Attempting to spawn item at " << newItemX << "," << newItemY << "\n";
			if (_world[newItemX][newItemY] != nullptr)
			{
				//valid location
				//cout << "Found a valid location!\n";
				_world[newItemX][newItemY]->AddItemToLocation(new_item);
				spawnSuccess = true;
			}
		}

	}

}

void ShowMap(Location* _world[WORLD_MAX_X][WORLD_MAX_Y], Player* _player)
{
	for (int y = 0; y < WORLD_MAX_Y; y++)
	{
		
		SetConsoleTextAttribute(hConsole, 47); //White on deep green

		for (int x = 0; x < WORLD_MAX_X; x++)
		{
			if (_world[x][y] == nullptr)
			{
				SetConsoleTextAttribute(hConsole, 17);
				cout << "#";
			}
			else if (_player->GetX() == x && _player->GetY() == y)
			{
				SetConsoleTextAttribute(hConsole, 46);
				cout << "@";
			}
			else if ((CityLocations[0][0] == x && CityLocations[0][1] == y) || (CityLocations[1][0] == x && CityLocations[1][1] == y) || (CityLocations[2][0] == x && CityLocations[2][1] == y)) //City locations. Ugh. Change these to constants later, if possible.
			{
				SetConsoleTextAttribute(hConsole, 47);
				cout << "C";
			}
			else if ((initialSpawnLocations[0][0] == x && initialSpawnLocations[0][1] == y) || (initialSpawnLocations[1][0] == x && initialSpawnLocations[1][1] == y) || (initialSpawnLocations[2][0] == x && initialSpawnLocations[2][1] == y) || (initialSpawnLocations[3][0] == x && initialSpawnLocations[3][1] == y))
			{
				SetConsoleTextAttribute(hConsole, 44);
				cout << "X";
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 33);
				cout << " ";
			}
			SetConsoleTextAttribute(hConsole, 15);
		}
		cout << "\n";
	}

	//Restore to normal colors
	SetConsoleTextAttribute(hConsole, 15);
}

bool PlaceToShop(int x, int y)
{
	for (int i = 0; i < 3; i++)
	{
		if (CityLocations[i][0] == x && CityLocations[i][1] == y)
		{
			return true;
		}
	}
	return false;
}

void Shop(Player* _player, Item* _item[MAX_ITEMS], Weapon* _weapons[MAX_WEAPONS], Armor* _armor[MAX_ARMORS])
{
	std::vector<Item*> storeInventory;
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> total(5, 10);
	std::uniform_int_distribution<int> itemType(1, 3);
	std::uniform_int_distribution<int> items(0, MAX_ITEMS-1);
	std::uniform_int_distribution<int> armors(0, MAX_ARMORS-1);
	std::uniform_int_distribution<int> weapons(1, MAX_WEAPONS-1);

	int itemsAvaiable = total(gen);
	for (int i = 0; i < itemsAvaiable; i++)
	{
		int currentItem = itemType(gen);
		Item* new_item;
		if (currentItem == 1)
		{
			new_item = new Item(*_item[items(gen)]);
		}
		else if (currentItem == 2)
		{
			new_item = new Weapon(*_weapons[weapons(gen)]);
		}
		else
		{
			new_item = new Armor(*_armor[armors(gen)]);
		}
		storeInventory.push_back(new_item);
	}

	cout << "You visit the shop. You have " << _player->getGold() << " gold pieces.\n";
	cout << "The merchant says \"Welcome to my shop! Please, browse my wares!\"\n";
	cout << "This is what I currently have in stock: \n";

	bool done = false;
	int choice;
	std::vector<Item*>::iterator it;
	do {
		int x = 0;
		for (it = storeInventory.begin(); it < storeInventory.end(); ++it)
		{
			x++;
			if (it != storeInventory.end())
			{
				std::cout << x << ") " << (*it)->GetName() << " - " << (*it)->GetCost() << "gp\n";
			}
			else
			{
				std::cout << "Item not found in vector - error.\n";
			}
		}
		x++;
		cout << x << ") Sell\n";
		x++;
		cout << x <<") Quit\n";

		cin >> choice;
		if (choice == x)
		{
			cout << "You decide not to buy anything.\n";
			return;
		}
		else if (choice > x || choice < 1)
		{
			cout << "Invalid selection. Try again.\n";
		}
		else if (choice == x - 1)
		{
			cout << "What would you like to sell?\n";
			_player->sellItems();
		}
		else
		{
			bool buy = false;
			char decision;
			do {
				storeInventory.at(choice - 1)->ItemDesc();
				cout << "Are you use you want to buy this? Y\\N ";
				cin >> decision;

				if (decision == 'Y' || decision == 'y')
				{
					if (_player->getGold() >= storeInventory.at(choice - 1)->GetCost())
					{
						_player->adjustGold(-storeInventory.at(choice - 1)->GetCost());
						cout << "You purchase " << storeInventory.at(choice - 1)->GetName() << " for " << storeInventory.at(choice - 1)->GetCost() << " gp. You have " << _player->getGold() << " gp left.\n";
						_player->pickUpItem(storeInventory.at(choice - 1));
						storeInventory.erase(storeInventory.begin() + choice - 1);
						storeInventory.shrink_to_fit();
						buy = true;
					}
					else
					{
						cout << "You only have " << _player->getGold() << " gp, but the " << storeInventory.at(choice - 1)->GetName() << " costs " << storeInventory.at(choice - 1)->GetCost() << " gp.\n";
						buy = true;
					}
				}
				else if (decision == 'N' || decision == 'n')
				{
					cout << "You decide not to buy the " << storeInventory.at(choice - 1)->GetName()<<".\n";
					buy = true;
				}
				else
				{
					cout << "Invalid entry. Try again.\n";
				}
			} while (buy == false);
		}
	} while (done == false);
	storeInventory.clear();
	storeInventory.shrink_to_fit();
}