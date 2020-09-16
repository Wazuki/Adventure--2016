#pragma once
#include "Character.h"
class Location;
class Player;

class Enemy : public Character
{
public:
	Enemy();
	Enemy(const Enemy &_enemy, Player* _player);
	Enemy& Enemy::operator=(const Enemy& _enemy);
	~Enemy();
	void TakeDamage(Player* _player, int dmgTaken);
	void MonsterRoam(Location* _world[][20], Player* _player);
	bool Read(std::ifstream &_file);
	bool isDead(Player* _player);
	void AttackPlayer(Player* _player);
	int GetHP();
private:
	bool roaming;
	int roamChance;
};

