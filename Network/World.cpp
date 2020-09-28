#include "World.h"

World::World(int ChannelId, std::string Ip, unsigned short Port, int MaxPopulation)
	: channelId(ChannelId)
	, ip(Ip)
	, port(Port)
	, maxPopulation(MaxPopulation)
	, available(true)
	, popularity(0)
{

}

World::~World()
{

}

void World::setAvailable(bool status)
{
	available = status;
}

void World::setPopularity(int value)
{
	popularity = value;
}

int World::getId() const
{
	return channelId;
}

std::string World::getIp() const
{
	return ip;
}

unsigned short World::getPort() const
{
	return port;
}

int World::getMaxPopulation() const
{
	return maxPopulation;
}

bool World::isAvailable() const
{
	return available;
}

int World::getPopularity() const
{
	return popularity;
}
