#pragma once

#include <string>

class World
{
public:
	World(int ChannelId, std::string Ip, unsigned short Port, int MaxPopulation);
	~World();
	void setAvailable(bool status);
	void setPopularity(int value);

	int getId() const;
	std::string getIp() const;
	unsigned short getPort() const;
	int getMaxPopulation() const;
	bool isAvailable() const;
	int getPopularity() const;

	//bool isShown();
	// For Act4 idea to be connectable through serv selection

private:
	int channelId;
	std::string ip;
	unsigned short port;
	int maxPopulation;
	bool available;
	int popularity;
};
