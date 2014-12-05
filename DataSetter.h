#pragma once

template<typename JsonNode, typename DataT>
class DataSetter
{
public:
	virtual ~DataSetter() {}
	virtual bool setData(JsonNode* value, DataT& data) = 0;
};