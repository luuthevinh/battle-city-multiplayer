#include "Player.h"

Player::Player(eObjectId id, int index) : Tank(id),
	_index(index)
{
}

Player::~Player()
{
}

int Player::getIndex()
{
	return _index;
}