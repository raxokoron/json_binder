#pragma once

template<typename JsonValue>
class ParserAdapter 
{
public:
	virtual ~ParserAdapter() {}

	virtual JsonValue* getChild(JsonValue* value, const char* name) = 0;
	virtual JsonValue* getChild(JsonValue* value, int index) = 0;
};
