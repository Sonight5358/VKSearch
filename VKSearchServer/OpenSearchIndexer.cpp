#include "OpenSearchIndexer.h"
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

namespace
{
    json PostToJson(const Post& post)
    {
        json j;
        j["id"] = post.id;
        j["owner_id"] = post.owner_id;
        j["date"] = post.date;
        j["text"] = post.text;
        return j;
    }
}

void CreateIndex(const std::string& indexName, const std::string& mappingFilePath)
{
    //
}

void IndexPosts(const std::string& indexName, const std::vector<Post>& posts)
{
    httplib::Client cli("localhost", 9200);

    std::string bulkBody;

    for (const auto& post : posts)
    {
        json actionLine;
        actionLine["index"]["_index"] = indexName;
        actionLine["index"]["_id"] = post.id;

        bulkBody += actionLine.dump() + "\n";
        bulkBody += PostToJson(post).dump() + "\n";
    }

    auto res = cli.Post("/_bulk", bulkBody, "application/x-ndjson");

    if (!res)
    {
        throw std::runtime_error("Bulk indexing failed: " + httplib::to_string(res.error()));
    }

    json response = json::parse(res->body);

    if (response["errors"] == true)
    {
        std::cout << "Some documents failed to index\n";
    }

    std::cout << "Bulk indexed " << posts.size() << " posts\n";
}