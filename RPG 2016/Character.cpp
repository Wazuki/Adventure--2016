#include <iostream>
#include <random>
#include "Character.h"

Character::Character()
{
	name = "", dmg = "";
	hp = 0, hd = 0, att = 0, dmgBonus = 0;
	x = 0, y = 0;
}

Character::~Character()
{

}

int Character::GetX()
{
	return x;
}

int Character::GetY()
{
	return y;
}

std::string Character::GetName()
{
	return name;
}

int Character::AttackDamage(int _min, int _max)
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dmg(_min, _max);
	int temp = dmg(gen);
	return temp;
}

void Character::SetX(int _x)
{
	x = _x;
}

void Character::SetY(int _y)
{
	y = _y;
}

void Character::SetXY(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Character::MoveNorth()
{
	y -= 1;
}

void Character::MoveSouth()
{
	y += 1;
}

void Character::MoveEast()
{
	x += 1;
}

void Character::MoveWest()
{
	x -= 1;
}

int Character::GetAC()
{
	return ac;
}