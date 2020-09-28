#pragma once

#include "Lang.h"
#include "Database/Tables/ay_accounts.h"

class Client
{
public:
	Client(const std::string& loginPacket, const std::string& Ip, bool IsInMaintenance);
	~Client();
	std::string getResponse();
	void setInMaintenance(bool IsInMaintenance);
	std::string getUsername() const;

protected:

private:
	int checkLoginPacket();

	int checkCode();
	int checkGfuid() const;
	int checkLang() const;
	int checkVersion() const;
	int checkNostaleHash() const;

	bool saveAccountDataInDatabase();

	bool isInMaintenance;

	std::string ip;

	std::string code;
	std::string gfuid;
	LangType lang;
	std::string version;
	std::string nostaleHash;

	int encryptionKey;

	std::string clientDxHash;
	std::string clientGlHash;

	ay_accounts accountInfo;
};
