#pragma once

#include <mysql.h>
#include <vector>
#include <string>

class DatabaseManager
{
public:
	DatabaseManager(std::string Host, std::string Username, std::string Password, std::string Database, unsigned short Port, bool Connect = true);
	~DatabaseManager();

	static std::vector<std::vector<std::string>> query(const std::string& query, int maxNbIteration);
	static std::vector<std::string> query(const std::string& query);

	bool connect();
	bool disconnect();

private:

	static MYSQL *connection;
	std::string host;
	std::string username;
	std::string password; 
	std::string database;
	unsigned short port;
};
