#include <iostream>

#include "httplib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
    httplib::Client cli("localhost", 9200);

    auto res = cli.Get("/");

    if (res)
    {
        std::cout << "Status: " << res->status << '\n';
        std::cout << "Body: " << res->body << '\n';

        json parsed = json::parse(res->body);
        std::cout << "Cluster name: " << parsed["cluster_name"] << '\n';
    }
    else
    {
        std::cout << "Request failed: " << httplib::to_string(res.error()) << '\n';
    }

    return 0;
}