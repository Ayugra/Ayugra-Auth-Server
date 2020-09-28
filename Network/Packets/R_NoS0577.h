#pragma once

#include "Utils.h"
#include "Lang.h"

class R_NoS0577
{
public:
	R_NoS0577(std::string Packet)
		: code("")
		, gfuid("")
		, randomPart("")
		, lang(LangType::EN)
		, version("")
		, nostaleHash("")
	{
		size_t pos = Packet.find("\v");
		if (pos != std::string::npos)
		{
			Packet.replace(pos, 1, " ");
			std::vector<std::string> packs = split(Packet, " ");
			if (packs.size() == 8)
			{
				code = packs[1];
				gfuid = packs[2];
				randomPart = packs[3];
				lang = static_cast<LangType>(ToNumber<int>(packs[4].c_str()));
				version = packs[5];
				nostaleHash = packs[7];
			}
		}
	}

	~R_NoS0577()
	{

	}

	std::string getCode() const
	{
		return code;
	}

	std::string getGfuid() const
	{
		return gfuid;
	}

	std::string getRandomPart() const
	{
		return randomPart;
	}

	LangType getLang() const
	{
		return lang;
	}

	std::string getVersion() const
	{
		return version;
	}

	std::string getNostaleHash() const
	{
		return nostaleHash;
	}

private:
	std::string code;
	std::string gfuid;
	std::string randomPart;
	LangType lang;
	std::string version;
	std::string nostaleHash;
};
