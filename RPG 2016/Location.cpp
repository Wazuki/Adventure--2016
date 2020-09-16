#include <iostream>
#include <fstream>
#include "Location.h"
#include "Player.h"

const char* X_LABEL = "X";
const char* Y_LABEL = "Y";
std::string DESC_LABEL = "description";
Location::Location()
{
	description = "";
	x = 0;
	y = 0;
	explored = false;
}

Location::~Location()
{
}

bool Location::Read(std::ifstream &_file)
{
	std::string label;
	_file >> label >> x;
	//std::cout << "X is " << x << "\n";
	if (label.compare(X_LABEL) != 0)
	{
		std::cout << "Label \'"<<X_LABEL<<"\' expected\n";
		return false;
	}

	_file >> label >> y;
	//std::cout << "Y is " << y << "\n";
	if (label.compare(Y_LABEL) != 0)
	{
		std::cout << "Label \'" << Y_LABEL<<"\' expected\n";
		return false;
	}

	_file >> label;
	_file.ignore(1);
	std::getline(_file, description);

	if (label.compare(DESC_LABEL) != 0)
	{
		std::cout << "Label \'" << DESC_LABEL << "\' expected\n";
		return false;
	}
	
	return true;
}

int Location::GetX()
{
	return x;
}

int Location::GetY()
{
	return y;
}

void Location::PrintDescription()
{
	std::cout<<description;
}

void Location::AddEnemyToLocation(Enemy* _enemy)
{
	//std::cout << "Adding enemy to vector\n";
	enemiesAtLoc.push_back(_enemy);
	//std::cout << "Enemy added to vector!\n";
	//std::cout << "There are now a "<<_enemy->GetName()<<"here, and a total of " << enemiesAtLoc.size() << " enemies here!";
}

void Location::DeleteEnemyFromlocation(Enemy* _enemy)
{
	std::vector<Enemy*>::iterator it;
	it = find(enemiesAtLoc.begin(), enemiesAtLoc.end(), _enemy);
	if (it != enemiesAtLoc.end())
	{
		//std::cout << "Enemy found - deleting enemy from vector.\n";
		enemiesAtLoc.erase(it);
		enemiesAtLoc.shrink_to_fit();
	}
	else
	{
		std::cout<<"Enemy not found in vector - error.\n";
	}
	
}

void Location::GetMonstersAtLocation()
{
	std::cout << "\n";
	std::vector<Enemy*>::iterator it;
	for (it = enemiesAtLoc.begin(); it < enemiesAtLoc.end(); ++it)
	{
		if (it != enemiesAtLoc.end())
		{
			//std::cout << "Enemy found - deleting enemy from vector.\n";
			std::cout << "You see a " << (*it)->GetName() << "\n";
		}
		else
		{
			std::cout << "Enemy not found in vector - error.\n";
		}
	}
}

bool Location::EnemiesPresent()
{
	if (enemiesAtLoc.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Location::AttackMonstersAtLoc(Player* _player)
{
	int ct = 0;
	unsigned int choice = 0;
	do {
	std::cout << "Select an enemy to attack: \n";
	ct = 0;
	std::vector<Enemy*>::iterator it;
	for (it = enemiesAtLoc.begin(); it < enemiesAtLoc.end(); ++it)
	{
		if (it != enemiesAtLoc.end())
		{
			ct++;
			std::cout <<ct<<") " << (*it)->GetName() << "\n";
		}
		else
		{
			std::cout << "Enemy not found in vector - error.\n";
		}
	}
	ct++;
	std::cout << ct << ") Back\n";
		choice = 0;
		bool didPlayerAttack = false;
		std::cin >> choice;
		if (choice == ct)
		{
			break;
		}
		else if (choice > enemiesAtLoc.size() || choice <= 0)
		{
			std::cout << "Invalid option. Try again.";
		}
		else
		{
			std::cout << "Attacking " << enemiesAtLoc.at(choice-1)->GetName()<<"\n";
			didPlayerAttack = _player->attack(enemiesAtLoc.at(choice-1));
		}
		if (didPlayerAttack == true)
		{
			return true;
		}
	} while (choice != ct);
	return false;
}

//Item-related functions

void Location::GetItemsAtLocation()
{
	std::cout << "\n";
	std::vector<Item*>::iterator it;
	for (it = itemsAtLoc.begin(); it < itemsAtLoc.end(); ++it)
	{
		if (it != itemsAtLoc.end())
		{
			//std::cout << "Item found - deleting enemy from vector.\n";
			std::cout << "You see a " << (*it)->GetName() << "\n";
		}
		else
		{
			std::cout << "Item not found in vector - error.\n";
		}
	}
}

void Location::AddItemToLocation(Item* _item)
{
	itemsAtLoc.push_back(_item);
}

void Location::DeleteItemFromLocation(Item* _item)
{
	std::vector<Item*>::iterator it;
	it = find(itemsAtLoc.begin(), itemsAtLoc.end(), _item);
	if (it != itemsAtLoc.end())
	{
		//std::cout << "Enemy found - deleting enemy from vector.\n";
		itemsAtLoc.erase(it);
		itemsAtLoc.shrink_to_fit();
	}
	else
	{
		std::cout << "Enemy not found in vector - error.\n";
	}
}

bool Location::ItemsPresent()
{
	if (itemsAtLoc.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Location::PassItemToPlayer(Player* _player)
{
	int ct = 0;
	unsigned int choice = 0;
	do {
		std::cout << "Select an item to pick up.\n";
		ct = 0;
		std::vector<Item*>::iterator it;
		for (it = itemsAtLoc.begin(); it < itemsAtLoc.end(); ++it)
		{
			if (it != itemsAtLoc.end())
			{
				ct++;
				std::cout << ct << ") " << (*it)->GetName() << "\n";
			}
			else
			{
				std::cout << "item not found in vector - error.\n";
			}
		}
		ct++;
		std::cout << ct << ") Back\n";
		choice = 0;
		std::cin >> choice;
		if (choice == ct)
		{
			break;
		}
		else if (choice > itemsAtLoc.size() || choice <= 0)
		{
			std::cout << "Invalid option. Try again.";
		}
		else
		{
			std::cout << "You picked up a " << itemsAtLoc.at(choice - 1)->GetName();
			_player->pickUpItem(itemsAtLoc.at(choice - 1));
			DeleteItemFromLocation(itemsAtLoc.at(choice - 1));
			break;
		}

	} while (choice != ct);
}

void Location::SetExplored(bool _explored)
{
	explored = _explored;
}

bool Location::IsExplored()
{
	return explored;
}