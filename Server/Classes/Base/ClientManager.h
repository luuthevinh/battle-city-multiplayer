#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include <vector>
#include <WinSock2.h>
#include <map>
#include <queue>
#include <deque>

#include "..\GameObject\Player.h"
#include "..\Definitions.h"
#include "..\Game.h"

class ClientManager
{
public:
	ClientManager();
	~ClientManager();

	int addClient(SOCKET socket);
	SOCKET getClientSocket(int id);

	int getSocketId(SOCKET socket);

	void removeClient(int id);
	void removeClient(SOCKET socket);

	const std::vector<SOCKET> &getAllClients();

	std::vector<Player*> getAllPlayers();
	Player* getPlayerByUniqueId(int id);
	Player* getPlayerBySocket(SOCKET socket);
	
	bool hasChanged();
	void onChanged();
	void setChanged(bool value);

	static int playerId;
	static int getNextPlayerId();

private:
	bool _roomHasChanged;
	std::vector<SOCKET> _clientSockets;

	std::map<SOCKET, Player*> _infoPlayers;
	
};


#endif // !__CLIENTMANAGER_H__
