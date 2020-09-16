#pragma once
#include <string>
class Character
{
public:
	Character();
	~Character();
	int GetX();
	int GetY();
	void SetXY(int _x, int _y);
	void SetX(int _x); // Legacy?
	void SetY(int _y); // Legacy?
	void MoveNorth();
	void MoveSouth();
	void MoveEast();
	void MoveWest();
	std::string GetName();
	int AttackDamage(int _min, int _max);
	virtual int GetAC();
protected:
	std::string name, dmg;
	int hd, hp, ac, att, dmgBonus, maxhp;
	int x, y;
};

