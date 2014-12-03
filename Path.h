#pragma once

#include <vector>
#include <string>

class Path final
{
public:
	enum class Type
	{
		index,
		name
	};

	struct Node
	{
		Node(const char* name) : mType(Type::name)
		{
			mData.mName = name;
		}
		Node(unsigned int index) : mType(Type::index)
		{
			mData.mIndex = index;
		}
	
		Type mType;
		union {
			const char* mName;
			unsigned int mIndex;
		} mData;
	};
	
	Path() = default;
	Path(const Path&) = default;
	Path(Path&&) = default;
	
	Path& operator=(const Path&) = default;
	Path& operator=(Path&&) = default;
	
	bool init(std::string path);
	
	const std::string& getPath() { return mPath; }
	const std::vector<Node>& getNodes() { return mNodes; }
	
	bool operator<(const Path& other) { return mPath < other.mPath; }
private:
	bool parse();

	std::string mPath;
	std::string mBuffer;
	std::vector<Node> mNodes;
};
