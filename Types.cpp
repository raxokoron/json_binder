#include "Types.h"

#include <vector>

const std::string& getResultDescription(ResultCode code)
{
/*
// Kinda debug
	static const std::map<ResultCode, std::string> aNames = [](){
		std::map<ResultCode, std::string> aNames;
		aNames[ResultCode::ok] = "ok";
		aNames[ResultCode::parseError] = "parser error";
		aNames[ResultCode::wrongType] = "wrong type";
		aNames[ResultCode::childNotFound] = "child not found";
		aNames[ResultCode::outOfBounds] = "out of bounds";
	}();
	return aNames[code];
*/

	static const std::vector<std::string> aNames = { 
		"ok",
		"parser error",
		"wrong type",
		"child not found",
		"out of bounds"
	};
	return aNames[static_cast<int>(code)];
}