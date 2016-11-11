#include "Player.h"
#include "Bullet.h"
#include "..\Game.h"

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

void Player::shoot()
{
	auto bullet = new Bullet(this->getPosition(), this->getDirection());
	Game::instance->addObject(bullet);

	//printf("shoot: %.2f, %.2f\n", bullet->getPosition().x, bullet->getPosition().y);
}

void Player::updateInput(eKeyInput input, bool start)
{
	if (start)
	{
		switch (input)
		{
		case KEY_LEFT:
			this->setDirection(eDirection::LEFT);
			this->addStatus(eStatus::RUNNING);
			break;
		case KEY_RIGHT:
			this->setDirection(eDirection::RIGHT);
			this->addStatus(eStatus::RUNNING);
			break;
		case KEY_UP:
			this->setDirection(eDirection::UP);
			this->addStatus(eStatus::RUNNING);
			break;
		case KEY_DOWN:
			this->setDirection(eDirection::DOWN);
			this->addStatus(eStatus::RUNNING);
			break;
		case KEY_SHOOT:
			this->shoot();
			break;
		default:
			break;
		}
	}
	else
	{
		this->removeStatus(eStatus::RUNNING);
		this->setVelocity(0.0f);
	}
}