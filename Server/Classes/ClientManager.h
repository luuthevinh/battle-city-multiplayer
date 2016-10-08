#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include <vector>
#include <WinSock2.h>

class ClientManager
{
public:
	ClientManager();
	~ClientManager();

	void addClient(SOCKET socket);
	SOCKET getClientSocket(int id);
	void removeClient(int id);

	const std::vector<SOCKET> &getAllClients();

	bool isSendNewUpdate(int id);
	void setUpdate(int id, bool val);

private:
	std::vector<SOCKET> _clientSockets;

	std::vector<bool> _updatedClients;
};


#endif // !__CLIENTMANAGER_H__
