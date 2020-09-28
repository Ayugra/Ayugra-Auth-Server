#pragma once

#include <string>
#include "Lang.h"
#include "Network/Server.h"
#include <iostream>

class S_NsTeST
{
public:
	S_NsTeST(LangType Lang, std::string Username, int EncryptionKey, std::vector<Server> Servers)
		: lang(Lang)
		, username(Username)
		, encryptionKey(EncryptionKey)
		, servers(Servers)
	{

	}

	~S_NsTeST()
	{

	}

	const std::string getPacket() const
	{
		std::string packet = HEADER;
		packet += "  " + std::to_string(static_cast<int>(lang));
		packet += " " + username;
		packet += " 2";
		packet += " " + std::to_string(encryptionKey);

		std::vector<Server> serv = Server::getServers();
		for (int i = 0; i < serv.size(); i++)
		{
			/*std::cout << "Name : " << serv[i].getName() << "\n"
				<< "Id   : " << serv[i].getId() << "\n";*/
			std::vector<World> worlds = serv[i].getChannels();
			for (int j = 0; j < worlds.size(); j++)
			{
				/*std::cout << "\nChannel " << worlds[j].getId() << worlds[j].isAvailable() << "\n"
					<< "\tIp : " << worlds[j].getIp() << "\n"
					<< "\tPort : " << worlds[j].getPort() << "\n"
					<< "\tMaxPopulation : " << worlds[j].getMaxPopulation();*/
				if (worlds[j].getId() > 0 && worlds[j].isAvailable())
				{
					packet += " " + worlds[j].getIp();
					packet += ":" + std::to_string(worlds[j].getPort());
					packet += ":" + std::to_string(worlds[j].getPopularity());
					packet += ":" + std::to_string(serv[i].getId());
					packet += "." + std::to_string(worlds[j].getId());
					packet += "." + serv[i].getName();
				}
			}
		}

		packet += " -1:-1:-1:10000.10000.1"; // Entwell stupidness.

		std::cout << packet;
		return packet;
	}

private:
	LangType lang;
	std::string username;
	int encryptionKey;
	std::vector<Server> servers;

	const std::string HEADER = "NsTeST";
};