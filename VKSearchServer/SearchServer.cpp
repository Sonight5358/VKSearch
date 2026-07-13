#include "SearchServer.h"
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

namespace
{
    json SearchOpenSearch(const std::string& indexName, const std::string& query, int topN = 10)
    {
        httplib::Client cli("localhost", 9200);

        json searchBody;
        searchBody["size"] = topN;
        searchBody["query"]["match"]["text"] = query;

        std::string path = "/" + indexName + "/_search";

        auto res = cli.Post(path.c_str(), searchBody.dump(), "application/json");

        if (!res)
        {
            throw std::runtime_error("OpenSearch request failed: " + httplib::to_string(res.error()));
        }

        return json::parse(res->body);
    }
}

void StartSearchServer(const std::string& indexName, int port)
{
    httplib::Server svr;

    svr.Get("/search", [indexName](const httplib::Request& req, httplib::Response& res)
        {
            if (!req.has_param("q"))
            {
                res.status = 400;
                res.set_content(R"({"error":"missing query parameter 'q'"})", "application/json");
                return;
            }

            std::string query = req.get_param_value("q");

            try
            {
                json searchResult = SearchOpenSearch(indexName, query);

                json response = json::array();

                for (const auto& hit : searchResult["hits"]["hits"])
                {
                    json item;
                    item["score"] = hit["_score"];
                    item["id"] = hit["_source"]["id"];
                    item["owner_id"] = hit["_source"]["owner_id"];
                    item["text"] = hit["_source"]["text"];
                    item["link"] = "https://vk.com/wall" +
                        std::to_string(hit["_source"]["owner_id"].get<int64_t>()) +
                        "_" + std::to_string(hit["_source"]["id"].get<int64_t>());

                    response.push_back(item);
                }

                res.set_content(response.dump(2), "application/json");
            }
            catch (const std::exception& e)
            {
                res.status = 500;
                json error;
                error["error"] = e.what();
                res.set_content(error.dump(), "application/json");
            }
        });

    std::cout << "Search server listening on port " << port << '\n';
    svr.listen("0.0.0.0", port);
}