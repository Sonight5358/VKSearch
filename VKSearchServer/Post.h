#pragma once
#include <string>
#include <cstdint>

struct Post
{
	int64_t id;
	int64_t owner_id;
	int64_t date;
	std::string text;
};