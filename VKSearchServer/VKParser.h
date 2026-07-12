#pragma once
#include "Post.h"
#include <vector>
#include <string>

std::vector<Post> ParseAllPosts(const std::string& domain, const std::string& token, int totalCount);