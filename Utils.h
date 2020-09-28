#pragma once

#include <string>
#include <vector>
#include <charconv>

const std::string HEXADECIMAL_UPPER_LOWER   = "0123456789abcdefABCDEF";
const std::string HEXADECIMAL_UPPER_ONLY    = "0123456789ABCDEF";
const std::string HEXADECIMAL_LOWER_ONLY    = "0123456789abcdef";

template <typename T>
static T ToNumber(const char* str)
{
    if (!str)
        return T{};
    T var = 0;
    auto [p, ec] = std::from_chars(str, str + strlen(str), var);
    if (ec != std::errc())
        return T{};
    return var;
}

static std::vector<std::string> split(const std::string& text, const std::string& delims)
{
    std::vector<std::string> tokens;
    std::size_t start = text.find_first_not_of(delims), end = 0;
    while ((end = text.find_first_of(delims, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(delims, end);
    }
    if (start != std::string::npos)
        tokens.push_back(text.substr(start));
    return tokens;
}

static unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else return '0';
}

static std::string hexToAscii(const std::string& in)
{
    std::string out;
    out.reserve(in.length() / 2);
    for (std::string::const_iterator p = in.begin(); p != in.end(); p++)
    {
        unsigned char c = hexval(*p);
        p++;
        if (p == in.end()) break;
        c = (c << 4) + hexval(*p);
        out.push_back(c);
    }
    return out;
}

static bool isUuidValid(std::string uuid)
{
    if (uuid.size() != 36)
        return false;
    if (uuid.find_first_not_of(HEXADECIMAL_LOWER_ONLY, 0) != 8 || uuid.at(8) != '-')
        return false;
    if (uuid.find_first_not_of(HEXADECIMAL_LOWER_ONLY, 9) != 13 || uuid.at(13) != '-')
        return false;
    if (uuid.find_first_not_of(HEXADECIMAL_LOWER_ONLY, 14) != 18 || uuid.at(18) != '-')
        return false;
    if (uuid.find_first_not_of(HEXADECIMAL_LOWER_ONLY, 19) != 23 || uuid.at(23) != '-')
        return false;
    if (!(uuid.find_first_not_of(HEXADECIMAL_LOWER_ONLY, 24) == std::string::npos))
        return false;
    return true;
}
