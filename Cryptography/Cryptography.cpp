#include "Cryptography.h"
#include "md5.h"

std::string Cryptography::LoginEncrypt(std::string packet)
{
    std::string output;
    if(packet.back() != 0xA)
        packet.push_back((unsigned char)'\n');
    for (size_t i = 0; i < packet.size(); i++)
        output.push_back((packet[i] + 0xF) & 0xFF);
    return output;
}

std::string Cryptography::LoginDecrypt(std::string packet)
{
    std::string output;
	for (size_t i = 0; i < packet.size(); i++)
		output.push_back(((packet[i] - 0xF) ^ 0xC3) & 0xFF);
	return output;
}

std::string Cryptography::HashMd5(std::string hash, bool lower)
{
    return md5(hash, lower);
}
