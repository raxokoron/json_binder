#include <iostream>
#include <set>

#include "../Path.h"

bool testPathSet(const std::set<std::string>& set, bool result)
{
	for(auto it = set.begin(); it != set.end(); ++it)
	{
		if(Path(*it).hasFailed() == result)
		{
			std::cout << "Test failed at: " << *it << std::endl;
			return false;
		}
	}
	return true;;
}

int main()
{
	std::set<std::string> aGoodPath = {
		"a.b.c.d",
		"a.b[5].c.d",
		"[2]",
		"[2].a",
		"a.b.c[3].d",
		"[1][2][3]",
		"[ 256 ].a.b",
		R"(a b . 31q\.111. \\\.)",
		R"(\.b.c.d)",
		R"(\[ 0 \].a.b)"
	};

	std::set<std::string> aBadPath = {
		"a.b.c[3]dqb",
		".b.c.d",
		R"(\b.c.d)",
		R"(a.\b.c.d)",
		"[a].b.c.d",
		"a.[b].c.d",
		"a[b].c.d",
		"a[ b ].c.d",
		"a[].c.d",
		"a[   ].c.d",
		"a[ 5 4 ].c.d",
		"a[ 5  ",
		R"(a b . 31q\.111. \\.)",
		R"(a\[   ].c.d)",
		R"(a[   \].c.d)",
		"].c.d"
	};
	
	if(!testPathSet(aGoodPath, true))
		return 1;
		
	if(!testPathSet(aBadPath, false))
		return 1;
		
	return 0;
}