#include "Server.h"

int main(int argc, char **argv)
{
	Server* server = new Server(1495, "127.0.0.1");
	server->init();

	printf("Server running...\n");

	server->run();
	server->destroy();

	delete server;

	return 0;
}