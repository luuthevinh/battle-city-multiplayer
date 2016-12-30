#include "Player.h"
#include "Bullet.h"
#include "..\Base\SceneManager.h"
#include "..\Shared\DataPacket.h"

#include "..\Server.h"

Player::Player(eObjectId id, int index) : Tank(id),
	_index(index),
	_isHost(false)
{
	_life = 3;
}

Player::~Player()
{
}

void Player::update(float dt)
{
	Tank::update(dt);

	if (_protectedTimer > 0)
	{
		_protectedTimer -= dt;
	}
	else if (this->hasStatus(eStatus::PROTECTED))
	{
		_protectedTimer = 0.0f;
		this->removeStatus(eStatus::PROTECTED);
	}
}

int Player::getIndex() const
{
	return _index;
}

void Player::handleData(Serializable * data)
{
	auto type = data->getType();

	switch (type)
	{
	case OBJECT:
	{
		Tank::handleData(data);
		break;
	}
	case PACKET:
		break;
	case REPLY_ID:
		break;
	case COMMAND:
	{
		if (auto command = dynamic_cast<CommandPacket*>(data))
		{
			_commandQueue.push(command);
			//printf("push command(%d)\n", _commandQueue.size());
		}
		break;
	}
	case INTEGER:
		break;
	default:
		break;
	}
}

void Player::shoot()
{
	Tank::shoot();

}

void Player::move(eDirection direction, float dt)
{
	this->setDirection(direction);

	this->addStatus(eStatus::RUNNING);
	_velocity = this->getVelocityByLevel();

	// fix position for turning
	this->fixPositionForTurn();

	this->moveByDistance(_velocity * dt);
}

void Player::gotHit(Damage * damage)
{
	if (damage->getObjectId() == this->getId() || this->hasStatus(eStatus::PROTECTED))
	{
		// cùng màu ko bắn nhau
		return;
	}

	_health -= damage->getValue();

	if (_health <= 0)
	{
		_life--;

		this->setStatus(eStatus::DIE);
		this->onChanged();
	}

	delete damage;
}

void Player::revive()
{
	if (_life <= 0)
		return;

	this->setStatus((eStatus)(STAND | PROTECTED));
	_protectedTimer = 3.0f;
}

int Player::getLife()
{
	return _life;
}

void Player::setHost(bool value)
{
	_isHost = value;
}

bool Player::isHost() const
{
	return _isHost;
}
