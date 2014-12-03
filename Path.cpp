#include "Path.h"

#include <cctype>
#include <cstring>
#include "pow10.h"

namespace
{
	inline bool isDelimChar(const char* c)
	{
		return (*c == '.') || (*c == '[');
	}

	inline bool isControlChar(const char* c)
	{
		return isDelimChar(c) || (*c == ']') || (*c == '\\');
	}

	char* parseName(char* start)
	{
		char* c = start;
		char* cw = c;
		while(!isDelimChar(c) && (*c != '\0'))
		{
			if(*c == '\\')
			{
				++c;
				if(!isControlChar(c))
					return nullptr;
			}
			else if(*c == ']')
			{
				return nullptr;
			}
			*cw = *c;
			++c;
			++cw;
		}
		if(c == start)
			return nullptr;
		memset(cw, '\0', c - cw);
		return c;
	}

	char* parseArrayIndex(char* c, unsigned int& index)
	{
		++c;
		const char* aNumStart = nullptr;
		const char* aNumEnd = nullptr;
		while(*c != ']')
		{
			if(std::isdigit(*c))
			{
				if(aNumStart == nullptr)
				{
					aNumStart = c;
				}
				else if(aNumEnd != nullptr)
				{
					return nullptr;
				}
			}
			else if(std::isspace(*c))
			{
				if(aNumStart != nullptr)
					aNumEnd = c;
			}
			else
			{
				return nullptr;
			};
			++c;
		}
		
		if(aNumStart == nullptr)
			return nullptr;
		
		if(aNumEnd == nullptr)
			aNumEnd = c;
		
		index = 0;
		for(;aNumStart != aNumEnd; ++aNumStart)
		{
			index += (*aNumStart - '0') * pow10<unsigned int>(aNumEnd - aNumStart - 1);
		}
		++c;
		return c;
	}
}

bool Path::init(std::string path)
{
	if(path.empty() || (path[0] == '.'))
		return false;

	mPath = std::move(path);
	mBuffer = (mPath[0] == '[') ? mPath : '.' + mPath;
	bool aResult = parse();
	if(!aResult)
	{
		mPath.clear();
		mBuffer.clear();
		mNodes.clear();
	}
	return aResult;
}

/*
expr = <name_index> [<expr_left>] 
name_index = <index> | <name>
index = '[' <spaces> <number> <spaces> ']'
expr_left = '.'<name>[<expr_left>] | <index>[<expr_left>]
name = ...
number = '[0-9]+'
*/
bool Path::parse()
{
	char* c = &mBuffer[0];
	while(*c != '\0')
	{
		if(*c == '.')
		{
			*c = '\0';
			++c;
			char* cn = parseName(c);
			if(cn == nullptr)
				return false;
			mNodes.emplace_back(static_cast<const char*>(c));
			c = cn;
		}
		else if(*c == '[')
		{
			unsigned int aIndex = 0;
			char* cn = parseArrayIndex(c, aIndex);
			if(cn == nullptr)
				return false;
			*c = '\0';
			c = cn;
			mNodes.emplace_back(aIndex);
		}
		else 
		{
			return false;
		}
	}
	return true;
}
