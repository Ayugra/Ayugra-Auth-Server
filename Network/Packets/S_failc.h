#pragma once

#include <string>

class S_failc
{
public:
	S_failc(int ErrorCode)
	{
		errorCode = ErrorCode;
	}

	~S_failc()
	{

	}

	const std::string getPacket() const
	{
		std::string packet = HEADER;
		packet += " " + std::to_string(errorCode);
		return packet;
	}

private:
	int errorCode;

	const std::string HEADER = "failc";
};