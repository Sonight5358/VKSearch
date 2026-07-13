#include "SearchServer.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    StartSearchServer("vk_posts", 8080);

    return 0;
}
