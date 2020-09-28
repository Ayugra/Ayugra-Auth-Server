#include "DatabaseManager.h"
#include <iostream>

MYSQL* DatabaseManager::connection;

DatabaseManager::DatabaseManager(std::string Host, std::string Username, std::string Password, std::string Database, unsigned short Port, bool Connect)
	: host(Host)
	, username(Username)
	, password(Password)
	, database(Database)
	, port(Port)
{
	if (Connect)
		connect();
}

DatabaseManager::~DatabaseManager()
{
	std::cout << "DatabaseManager::~DatabaseManager();";
}

// Is there a race condition in this function when it is used by 2 threads ?
std::vector<std::vector<std::string>> DatabaseManager::query(const std::string& query, int maxNbIteration)
{
	if (!connection)
		return {};
	if (mysql_query(connection, query.c_str()))
		return {};
	MYSQL_RES* res = mysql_store_result(connection);
	if (!res)
		return {};
	std::vector<std::vector<std::string>> output;
	MYSQL_ROW row;
	int it = 0;
	while ((row = mysql_fetch_row(res)) && (it < maxNbIteration || maxNbIteration == -1))
	{
		std::vector<std::string> tmp;
		for (size_t i = 0; i < res->field_count; i++)
			tmp.push_back(row[i]); // Populate by columns
		output.push_back(tmp); // Populate by rows
		it++;
	}
	return output;
}

std::vector<std::string> DatabaseManager::query(const std::string& query)
{
	if (!connection)
		return {};
	if (mysql_query(connection, query.c_str()))
		return {};
	MYSQL_RES* res = mysql_store_result(connection);
	if (!res)
		return {};
	std::vector<std::string> output;
	MYSQL_ROW row;
	if (row = mysql_fetch_row(res))
	{
		for (size_t i = 0; i < res->field_count; i++)
			output.push_back(row[i]);
	}
	return output;
}

bool DatabaseManager::connect()
{
	connection = mysql_init(0);
	connection = mysql_real_connect(connection, host.c_str(), username.c_str(), password.c_str(), database.c_str(), port, NULL, 0);

	if (!connection)
	{
		std::cerr << "Error : Failed to connect to database\n";
	}
	return connection;
}

bool DatabaseManager::disconnect()
{
	return false;
}