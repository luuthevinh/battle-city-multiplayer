#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Player.h"

class Game
{
public:
	Game();
	~Game();

	bool init();
	void update(float dt);
	void update(char* data);

	std::vector<Player*> getAllPlayers();

	void addPlayer(Player* player);
	void removePlayer(int index);
	Player* getPlayer(int index);

private:
	std::vector<Player*> _players;
};

#endif // !__GAME_H__
