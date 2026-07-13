#pragma once
#include "Post.h"
#include <string>
#include <vector>

void CreateIndex(const std::string& indexName, const std::string& mappingFilePath);
void IndexPosts(const std::string& indexName, const std::vector<Post>& posts);