#pragma once

#include <document.h>
#include "Network/Server.h"

/*{
	"server":
	{
		"port": (unsigned short) port used by the login server
		"dx_client_hash": (std::string) NostaleClientX.exe md5 hash
		"gl_client_hach": (std::string) NostaleClient.exe md5 hash
		"client_version": (std::string) NostaleClientX.exe version
	},
	"database":
	{
		"port": (unsigned short) port used for MySQL
		"host": (std::string) host used to connect to the database
		"username": (std::string) username used to connect to the database
		"password": (std::string) password used to connect to the database
		"database": (std::string) database to connect to
	}
	"worlds": [
		{
			"name": (std::string) server name
			"id": (int) server id
			"channels": [
				{ "numero": (int) channel (if -1 : act4), "ip": (std::string) ip to connect, "port": (unsigned short) port to connect, "max_population": (int) maximum players on this channel }
			]
		}
	]
}*/

class Configuration
{
public:
	Configuration();
	~Configuration();
	bool load();
	bool reload();

	unsigned short d_server_port() const	{ return server_port;		}
	static std::string d_dx_client_hash()	{ return server_dx_client_hash;	}
	static std::string d_gl_client_hash()	{ return server_gl_client_hash;	}
	static std::string d_client_version()	{ return server_client_version;	}

	unsigned short d_database_port() const	{ return database_port;		}
	std::string d_database_host() const		{ return database_host;		}
	std::string d_database_username() const	{ return database_username;	}
	std::string d_database_password() const	{ return database_password; }
	std::string d_database_database() const	{ return database_database; }

private:
	bool isConfigValid(const rapidjson::Document& document);
	bool parseConfig(const rapidjson::Document& document);
	bool parseServers(const rapidjson::Document& document);

	std::string pathToConfig;
	
	unsigned short server_port;
	static std::string server_dx_client_hash;
	static std::string server_gl_client_hash;
	static std::string server_client_version;

	unsigned short database_port;
	std::string database_host;
	std::string database_username;
	std::string database_password;
	std::string database_database;
};