#include "Server.h"

int main(int argc, char **argv)
{
	Server* server = new Server(PORT, LOCALHOST);
	server->init();

	printf("Server running...\n");

	server->run();
	server->destroy();

	delete server;

	return 0;
}