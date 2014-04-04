#include "Path.h"

#include <cctype>

const char* skipSpaces(const char* str) 
{
	for(;std::isspace(*str);++str);
	return str;
}

unsigned int simpleAtoi(const char *p) 
{
	unsigned int x = 0;
	while (*p != '\0') 
	{
		x = (x*10) + (*p - '0');
		++p;
	}
	return x;
}

const char* parseArray(const char* data, PathNode& node)
{
	ASSERT(*data == '[');
	++data;
	data = skipSpaces(data);
	const char* start = data;
	for(; isdigit(*data); ++data);
	ASSERT((*data == ']') || std::isspace(*data));
	ASSERT(data - start > 0);

	node.isObjectChild = false;
	char tmp = *data;
	*data = '\0';
	node.data.index = simpleAtoi(start);
	*data = tmp;
	return data;
}

std::deque<PathNode> parse(const std::string& path);
{
	std::deque<PathNode> data;

	const char* data = path.c_str();

	while(*data != '\0')
	{
		ASSERT(*data != '.');
		if(*data == '[')
		{
		}
		else
		{
		}
	}

	return true;
}
