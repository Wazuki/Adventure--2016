#include <iostream>
#include <random>
#include <string>
#include <limits>
#include <Windows.h>
#include "Player.h"
#include "Location.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Armor.h"
#include "Console.h"
using std::cout;
using std::endl;
using std::cin;
using std::string;

const int PLAYER_START_X = 11;
const int PLAYER_START_Y = 15;



Player::Player() 
{
	/* Legacy
	weaponTypes["unarmed strike"] = "1d3";
	weaponTypes["dagger"] = "1d4";
	weaponTypes["handaxe"] = "1d6";
	weaponTypes["longsword"] = "1d8";

	armorTypes["leather"] = 2;
	armorTypes["studded leather"] = 3;
	armorTypes["chain shirt"] = 4;
	armorTypes["breastplate"] = 5;
	*/
	//C-Style comment above

	str = 0, dex = 0, mind = 0;
	level = 1;
	ac = 10, hp = 0, xp = 0, xpTNL = 10;
	dmgBonus = 0;
	physical = 1, subterfuge = 1, knowledge = 1, communication = 1;
	generateStats();
	chooseRace();
	chooseClass();
	maxhp = str + 6 + 10; //10 Bonus HP assigned for being solo!
	hp = maxhp; // Start with max hp
	x = PLAYER_START_X;
	y = PLAYER_START_Y;
	cout << "Please name your hero: ";
	//cin.ignore();
	getline(cin, playerName); // This suddenly flags an error, WEEKS later. No idea why.

	equippedArmor = nullptr;
	/* Legacy
	for (int w = 0; w < MAX_WEAPONS; w++)
	{
		weapons[w] = "none";
	}
	weapons[0] = "dagger";
	*/

}

Player::~Player()
{
}

void Player::generateStats()
{
	cout << "Welcome to ADVENTURE!, loosely based on Microlite20!\n";
	
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> d6(1, 6);
	int x, stat, stats[3];
	int lowest = 0;
	char reroll = 'y';
	do {
		for (int y = 0; y < 3; y++)
		{
			x = 0;
			lowest = 0;
			stat = 0;
			for (int i = 0; i < 4; i++)
			{
				//cout << "Rolling die " << i + 1 << endl;
				do {
					x = d6(gen);
				} while (x == 1);
				//cout << "Rolled a " << x << endl;
				if (i == 0)
				{
					lowest = x;
					//cout << "Defaulting this roll to lowest.\n";
				}
				else if (x > lowest)
				{
					stat += x;
					//cout << "Greater than lowest - adding to stat.\n";

				}
				else
				{
					//cout << "New lowest.\n";
					stat += lowest;
					lowest = x;
				}
			}
			//cout << "Total roll: " << stat << endl;
			stats[y] = stat;
		}
		cout << "Your rolls are: " << stats[0] << ", " << stats[1] << ", and " << stats[2] << "\n";
		do {
			cout << "Would you like to roll again? Y\\N: ";
			cin >> reroll;
		} while (reroll != 'Y' && reroll != 'y' && reroll != 'n' && reroll != 'N');
	} while (reroll == 'Y' || reroll == 'y');
	cout << "\n";
	int assigned = 0;
	int choice = 0;

	for (int y = 0; y < 3; y++)
	{
		do {
			cout << "Assign your stats! Where would you like to assign the " << stats[y] << "?\n";
			cout << "1) Strength \n2) Dexterity \n3) Mind \n";
			cin >> choice;

			switch (choice)
			{
			case 1:
				if (str == 0)
				{
					cout << "You have assigned the " << stats[y] << " to Strength.\n";
					str = stats[y];
					assigned++;
				}
				else
				{
					cout << "You have already assigned a value to Strength. Please try again.\n";
				}
				break;
			case 2:
				if (dex == 0)
				{
					cout << "You have assigned the " << stats[y] << " to Dexterity.\n";
					dex = stats[y];
					assigned++;
				}
				else
				{
					cout << "You have already assigned a value to Dexterity. Please try again.\n";
				}
				break;
			case 3:
				if (mind == 0)
				{
					cout << "You have assigned the " << stats[y] << " to Mind.\n";
					mind = stats[y];
					assigned++;
				}
				else
				{
					cout << "You have already assigned a value to Mind. Please try again.\n";
				}
				break;
			default:
				cout << "Invalid entry.\n";
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
				choice = 0;
			}

		} while (assigned <= y);
	}
}

void Player::chooseRace()
{
	int choice;
	char raceChoice = 'N';
	do {
		cout << "Choose your Race! You may pick Human, Elf, Dwarf, or Halfling.\n"; // Add more later? Magic implementation, etc.
		cout << "1) Human \n2) Elf \n3) Dwarf \n4) Halfling \n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "Humans get a +1 bonus to all skill roles.\n";
			cout << "Do you want to choose this race? Y\\N: ";
			cin >> raceChoice;
			playerRace = "human";
			break;
		case 2:
			cout << "Elves get a +2 bonus to Mind.\n";
			cout << "Do you want to choose this race Y\\N: ";
			cin >> raceChoice;
			playerRace = "elf";
			break;
		case 3:
			cout << "Dwarves get a +2 bonus to Str.\n";
			cout << "Do you want to choose this race Y\\N: ";
			cin >> raceChoice;
			playerRace = "dwarf";
			break;
		case 4:
			cout << "Halflings get a +2 bonus to Dex.\n";
			cout << "Do you want to choose this race Y\\N: ";
			cin >> raceChoice;
			playerRace = "halfling";
			break;
		default:
			cout << "You have entered an invalid choice. Please try again.\n";

		}
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
	} while (raceChoice != 'Y' && raceChoice != 'y');

	if (playerRace == "human")
	{
		physical++;
		subterfuge++;
		knowledge++;
		communication++;
	}
	else if (playerRace == "elf")
	{
		mind += 2;
	}
	else if (playerRace == "dwarf")
	{
		str += 2;
	}
	else if (playerRace == "halfling")
	{
		dex += 2;
	}
}

void Player::chooseClass()
{
	int choice;
	char classChoice = 'N';
	do {
		cout << "Choose your class! You may pick from Fighter or Rogue.\n"; // Add more later? Magic implementation, etc.
		cout << "1) Fighter \n2) Rogue \n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "Fighters can wear any kind of armor and can use shields. They have a +3 bonus to the Physical skill and add +1 to all attack and damage rolls. This bonus increases by +1 at 5th level and every 5 levels thereafter.\n";
			cout << "Do you want to choose this class? Y\\N: ";
			cin >> classChoice;
			playerClass = "fighter";
			break;
		case 2:
			cout << "Rogues can wear light. They have a +3 bonus to the Subterfuge skill. They can try to Sneak up on a foe with the Subterfuge skill, adding the Subterfuge skill rank to the damage of their first attack.\n";
			cout << "Do you want to choose this class? Y\\N: ";
			cin >> classChoice;
			playerClass = "rogue";
			break;
		default:
			cout << "You have entered an invalid choice. Please try again.\n";
		}
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
	} while (classChoice != 'Y' && classChoice != 'y');

	if (playerClass == "fighter")
	{
		physical += 3;
		att += 1;
		dmgBonus += 1;
	}
	else if (playerClass == "rogue")
	{
		subterfuge += 3;
	}
}

int Player::statMod(int stat)
{
	int mod = (int)floor((((double)stat - 10) / 2));
	return mod;
}

std::string Player::statModAdjust(int stat)
{
	std::string temp = "";
	if (statMod(stat) > -1)
	{
		temp = "+";
	}
	
	temp += std::to_string(statMod(stat));
	//cout << "Stat mod concat: " << temp << ". Stat was "<<stat<<" and mod was "<<statMod(stat)<<"\n";
	return temp;
}

void Player::displayStats()
{
	cout << "You are a level "<<level<<" " << playerRace<<" " << playerClass << "\n";
	cout << "Stats:\n";
	cout << "Strength: " << str << "(" << statModAdjust(str) << ")\n";
	cout << "Dexterity: " << dex << "(" << statModAdjust(dex) << ")\n";
	cout << "Mind: " << mind << "(" << statModAdjust(mind) << ")\n";
	cout << "\n";
	cout << "Skills:\n";
	cout << "Physical: " << (physical + level) << " ranks, " << (physical + level + statMod(str)) << " bonus\n";
	cout << "Subterfuge: " << (subterfuge + level) << " ranks, " << (subterfuge + level + statMod(dex)) << " bonus\n";
	cout << "Knowledge: " << (knowledge + level) << " ranks, " << (knowledge + level + statMod(mind)) << " bonus\n";
	cout << "Communication: " << (communication + level) << " ranks, " << (communication + level + statMod(mind)) << " bonus\n";
	cout << "\n" << gold << " gold pieces\n";

	if (equippedArmor != nullptr)
	{
		cout << "You are wearing " << equippedArmor->GetName() << ". It gives an AC bonus of +" << equippedArmor->GetArmorBonus() << ".\n";
	}
	else
	{
		cout << "You are not wearing any armor.\n";
	}

}

int Player::weaponDamage(int _minDmg, int _maxDmg)
{
	//cout << "You are wielding a " << weapons[0] << " which does " << weaponTypes[weapons[0]] << " damage.\n";
	//cout << "Testing attack function: ";
	//string currentWeapon = weaponTypes[weapons[weap]];
	//int minDmg = std::stoi(currentWeapon.substr(0, currentWeapon.find("d")));
	//int maxDmg = std::stoi(currentWeapon.substr((currentWeapon.find("d")+1)));

	int dmgDealt = AttackDamage(_minDmg, _maxDmg);

	return dmgDealt;
}

bool Player::attack(Enemy* _enemy)
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> d20(1, 20);
	int validWeapons = 0;
	int weaponChoice = 0;
	int ct = 0;
	std::cout << "Select your weapon: \n";

	/* Old weapon function via arrays. Defunct with Weapon class.
	for (int w = 0; w < MAX_WEAPONS; w++)
	{
		if (weapons[w] != "none")
		{
			cout << w + 1 << ") " << weapons[w] << "\n";
			validWeapons++;
		}
	}
	*/
	//C-style comment above this

	ct = 0;
	std::vector<Item*>::iterator it;
	for (it = inventory.begin(); it < inventory.end(); ++it)
	{
		//Get weapon name
		string temp = (*it)->GetType();
		if (it != inventory.end())
		{
			if (!temp.compare("weapon"))
			{
				ct++;
				std::cout << ct << ") " << (*it)->GetName() << "\n";
				weaponSelector[ct] = it - inventory.begin();
				//cout << "weaponSelector[" << ct << "] is assigned " << weaponSelector[ct] << "\n";
			}
		}
		else
		{
			std::cout << "item not found in vector - error.\n";
		}
	}

	ct++;
	std::cout << ct << ") Back\n";

	do {
		cin >> weaponChoice;
		if (weaponChoice == ct)
		{
			//cout << "Back\n";
			return false;
			break;
		}
		else if (weaponChoice > ct || ct <= 0)
		{
			cout << "Invalid choice, try again.\n";
		}
		else
		{
			cout << "Attacking " << _enemy->GetName() << " with " << inventory.at(weaponSelector[weaponChoice])->GetName() << "\n";
			int damageDealt = 0;

			//See if player is a rogue and, if so, attack to SNEAK ATTACK!

			if (playerClass == "rogue")
			{
				int sneakAttack = d20(gen) + subterfuge + statMod(dex);
				if (sneakAttack >= _enemy->GetAC())
				{
					//cout << "SNEAK ATTACK! ";
					damageDealt += subterfuge;
				}
			}
			// Add Dex or Str to attak roll, whicheve is greater
			int attackRoll = d20(gen) + att;
			if (statMod(dex) > statMod(str))
			{
				attackRoll += statMod(dex);
			}
			else
			{
				attackRoll += statMod(str);
			}

			if (attackRoll == 20)
			{
				//This could be better. Super redudnant by repeatedly finding the max damage in the map but it works.
				//string currentWeapon = weaponTypes[weapons[weaponChoice - 1]];
				//damageDealt += (std::stoi(currentWeapon.substr((currentWeapon.find("d") + 1)))) + dmgBonus + statMod(str);
				damageDealt += inventory.at(weaponSelector[weaponChoice])->GetMaxDmg() + dmgBonus + statMod(str);

				//If two handed, add Strength mod again
				if (inventory.at(weaponSelector[weaponChoice])->GetTwoHanded())
				{
					damageDealt += statMod(str);
				}

				cout << "You score a CRITICAL HIT on the " << _enemy->GetName() << ", dealing " << damageDealt << " points of damage!\n";
				_enemy->TakeDamage(this, damageDealt);
			}
			else if (attackRoll >= _enemy->GetAC())
			{
				damageDealt += weaponDamage(inventory.at(weaponSelector[weaponChoice])->GetMinDmg(), inventory.at(weaponSelector[weaponChoice])->GetMaxDmg()) + statMod(str) + dmgBonus;

				//If two handed, add Strength mod again
				if (inventory.at(weaponSelector[weaponChoice])->GetTwoHanded())
				{
					damageDealt += statMod(str);
				}

				cout << "You hit the " << _enemy->GetName() << " for " << damageDealt << " points of damage!\n";
				_enemy->TakeDamage(this, damageDealt);
			}
			else
			{
				cout << "You missed the " << _enemy->GetName()<<"\'s AC of " << _enemy->GetAC() << " with a " << attackRoll << "\n";
			}
			return true;
		}

	} while (weaponChoice != ct);
	return false;
}

void Player::gainXP(int gainedXP)
{
	xp += gainedXP;
	cout << "You gained " << gainedXP << " experience!\n";
	if (xp >= xpTNL)
	{
		levelUp();
	}
	else
	{
		cout<<"You need " << xpTNL - xp << " more experience points to level up!\n";
	}
}

void Player::levelUp()
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> levelHP(3, 6);
	level++;
	cout << "You have gained a level! You are now level " << level << "!\n";
	int bonusHP = levelHP(gen);
	cout << "You have gained " << bonusHP << "HP, +1 to attack rolls and +1 to all skills!\n";
	att++;
	physical++;
	subterfuge++;
	knowledge++;
	communication++;
	maxhp += bonusHP;
	hp += bonusHP;
	if ((level % 3) == 0)
	{
		int pickStat = 0;
		bool assigned = false;
		do{
			cout << "You have gained an attribute point! Where would you like to assign it?\n";
			cout << "1) Str: "<<str<<"\n2) Dex: "<<dex<<"\n3)Mind: "<<mind<<"\n";
			cin >> pickStat;
			switch (pickStat){
			case 1:
				str++;
				cout << "You have chosen Str! Your Str is now " << str << "\n";
				assigned = true;
				break;
			case 2:
				dex++;
				cout << "You have chosen Dex Your Dex is now " << dex << "\n";
				assigned = true;
				break;
			case 3:
				mind++;
				cout << "You have chosen Mind! Your Mind is now " << mind << "\n";
				assigned = true;
				break;
			default:
				cout << "Invalid entry. Try again.\n";
				assigned = false;
			}
		} while (assigned != true);
	}

	if (playerClass == "fighter" && (level % 5) == 0)
	{
		cout << "You are a Fighter and gain an additional +1 to hit and damage!\n";
		att++;
		dmgBonus++;
	}

	//reset XP
	xp = xp - xpTNL;
	xpTNL = level * 10;
	cout<<"You have " << xp << " expereince and need an additional " << xpTNL - xp << " to level again.\n";

}

void Player::takeDamage(int _dmgTaken)
{
	hp -= _dmgTaken;
	cout << "HP: " << hp << "/" << maxhp << "\n";
}

int Player::GetAC()
{
	if (equippedArmor != nullptr)
	{
		ac = 10 + statMod(dex) + equippedArmor->GetArmorBonus();
	}
	else
	{
		ac = 10 + statMod(dex);
	}
	return ac;
}

void Player::pickUpItem(Item* _item)
{
	inventory.push_back(_item);
}

void Player::seeInventory()
{
	int ct = 0;
	if (inventory.empty())
	{
		cout << "Your backpack is empty!\n";
		return;
	}
	std::cout << "You open your backpack and see:\n";
	ct = 0;
	std::vector<Item*>::iterator it;
	for (it = inventory.begin(); it < inventory.end(); ++it)
	{
		if (it != inventory.end())
		{
			ct++;
			std::cout << ct << ") " << (*it)->GetName() << "\n";
		}
		else
		{
			std::cout << "item not found in vector - error.\n";
		}
	}
}

void Player::sellItems()
{
	int x;
	int choice;
	do {
		cout << "\n";
		seeInventory();
		x = inventory.size() + 1;
		cout << x << ") Back\n";
		cout << "\n";

		cin >> choice;

		if (choice > x || choice < 1)
		{
			cout << "Invalid entry. Try again.\n";
		}
		else if (choice == x)
		{
			cout << "You decide not to sell anything.\n\n";
			return;
		}
		else
		{
			if (choice == 1) // This is a VERY poor patchwork for fixing fist-related sales. 
			{
				cout << "You can't sell your fists!\n";
				
			}
			else
			{
				char sell;
				cout << "You can sell the " << inventory.at(choice - 1)->GetName() << " for " << inventory.at(choice - 1)->GetCost() / 2 << " gold pieces. Is that okay? Y\\N ";
				cin >> sell;
				if (sell == 'Y' || sell == 'y')
				{
					cout << "You can sell the " << inventory.at(choice - 1)->GetName() << " for " << inventory.at(choice - 1)->GetCost() / 2 << " gold pieces.\n";
					adjustGold(inventory.at(choice - 1)->GetCost() / 2);
					inventory.erase(inventory.begin() + choice - 1);
					inventory.shrink_to_fit();
				}
			}
		}
	} while (choice != x);
}

void Player::dropItem(Location* _world[][20])
{
	int ct = 0;
	unsigned int choice = 0;
	do {
		if (inventory.empty())
		{
			cout << "Your backpack is empty!\n";
			return;
		}
		std::cout << "You open your backpack and see:\n";
		ct = 0;
		std::vector<Item*>::iterator it;
		for (it = inventory.begin(); it < inventory.end(); ++it)
		{
			if (it != inventory.end())
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
		std::cout << "Enter a number to drop that item.\n";
		choice = 0;
		std::cin >> choice;

		if (choice == ct)
		{
			break;
		}
		else if (choice > inventory.size() || choice <= 0)
		{
			std::cout << "Invalid option. Try again.";
		}
		else
		{

			std::cout << "You have chosen " << choice - 1 << ", a " << inventory.at(choice - 1)->GetName() << ".\n";

			string itemName = inventory.at(choice - 1)->GetName();
			if (!itemName.compare("Unarmed"))
			{
				std::cout << "Despite your best efforts, your fists remain firmly attached you your wrists. You are unable to drop them.\n";
				break;
			}
			//cout << "You used a " << inventory.at(choice - 1)->GetName();
			_world[x][y]->AddItemToLocation(inventory.at(choice - 1));
			inventory.erase(inventory.begin() + choice - 1);
			inventory.shrink_to_fit();
			break;
		}
	} while (choice != ct);
}

void Player::useItem(Location* _world[][20])
{
	int ct = 0;
	unsigned int choice = 0;
	do {
		if (inventory.empty())
		{
			cout << "Your backpack is empty!\n";
			break;
		}
		std::cout << "You open your backpack and see:\n";
		ct = 0;
		std::vector<Item*>::iterator it;
		for (it = inventory.begin(); it < inventory.end(); ++it)
		{
			if (it != inventory.end())
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
		else if (choice > inventory.size() || choice <= 0)
		{
			std::cout << "Invalid option. Try again.";
		}
		else
		{

			std::cout << "You have chosen " << choice - 1 << ", a " << inventory.at(choice - 1)->GetName() << ".\n";

			string itemType = inventory.at(choice - 1)->GetType();
			if (!itemType.compare("weapon"))
			{
				inventory.at(choice - 1)->UseWeapon();
				break;
			}
			if (!itemType.compare("armor"))
			{
				//inventory.at(choice - 1)->EquipArmor();
				EquipArmor((Armor*)inventory.at(choice - 1), choice-1);
				break;
			}
			//cout << "You used a " << inventory.at(choice - 1)->GetName();
			bool itemDestroyed = inventory.at(choice - 1)->UseItem(this);
			if (itemDestroyed)
			{
				inventory.erase(inventory.begin() + choice - 1);
				inventory.shrink_to_fit();
			}
			break;
		}
	} while (choice != ct);
}

void Player::heal(int _healAmount)
{
	hp += _healAmount;
	if (hp > maxhp)
	{
		hp = maxhp;
	}
}

void Player::adjustGold(int _gold)
{
	gold += _gold;
}

int Player::getGold()
{
	return gold;
}

Armor* Player::getEquippedArmor()
{
	return equippedArmor;
}

void Player::EquipArmor(Armor* _armor, int _invLoc)
{
	//cout << "Player Class is " << playerClass << " and armor type is " << _armor->GetArmorType() << "\n";
	if (!playerClass.compare("fighter") || (!playerClass.compare("rogue") && !_armor->GetArmorType().compare("light")))
	{
		if (equippedArmor == nullptr)
		{
			equippedArmor = _armor;
			cout << "You are now wearing " << equippedArmor->GetName() << ". It gives an AC bonus of +" << equippedArmor->GetArmorBonus() << ".\n";
			inventory.erase(inventory.begin() + _invLoc);
			inventory.shrink_to_fit();
		}
		else
		{
			cout << "You are curently wearing " << equippedArmor->GetName() << ". It gives an AC bonus of +" << equippedArmor->GetArmorBonus() << ".\n";
			cout << "Do you want to replace it with " << _armor->GetName() << ", " << _armor->GetArmorType() << " armor which provides an AC bonus of +" << _armor->GetArmorBonus() << "? ";
			bool done;
			char choice;
			do {
				cin >> choice;
				switch (choice)
				{
				case 'Y':
				case 'y':
					inventory.erase(inventory.begin() + _invLoc);
					inventory.shrink_to_fit();
					pickUpItem(equippedArmor);
					equippedArmor = _armor;
					cout << "You are now wearing " << equippedArmor->GetName() << ". It gives an AC bonus of +" << equippedArmor->GetArmorBonus() << ".\n";
					done = true;
					break;
				case 'N':
				case 'n':
					cout << "You decide not to change your armor.\n";
					done = true;
					break;
				default:
					cout << "Invalid entry. Try again.\n";
					done = false;
				}
			} while (done != true);
		}
	}
	else
	{
		cout << "You can't wear that armor!\n";
	}
}

int Player::GetLevel()
{
	return  level;
}

std::string Player::GetName()
{
	return name;
}

void Player::Intro(HANDLE _handle)
{
	cout << "The small island of Demonreach has recently put out a call for mercenaries to\n";
	cout << "help combat a growing monster problem. Demand for " << playerClass << "s is high and you are\n";
	cout << "nearly broke, making it a perfect opportunity for you to make some easy gold.\n";
	cout << "\n\n";

	cout << "Unfortunately, some thugs beat you up right after you landed and took most\n";
	cout << "of your weapons and armor.Fortunately, they didn't find your secret stash\n";
	cout << "of gold or your back-up dagger.\n";

	cout << "Making your way from the dock to the fortress of Caer Benowyc, you are met by\n";
	cout << "The guard captain, a dwarf named \'King\'. \n\n";
	SetConsoleTextAttribute(_handle, 2);
	cout << "\"Welcome to Caer Benowyc, " << playerName << ".\nIt just so happens we need more " << playerClass << "s. \nThe";
	cout << "job is simple: protect the city for some time,\n killing as many monsters as you ";
	cout << "can.\nTake this map, and try not to die. I'll update you on your\n";
	cout << "progress when you stop by. Good luck, " << playerName << ".\"\n\n";
	SetConsoleTextAttribute(_handle, 15);
}

void Player::Victory(HANDLE _handle, int _monstersSlain)
{
	cout << "You return to Caer Benowyc only to be greeted by King again - this time\n";
	cout << "with dozens of soldiers. ";
	SetConsoleTextAttribute(_handle, 2);
	cout << "\"You're just in time " << name << "! Our reinforcements have finally arrived!\n";
	cout << "Thank you for all your help. You've done the city a great service.\"\n\n";
	SetConsoleTextAttribute(_handle, 15);
	cout << "You are given a large bag of gold for your troubles. You have gained 1000 gold!\n";
	adjustGold(1000);
	cout << "Congratulations! You have completed the adventure and saved Demonreach!\n";
	cout << "\n\nSort of.\n\n";
	cout << "You achieved level " << level << " and slayed " << _monstersSlain << "!\n";
	displayStats();
	cout << "\n\nThank you for playing!\n\n";
}

int Player::GetHP()
{
	return hp;
}

void Player::Lose(HANDLE _handle, int _monstersSlain, int _steps)
{
	cout << "You have been slain!\n";
	cout << "After battling for "<<_steps<<" turns, you have finally succumbed to your wounds.\n";
	cout << "Now who will save Demonreach?\n\n";
	cout << "Honestly, the island was probably doomed anyway.\\nn";
	displayStats();
	cout << "\n\nThank you for playing!\n\n";
}