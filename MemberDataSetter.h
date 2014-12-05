#pragma once

#include <type_traits>
#include "DataSetter.h"

template<typename JsonProcessor, typename DataT, typename MemberT>
class MemberDataSetter : public DataSetter<typename JsonProcessor::JsonNode, DataT>
{
public:
	using MemberPtr = MemberT DataT::*;
	using JsonNode = typename JsonProcessor::JsonNode;

	MemberDataSetter(MemberPtr ptr) : mPtr(ptr) {}

	virtual bool setData(JsonNode* value, DataT& data)
	{
		return setData_(value, data.*mPtr);
	}

private:
	template<typename T>
	struct DefaultHelper
	{
	static const bool value = std::is_same<int64_t, T>::value 
		|| std::is_same<uint64_t, T>::value 
		|| std::is_same<double, T>::value 
		|| !std::is_arithmetic<T>::value;
	};

	template<typename T>
	static inline bool setData__(JsonNode* node, MemberT& m)
	{
		T aTmp;
		if(JsonProcessor::getValue(node, aTmp))
		{
			m = static_cast<MemberT>(aTmp);
			return true;
		}
		return false;
	}
	
	template<typename T>
	static inline typename std::enable_if<DefaultHelper<T>::value, bool>::type setData_(JsonNode* node, T& t)
	{
		return JsonProcessor::getValue(node, t);
	}
	
	template<typename T>
	using SfinaeHelper = typename std::enable_if<T::value, bool>::type;
	
	template<typename T>
	static inline SfinaeHelper<std::is_signed<T>> setData_(JsonNode* node, T& t)
	{
		return setData__<int64_t>(node, t);
	}
	
	template<typename T>
	static inline SfinaeHelper<std::is_unsigned<T>> setData_(JsonNode* node, T& t)
	{
		return setData__<uint64_t>(node, t);
	}

	template<typename T>
	static inline SfinaeHelper<std::is_floating_point<T>> setData_(JsonNode* node, T& t)
	{
		return setData__<double>(node, t);
	}

	MemberPtr mPtr;
};
