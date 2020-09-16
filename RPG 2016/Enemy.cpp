#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "Enemy.h"
#include "Location.h"
#include "Player.h"

const int XP_MODIFIER = 2;

std::string NAME_LABEL = "name";
std::string HD_LABEL = "hd";
std::string HP_LABEL = "hp";
std::string AC_LABEL = "ac";
std::string ATT_LABEL = "att";
std::string DMG_LABEL = "dmg";
std::string DMGBONUS_LABEL = "dmgBonus";
std::string ROAMING_LABEL = "roaming";

Enemy::Enemy() :Character()
{
	roamChance = 10;
}

Enemy::Enemy(const Enemy &_enemy, Player* _player)
{
	//std::cout << "Copy constructor running!\n";
	int diffAdjust = (int)floor((_player->GetLevel() / 3));
	name = _enemy.name;
	dmg = _enemy.dmg;
	hd = _enemy.hd + diffAdjust;
	hp = _enemy.hp * hd;
	ac = _enemy.ac + diffAdjust;
	att = _enemy.att + diffAdjust;
	dmgBonus = _enemy.dmgBonus + diffAdjust;
	x = _enemy.x;
	y = _enemy.y;
	roaming = _enemy.roaming;
	roamChance = _enemy.roamChance;
}

Enemy& Enemy::operator=(const Enemy& _enemy)
{
	std::cout << "Overloaded assignment operator called!\n";
	if (this == &_enemy)
	{
		name = _enemy.name;
		dmg = _enemy.dmg;
		hd = _enemy.hd;
		hp = _enemy.hp;
		ac = _enemy.ac;
		att = _enemy.att;
		dmgBonus = _enemy.dmgBonus;
		x = _enemy.x;
		y = _enemy.y;
		roaming = _enemy.roaming;
		roamChance = _enemy.roamChance;
	}
	return *this;
}


Enemy::~Enemy()
{
}
bool Enemy::Read(std::ifstream &_file)
{
	std::string label;
	std::string temp;
	_file >> label;
	_file.ignore(1);
	std::getline(_file, temp);
	name = temp;

	if (label.compare(NAME_LABEL) != 0)
	{
		std::cout << "Label \'" << NAME_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> hd;
	if (label.compare(HD_LABEL) != 0)
	{
		std::cout << "Label \'" << HD_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> hp;

	if (label.compare(HP_LABEL) != 0)
	{
		std::cout << "Label \'" << HP_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> ac;

	if (label.compare(AC_LABEL) != 0)
	{
		std::cout << "Label \'" << AC_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> att;

	if (label.compare(ATT_LABEL) != 0)
	{
		std::cout << "Label \'" << ATT_LABEL << "\' expected\n";
		return false;
	}

	_file >> label;
	_file.ignore(1);
	std::getline(_file, dmg);

	if (label.compare(DMG_LABEL) != 0)
	{
		std::cout << "Label \'" << DMG_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> dmgBonus;

	if (label.compare(DMGBONUS_LABEL) != 0)
	{
		std::cout << "Label \'" << DMGBONUS_LABEL << "\' expected\n";
		return false;
	}

	_file >> label >> roaming;

	if (label.compare(ROAMING_LABEL) != 0)
	{
		std::cout << "Label \'" << ROAMING_LABEL << "\' expected\n";
		return false;
	}

	return true;
}

void Enemy::MonsterRoam(Location* _world[][20], Player* _player)
{
	
	bool roamed = false;
	int dir = 0;
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> chanceToRoam(1, 100);
	do {
		if (chanceToRoam(gen) <= roamChance)
		{
			do{
				std::uniform_int_distribution<int> roamDir(1, 4);
				dir = roamDir(gen);
				switch (dir){
				case 1:
					if (_world[GetX()][(GetY() - 1)] != nullptr)
					{
						//cout << " north";
						_world[GetX()][GetY()]->DeleteEnemyFromlocation(this);
						MoveNorth();
						_world[GetX()][GetY()]->AddEnemyToLocation(this);
						roamed = true;


					}
					break;

				case 2:
					if (_world[GetX()][(GetY() + 1)] != nullptr)
					{
						//cout << " south";
						_world[GetX()][GetY()]->DeleteEnemyFromlocation(this);
						MoveSouth();
						_world[GetX()][GetY()]->AddEnemyToLocation(this);
						roamed = true;
					}
					break;

				case 3:
					if (_world[GetX() + 1][GetY()] != nullptr)
					{
						//cout << " east";
						_world[GetX()][GetY()]->DeleteEnemyFromlocation(this);
						MoveEast();
						_world[GetX()][GetY()]->AddEnemyToLocation(this);
						roamed = true;
					}
					break;

				case 4:
					if (_world[GetX() - 1][GetY()] != nullptr)
					{
						//cout << " west";
						_world[GetX()][GetY()]->DeleteEnemyFromlocation(this);
						MoveWest();
						_world[GetX()][GetY()]->AddEnemyToLocation(this);
						roamed = true;
					}
					break;

				default:
					std::cout << "Something went wrong with the monster movement script!\n";
				}

			} while (roamed == false);
			//std::cout << name << " is now at " << x << "," << y << "\n";
		}
		else
		{
			roamChance += 10;
			roamed = true;
			//std::cout << name<< " did not roam.\n";
		}
	} while(roamed == false);
}

void Enemy::TakeDamage(Player* _player, int damageTaken)
{
	hp -= damageTaken;
	//std::cout << name << " has " << hp << " hp left.\n";
}

bool Enemy::isDead(Player* _player)
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> gold(hd*2, hd*4);
	if (hp <= 0)
	{
		std::cout << "You have slain the " << name << "!\n";
		_player->gainXP(hd * XP_MODIFIER); 
		int gainedGold = gold(gen);
		std::cout << "Found " << gainedGold << " gold coins!\n";
		_player->adjustGold(gainedGold);
		//std::cout << "Enemy::IsDead is declaring " << name << " dead. Delete from vector\n";
		return true;
	}
	else
	{
		return false;
	}
}

int Enemy::GetHP()
{
	return hp;
}

void Enemy::AttackPlayer(Player* _player)
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> d20(1, 20);
	std::cout << "The " << name << " attacks you and ";
	int attackRoll = d20(gen) + att;
	int minDmg = std::stoi(dmg.substr(0, dmg.find("d")));
	int maxDmg = std::stoi(dmg.substr((dmg.find("d") + 1)));
	int dmgToPlayer = AttackDamage(minDmg, maxDmg) + dmgBonus;
	if (attackRoll == 20)
	{
		dmgToPlayer = maxDmg + dmgBonus;
		std::cout << "scores a CRITICAL HIT dealing "<<dmgToPlayer<<" damage! ";
		_player->takeDamage(dmgToPlayer);
	}
	else if (attackRoll >= _player->GetAC())
	{
		//If the damage to teh player is less than 1, make it 1
		if (dmgToPlayer <= 0) dmgToPlayer = 1;

		std::cout << " hits, dealing " << dmgToPlayer << " damage! ";
		_player->takeDamage(dmgToPlayer);
	}
	else
	{
		std::cout << "misses you!\n";
	}

}