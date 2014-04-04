#pragma once

#include <deque>
#include <string>

#include "ParserAdapter.h"
#include "Common.h"

struct PathNode 
{
	bool isObjectChild;
	union 
	{
		const char* name;
		unsigned int index;
	} data;
};

template<typename JsonValue>
class Path 
{
public:
	Path(ParserAdapter<JsonValue>& adapter, const std::string& path) : 
		adapter_(adapter), path_(path) 
	{
		ASSERT(path_.length() > 0);
		nodes_ = parse(path_);
	}

	Path(Path&& other) = default;
	Path& operator=(Path&& other) = default;
	
	Path(const Path& other) = delete;
	Path& operator=(const Path& other) = delete;

	JsonValue* getChild(JsonValue* value, int index) 
	{
		auto& node = nodes_[index];
		if(node.isObjectChild)
			return adapter_.getChild(value, node.data.name);
		return adapter_.getChild(value, node.data.index);
	}
private:
	std::string path_;
	std::deque<PathNode> nodes_;
	ParserAdapter<JsonValue>& adapter_;
};

std::deque<Path::PathNode> parse(const std::string& path);
