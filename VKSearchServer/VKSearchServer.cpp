#include "VkParser.h"
#include "Config.h"
#include <iostream>
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    try
    {
        std::string token = LoadVkToken();
        std::string domain = "nust_misis";

        auto posts = ParseAllPosts(domain, token, 21000);

        for (const auto& post : posts)
        {
            /*std::cout << "ID: " << post.id << '\n';
            std::cout << "Date: " << post.date << '\n';
            std::cout << "Text: " << post.text << "\n\n";*/
        }

        std::cout << "Total posts fetched: " << posts.size() << '\n';


    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}