#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "VKParser.h"
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <stdexcept>

using json = nlohmann::json;

namespace
{
    json FetchPostsPage(httplib::Client& cli, const std::string& domain, const std::string& token, int offset, int count = 100)
    {
        std::string path = "/method/wall.get?domain=" + domain +
            "&offset=" + std::to_string(offset) +
            "&count=" + std::to_string(count) +
            "&access_token=" + token +
            "&v=5.199";

        const int maxRetries = 3;

        for (int attempt = 1; attempt <= maxRetries; ++attempt)
        {
            auto res = cli.Get(path);

            if (res)
            {
                return json::parse(res->body);
            }

            std::cout << "Attempt " << attempt << " failed, retrying...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
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