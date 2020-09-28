#include "Configuration.h"
#include <rapidjson.h>
#include <filereadstream.h>
#include <cstdio>
#include <filesystem>
#include <iostream>

std::string Configuration::server_dx_client_hash;
std::string Configuration::server_gl_client_hash;
std::string Configuration::server_client_version;

Configuration::Configuration()
{
	std::filesystem::path cwd = std::filesystem::current_path();
#ifdef _WIN32
	cwd += "\\ServAuth_config.json";
#else
	cwd += "/ServAuth_config.json";
#endif
	pathToConfig = cwd.string();
	std::cout << "\nFile : " << pathToConfig << "\n";
}

Configuration::~Configuration()
{
}

bool Configuration::load()
{
	return reload();
}

bool Configuration::reload()
{
	FILE* fp;
#ifdef _WIN32
	errno_t status = fopen_s(&fp, pathToConfig.c_str(), "rb");
#else
	errno_t status = fopen_s(&fp, pathToConfig.c_str(), "r");
#endif
	if (status == 0 && fp != NULL)
	{
		char buffer[4096];
		rapidjson::FileReadStream reader(fp, buffer, sizeof(buffer));
		rapidjson::Document d;
		d.ParseStream(reader);
		if (fp)
			fclose(fp);
		if (!parseConfig(d))
		{
			std::cerr << "Configuration : error (config file has wrong format)\n";
			return false;
		}
	}
	else
	{
		char buffer[100];
		strerror_s(buffer, status);
		std::cerr << "Configuration : error n" << status << " (" << buffer << ")\n";
		return false;
	}
	return true;
}

bool Configuration::isConfigValid(const rapidjson::Document& document)
{
	if (!document.IsObject())
		return false;

	if (!document.HasMember("server") || !document["server"].IsObject())
		return false;
	{
		const rapidjson::Value& server = document["server"];
		if (!server.HasMember("port") || !server["port"].IsInt())
			return false;
		if (!server.HasMember("dx_client_hash") || !server["dx_client_hash"].IsString())
			return false;
		if (!server.HasMember("gl_client_hash") || !server["gl_client_hash"].IsString())
			return false;
		if (!server.HasMember("client_version") || !server["client_version"].IsString())
			return false;
	}

	if (!document.HasMember("database") || !document["database"].IsObject())
		return false;
	{
		const rapidjson::Value& database = document["database"];
		if (!database.HasMember("port") || !database["port"].IsInt())
			return false;
		if (!database.HasMember("host") || !database["host"].IsString())
			return false;
		if (!database.HasMember("username") || !database["username"].IsString())
			return false;
		if (!database.HasMember("password") || !database["password"].IsString())
			return false;
		if (!database.HasMember("database") || !database["database"].IsString())
			return false;
	}

	if (!document.HasMember("worlds") || !document["worlds"].IsArray())
		return false;
	{
	}

	return true;
}

bool Configuration::parseConfig(const rapidjson::Document& document)
{
	if (!isConfigValid(document))
		return false;

	{
		server_port = document["server"]["port"].GetInt();
		server_dx_client_hash = document["server"]["dx_client_hash"].GetString();
		server_gl_client_hash = document["server"]["gl_client_hash"].GetString();
		server_client_version = document["server"]["client_version"].GetString();
	}

	{
		database_port = document["database"]["port"].GetInt();
		database_host = document["database"]["host"].GetString();
		database_username = document["database"]["username"].GetString();
		database_password = document["database"]["password"].GetString();
		database_database = document["database"]["database"].GetString();
	}

	{
		if (!parseServers(document))
			return false;
	}

	return true;
}

bool Configuration::parseServers(const rapidjson::Document& document)
{
	Server::clearServers();

	const rapidjson::Value& worlds = document["worlds"];
	if (worlds.Empty())
		return false;

	rapidjson::Value::ConstMemberIterator itr;
	for (rapidjson::SizeType i = 0; i < worlds.Size(); i++)
	{
		const rapidjson::Value& world = worlds[i];

		itr = world.FindMember("name");
		if (itr == world.MemberEnd())
			return false;
		std::string name = itr->value.GetString();

		itr = world.FindMember("server_id");
		if (itr == world.MemberEnd())
			return false;
		int server_id = itr->value.GetInt();

		itr = world.FindMember("channels");
		if (itr == world.MemberEnd())
			return false;
		std::vector<World> chans;

		const rapidjson::Value& channels = itr->value;
		for (rapidjson::Value::ConstValueIterator itrChan = channels.Begin(); itrChan != channels.End(); ++itrChan)
		{
			const rapidjson::Value& attribute = *itrChan;

			itr = attribute.FindMember("channel_id");
			if (itr == attribute.MemberEnd())
				return false;
			int channel_id = itr->value.GetInt();

			itr = attribute.FindMember("ip");
			if (itr == attribute.MemberEnd())
				return false;
			std::string ip = itr->value.GetString();

			itr = attribute.FindMember("port");
			if (itr == attribute.MemberEnd())
				return false;
			unsigned short port = itr->value.GetInt();

			itr = attribute.FindMember("max_population");
			if (itr == attribute.MemberEnd())
				return false;
			int max_population = itr->value.GetInt();

			chans.push_back(World(channel_id, ip, port, max_population));
		}
		Server::addServer(Server(name, server_id, chans));
	}
	return true;
}
