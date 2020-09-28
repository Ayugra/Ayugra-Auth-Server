#pragma once

#include <string>
#include <vector>

namespace Cryptography
{
    std::string LoginEncrypt(std::string packet);
    std::string LoginDecrypt(std::string packet);
    std::string HashMd5(std::string hash, bool lower = true);
};

