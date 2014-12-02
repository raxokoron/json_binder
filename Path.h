#pragma once

#include <vector>
#include <string>

class Path
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
	
	Path(std::string path);
	Path(const char* path) : Path(std::string(path)) {}
	
	bool hasFailed() { return mHasFailed; }
	
	const std::string& getPath() { return mPath; }
	const std::vector<Node>& getNodes() { return mNodes; }
	
	bool operator<(const Path& other) { return mPath < other.mPath; }
private:
	bool parse();

	const std::string mPath;
	std::string mBuffer;
	bool mHasFailed;
	std::vector<Node> mNodes;
};
