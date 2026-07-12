#include "VKParser.h"
#include "nlohmann/json.hpp"
#include <stdexcept>
#include <cstdio>
#include <memory>
#include <array>
#include <thread>
#include <chrono>
#include <iostream>

using json = nlohmann::json;

namespace
{
    std::string RunCurl(const std::string& url)
    {
        std::string command = "curl -s \"" + url + "\"";
        std::array<char, 4096> buffer;
        std::string result;

        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);

        if (!pipe)
        {
            throw std::runtime_error("Failed to launch curl");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }

        return result;
    }

    json FetchPostsPage(const std::string& domain, const std::string& token, int offset, int count = 100)
    {
        std::string url = "https://api.vk.com/method/wall.get?domain=" + domain +
            "&offset=" + std::to_string(offset) +
            "&count=" + std::to_string(count) +
            "&access_token=" + token +
            "&v=5.199";

        const int maxRetries = 3;

        for (int attempt = 1; attempt <= maxRetries; ++attempt)
        {
            std::string body = RunCurl(url);

            if (!body.empty())
            {
                return json::parse(body);
            }

            std::cout << "Attempt " << attempt << " failed (empty response), retrying...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        throw std::runtime_error("VK request failed after " + std::to_string(maxRetries) + " attempts");
    }
}

std::vector<Post> ParseAllPosts(const std::string& domain, const std::string& token, int totalCount)
{
    std::vector<Post> posts;

    for (int offset = 0; offset < totalCount; offset += 100)
    {
        json page = FetchPostsPage(domain, token, offset);

        for (const auto& item : page["response"]["items"])
        {
            Post post;
            post.id = item["id"];
            post.owner_id = item["owner_id"];
            post.date = item["date"];
            post.text = item["text"];

            posts.push_back(post);
        }
    }

    return posts;
}