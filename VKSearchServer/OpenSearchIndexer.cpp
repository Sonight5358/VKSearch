#include "OpenSearchIndexer.h"
#include "httplib.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

void CreateIndex(const std::string& indexName, const std::string& mappingFilePath)
{
    std::ifstream file(mappingFilePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open mapping file: " + mappingFilePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string mappingBody = buffer.str();

    httplib::Client cli("localhost", 9200);

    auto res = cli.Put(("/" + indexName).c_str(), mappingBody, "application/json");

    if (!res)
    {
        throw std::runtime_error("Failed to create index: " + httplib::to_string(res.error()));
    }

    std::cout << "Create index response: " << res->body << '\n';
}
