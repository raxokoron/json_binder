#pragma once

#include <memory>
#include <string>

#include <rapidjson/document.h>
#include "Types.h"

class RapidJsonProcessor
{
public:
	using JsonNode = rapidjson::Value;

	static std::unique_ptr<JsonNode> parse(const std::string& data, std::string& errorDescription)
	{
		std::unique_ptr<rapidjson::Document> aDoc(new rapidjson::Document);
		aDoc->Parse<0>(data.c_str());
		if(aDoc->HasParseError())
		{
			errorDescription = aDoc->GetParseError();
			return nullptr;
		}
		return std::move(aDoc);
	}

	static NodeType getType(JsonNode* node)
	{
		using namespace rapidjson;
		ASSERT(node != nullptr);
		switch(node->GetType())
		{
		case kNullType:
			return NodeType::null;
		case kFalseType:
		case kTrueType:
			return NodeType::boolean;
		case kNumberType:
			return node->IsDouble() ? NodeType::floatPoint : NodeType::integer;
		case kStringType:
			return NodeType::string;
		case kObjectType:
			return NodeType::object;
		case kArrayType:
			return NodeType::array;
		}
		ASSERT(!"Bad type!");
		return NodeType::null;
	}

	static unsigned int size(JsonNode* node)
	{
		ASSERT(node != nullptr);
		return node->Size();
	}

	static JsonNode* getChild(JsonNode* node, unsigned int i, ResultCode& code)
	{
		ASSERT(node != nullptr);
		if(!node->IsArray())
		{
			code = ResultCode::wrongType;
			return nullptr;
		}
		if(node->Size() <= i)
		{
			code = ResultCode::outOfBounds;
			return nullptr;
		}
		return &(*node)[i];
	}
	static JsonNode* getChild(JsonNode* node, const char* name, ResultCode& code)
	{
		ASSERT(node != nullptr);
		ASSERT(name != nullptr);
		if(!node->IsObject())
		{
			code = ResultCode::wrongType;
			return nullptr;
		}
		JsonNode* aChild = &(*node)[name];
		if(aChild->IsNull() && !node->HasMember(name))
		{
			code = ResultCode::childNotFound;
			return nullptr;
		}
		return aChild;
	}
	
	static bool getValue(JsonNode* node, bool& value)
	{
		ASSERT(node != nullptr);
		if(node->IsBool())
		{
			value = node->GetBool();
			return true;
		}
		return false;
	}
	static bool getValue(JsonNode* node, int64_t& value)
	{
		ASSERT(node != nullptr);
		if(node->IsInt64())
		{
			value = node->GetInt64();
			return true;
		}
		return false;
	}
	static bool getValue(JsonNode* node, uint64_t& value)
	{
		ASSERT(node != nullptr);
		if(node->IsUint64())
		{
			value = node->GetUint64();
			return true;
		}
		return false;
	}
	static bool getValue(JsonNode* node, double& value)
	{
		ASSERT(node != nullptr);
		if(node->IsDouble())
		{
			value = node->GetDouble();
			return true;
		}
		return false;
	}
	static bool getValue(JsonNode* node, std::string& value)
	{
		ASSERT(node != nullptr);
		if(node->IsString())
		{
			value = node->GetString();
			return true;
		}
		return false;
	}
};
