#include "Player.h"
#include "Bullet.h"
#include "..\Base\SceneManager.h"
#include "..\Shared\DataPacket.h"

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
	if (_bulletCounter >= this->getMaxBullet())
	{
		return;
	}

	Vector2 shootPosition = this->getPosition();
	float offset = 0;

	switch (_direction)
	{
	case LEFT:
		shootPosition.x -= offset;
		break;
	case UP:
		shootPosition.y += offset;
		break;
	case RIGHT:
		shootPosition.x += offset;
		break;
	case DOWN:
		shootPosition.y -= offset;
		break;
	default:
		break;
	}

	auto bullet = new Bullet(shootPosition, this->getDirection());
	bullet->setOwner(this);

	SceneManager::getInstance()->getCurrentScene()->addObject(bullet);

	_bulletCounter++;

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
	}
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
			this->updateInput(command->input, command->begin);
		}
		break;
	}
	case INTEGER:
		break;
	default:
		break;
	}
}
