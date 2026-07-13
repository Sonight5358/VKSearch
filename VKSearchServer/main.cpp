#include "VkParser.h"
#include "Config.h"
#include "OpenSearchIndexer.h"
#include <iostream>
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    try
    {
        CreateIndex("vk_posts", "index_mapping.json");

        std::string token = LoadVkToken();
        std::string domain = "nust_misis";
        int totalCount = 21000;

        int totalIndexed = 0;

        for (int offset = 0; offset < totalCount; offset += 100)
        {
            auto page = ParsePostsPage(domain, token, offset);
            IndexPosts("vk_posts", page);

            totalIndexed += static_cast<int>(page.size());
            std::cout << "Progress: " << totalIndexed << " / " << totalCount << '\n';
        }

        std::cout << "Done! Total indexed: " << totalIndexed << '\n';
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}