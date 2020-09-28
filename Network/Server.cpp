#include "Server.h"

std::vector<Server> Server::servers;

Server::Server(std::string Name, int ServerId, std::vector<World> Channels)
	: name(Name)
	, serverId(ServerId)
	, channels(Channels)
{

}

Server::~Server()
{

}

std::string Server::getName() const
{
	return name;
}

int Server::getId() const
{
	return serverId;
}

std::vector<World> Server::getChannels() const
{
	return channels;
}

void Server::addServer(Server server)
{
	servers.push_back(server);
}

std::vector<Server> Server::getServers()
{
	return servers;
}

void Server::clearServers()
{
	servers.clear();
}
