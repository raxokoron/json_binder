#pragma once

#include <string>

enum class NodeType
{
	null,
	boolean,
	integer,
	floatPoint,
	string,
	object,
	array
};

enum class ResultCode
{
	ok,
	parseError,
	wrongType,
	childNotFound,
	outOfBounds
};

const std::string& getResultDescription(ResultCode code);