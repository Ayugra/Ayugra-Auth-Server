#include "Client.h"
#include "Packets/R_NoS0577.h"
#include "Packets/S_failc.h"
#include "Packets/S_NsTeST.h"
#include "Database/DatabaseManager.h"
#include "Cryptography/Cryptography.h"
#include "Configuration.h"
#include <iostream>
#include <random>
#include <ctime>

Client::Client(const std::string& loginPacket, const std::string& Ip, bool IsInMaintenance)
	: isInMaintenance(IsInMaintenance)
	, ip(Ip)
	, code("")
	, gfuid("")
	, lang(LangType::EN)
	, encryptionKey(-1)
	, version("")
	, nostaleHash("")
	, clientDxHash("")
	, clientGlHash("")
{
	R_NoS0577 p(loginPacket);
	code = hexToAscii(p.getCode());
	gfuid = p.getGfuid();
	lang = p.getLang();
	version = p.getVersion();
	nostaleHash = p.getNostaleHash();

	std::cout << "\nCode : " << code << "\n";
	std::cout << "Gfuid : " << gfuid << "\n";
	std::cout << "Lang : " << (int)lang << "\n";
	std::cout << "Version : " << version << "\n";
	std::cout << "Loginhash : " << nostaleHash << "\n";
}

Client::~Client()
{

}

std::string Client::getResponse()
{
	int check = checkLoginPacket();
	if (check != -1)
		return S_failc(check).getPacket();
	std::random_device randomDevice;
	std::mt19937 encryptionKeyGenerator(randomDevice());
	std::uniform_int_distribution<int> distribution(3000, 65536);
	encryptionKey = distribution(encryptionKeyGenerator);
	encryptionKey += encryptionKey % 2;
	saveAccountDataInDatabase();
	return S_NsTeST(lang, accountInfo.getUsername(), encryptionKey, Server::getServers()).getPacket();
}

void Client::setInMaintenance(bool IsInMaintenance)
{
	isInMaintenance = IsInMaintenance;
}

std::string Client::getUsername() const
{
	return accountInfo.getUsername();
}

int Client::checkLoginPacket()
{
	if (isInMaintenance)
		return 3;

	int cVersion = checkVersion();
	if (cVersion != -1)
		return 1;

	int cNostaleHash = checkNostaleHash();
	if (cNostaleHash != -1)
		return cNostaleHash;

	int cCode = checkCode();
	if (cCode != -1)
		return cCode;

	int cGfuid = checkGfuid();
	if (cGfuid != -1)
		return cGfuid;

	int cLang = checkLang();
	if (cLang != -1)
		return cLang;

	return -1;
}

/*
4023 1 ton client est obsol�e.#13#10Merci de r�ssayer apr� la maintenance ou de t��harger le client actuel sur notre site.#13#10
4024 2 une erreur s'est produite lors de la connexion. Merci de r�ssayer.#13#10
4025 3 le serveur est en cours de maintenance.#13#10Pour conna�re les p�iodes de maintenance, consulte le site de NosTale.#13#10
4026 4 le nom du compte est d��utilis�#13#10
4027 5 le nom du compte ou le mot de passe sont incorrects.#13#10La saisie r��� de donn�s incorrectes entra�era un blocage temporaire de ton compte.#13#10
4028 6 le client ne peut pas se connecter.#13#10
4029 7 ton compte a ��bloqu�#13#10Merci de contacter notre �uipe NosTale.
4030 8 tu ne peux pas te connecter �ce serveur depuis ton pays.#13#10
*/

int Client::checkCode()
{
	if (!isUuidValid(code))
		return 2;
	accountInfo.populate(DatabaseManager::query("SELECT * FROM ay_accounts WHERE code = '" + code + "'"));
	if (accountInfo.getCode() != code)
		return 6;
	if (accountInfo.isBanned())
		return 7;
	if (!accountInfo.isVerified())
		return 6;
	std::vector<std::string> res = DatabaseManager::query("SELECT connected FROM ay_accounts_status WHERE account_id = '"
		+ std::to_string(accountInfo.getId()) + "'");
	if (res.size() == 0)
		return 2;
	if (res[0] == "1")
		return 4;
	return -1;
}

int Client::checkGfuid() const
{
	if (!isUuidValid(gfuid))
		return 2;
	if (!DatabaseManager::query("SELECT * FROM ay_gfuid_banned WHERE gfuid ='" + gfuid + "'").empty())
		return 6;
	return -1;
}

int Client::checkLang() const
{
	if (lang != accountInfo.getLang())
		return 8;
	return -1;
}

int Client::checkVersion() const
{
	if (version != Configuration::d_client_version())
		return 1;
	return -1;
}

int Client::checkNostaleHash() const
{
	if (nostaleHash.size() != 32)
		return 2;
	if (!(nostaleHash.find_first_not_of(HEXADECIMAL_UPPER_ONLY) == std::string::npos))
		return 2;
	if (Cryptography::HashMd5(Configuration::d_dx_client_hash() + Configuration::d_gl_client_hash(), false) != nostaleHash)
		return 1;
	return -1;
}

bool Client::saveAccountDataInDatabase()
{
	if (encryptionKey == -1)
		return false;
		DatabaseManager::query("UPDATE ay_accounts SET encryption_key = '" + std::to_string(encryptionKey)
			+ "' WHERE account_id = " + std::to_string(accountInfo.getId()));
	time_t t = time(0);
	struct tm timeStruct;
	errno_t error = localtime_s(&timeStruct, &t);
	if (error)
		return false;
	std::string date = std::to_string(timeStruct.tm_year + 1900) + "/" + std::to_string(timeStruct.tm_mon + 1) + "/" + std::to_string(timeStruct.tm_mday);
	std::cout << date << ip << gfuid;
	if (!ip.empty() && !gfuid.empty() && !date.empty())
	{
		DatabaseManager::query("UPDATE ay_accounts_logs SET date_last_connection = '" + date +
			"', ip_last_connection = '" + ip + "', gfuid_last_connection = '" + gfuid + "' WHERE `account_id` = '"
			+ std::to_string(accountInfo.getId()) + "'");
		return true;
	}
	return false;
}
