#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::init()
{
	return true;
}

void Game::update(float dt)
{
	for(auto player : _players)
	{
		player->update(dt);
	}
}

void Game::update(char * data)
{
}

std::vector<Player*> Game::getAllPlayers()
{
	return _players;
}


void Game::addPlayer(Player* player)
{
	_players.push_back(player);
}

void Game::removePlayer(int index)
{
	_players.erase(_players.begin() + index);
}

Player * Game::getPlayer(int index)
{
	return _players[index];
}
