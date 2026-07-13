#pragma once
#include "Post.h"
#include <vector>
#include <string>

std::vector<Post> ParsePostsPage(const std::string& domain, const std::string& token, int offset, int count = 100);
std::vector<Post> ParseAllPosts(const std::string& domain, const std::string& token, int totalCount);