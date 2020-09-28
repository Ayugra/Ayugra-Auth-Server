#pragma once

#include "Lang.h"
#include "Utils.h"

class ay_accounts
{
public:
	ay_accounts()
		: valid(false)
		, id(-1)
		, username("")
		, password("")
		, email("")
		, code("")
		, verified(false)
		, banned(true)
		, authority(0)
		, lang(LangType::EN)
	{

	}

	~ay_accounts()
	{

	}

	void populate(std::vector<std::string> datas)
	{
		valid = true;
		if (datas.size() != NUMBER_COLUMN)
		{
			valid = false;
			return;
		}
		id = ToNumber<int>(datas[0].c_str());
		username = datas[1];
		password = datas[2];
		email = datas[3];
		code = datas[4];
		verified = ToNumber<int>(datas[5].c_str());
		banned = ToNumber<int>(datas[6].c_str());
		authority = ToNumber<int>(datas[7].c_str());
		lang = static_cast<LangType>(ToNumber<int>(datas[8].c_str()));

		valid = (id > 0) &&
				(!username.empty()) &&
				(!password.empty()) &&
				(!email.empty()) &&
				(!code.empty());
	}

	int getId() const { return valid ? id : -1; }
	std::string getUsername() const { return valid ? username : ""; }
	std::string getPassword() const { return valid ? password : ""; }
	std::string getEmail() const { return valid ? email : ""; }
	std::string getCode() const { return valid ? code : ""; }
	bool isVerified() const { return valid ? verified : false; }
	bool isBanned() const { return valid ? banned : true; }
	int getAuthority() const { return valid ? authority : 0; }
	LangType getLang() const { return lang; }

	static const size_t NUMBER_COLUMN = 10;

private:
	bool valid;
	int id;
	std::string username;
	std::string password;
	std::string email;
	std::string code;
	bool verified;
	bool banned;
	int authority;
	LangType lang;
};