#include "Network/Network.h"
#include "Database/DatabaseManager.h"
#include "Network/Configuration.h"
#include <iostream>

void startServerConnection()
{
	asio::io_context io_context;
	Network server(io_context, 4002);
	server.startServer();
	io_context.run();
}

int main()
{
	std::cout << "Ayugra Emulator Project\n\n";

	Configuration config;
	while (!config.load())
		Sleep(2000);
	std::cout << "Configuration successfully read\n\n";

	DatabaseManager db(config.d_database_host(), config.d_database_username(), config.d_database_password(),
		config.d_database_database(), config.d_database_port());

	std::thread networkThread(&startServerConnection);

	while (true)
		Sleep(2000);

	return 0;
}