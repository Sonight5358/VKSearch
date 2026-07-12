#include "VkParser.h"
#include "Config.h"
#include <iostream>
#include <windows.h>
#include "OpenSearchIndexer.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    try
    {
        CreateIndex("vk_posts", "index_mapping.json");

        std::string token = LoadVkToken();
        std::string domain = "nust_misis";

        auto posts = ParseAllPosts(domain, token, 100);

        std::cout << "Total posts fetched: " << posts.size() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}