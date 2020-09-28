#pragma once

#include <vector>
#include "World.h"

class Server
{
public:
	Server(std::string Name, int ServerId, std::vector<World> Channels);
	~Server();

	std::string getName() const;
	int getId() const;
	std::vector<World> getChannels() const;

	static void addServer(Server server);
	static std::vector<Server> getServers();
	static void clearServers();

private:
	std::string name;
	int serverId;
	std::vector<World> channels;

	static std::vector<Server> servers;
};
