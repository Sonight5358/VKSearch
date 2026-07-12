#pragma once
#include <string>
#include <fstream>
#include <stdexcept>

inline std::string LoadVkToken(const std::string& path = "vk_token.txt")
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open token file: " + path);
    }

    std::string token;
    std::getline(file, token);

    return token;
}
