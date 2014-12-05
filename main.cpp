#include <iostream>
#include <string>
#include "Assert.h"

#include "JsonParser.h"
#include "RapidJsonProcessor.h"

template<typename DataT>
struct Test
{
	std::string json;
	std::function<void(JsonParser<RapidJsonProcessor, DataT>&)> bindings;
	DataT etalon;
};

template<typename DataT>
bool test(const Test<DataT>& t)
{
	ASSERT(!t.json.empty());
	ASSERT(t.bindings);

	JsonParser<RapidJsonProcessor, DataT> parser;
	t.bindings(parser);
	
	DataT userData;
	ResultCode code = parser.parse(t.json, userData);
	if(code != ResultCode::ok)
	{
		std::cout << "Errrr (" << getResultDescription(code) << "): " << parser.getErrorDescription();
		return false;
	}
	
	return userData == t.etalon;
}

struct BinderTest0
{
	struct User
	{
		int index;
		std::string name;
		std::string first_name;
		std::string last_name;
		unsigned int age;
		std::string gender;
		
		bool operator==(const User& other)
		{
			return (index == other.index) 
				&& (name == other.name)
				&& (first_name == other.first_name)
				&& (last_name == other.last_name)
				&& (age == other.age)
				&& (gender == other.gender);
		}
	};
	static Test<User> create()
	{
		Test<User> t;
		t.json = 
		R"({"Users" : [
			{
				"index" : 0,
				"name" : {
					"first name" : "Jin",
					"last name" : "Kisaragi"
				},
				"age" : 21,
				"gender" : "male"
			}
		]})";
		t.bindings = [](JsonParser<RapidJsonProcessor, User>& parser){
			parser.bind("Users[0].index", &User::index, true);
			parser.bind("Users[0].name.first name", &User::name, false);
			parser.bind("Users[0].name.first name", &User::first_name, true);
			parser.bind("Users[0].name.last name", &User::last_name, true);
			parser.bind("Users[0].age", &User::age, true);
			parser.bind("Users[0].gender", &User::gender, true);
		};
		t.etalon.index = 0;
		t.etalon.name = "Jin";
		t.etalon.first_name = "Jin";
		t.etalon.last_name = "Kisaragi";
		t.etalon.age = 21;
		t.etalon.gender = "male";
		return std::move(t);
	}
};


/*
Plan:
1) Custom errors
2) Conditions
3) Custom setters
4) Binder bindings
5) Error descriptions
6) map<std::string, T> binding for objects;
7) std::vector<T>, std::deque<T>, std::list<T> bindings for arrays;
8) Path tree builder;
9) Builder and parser classes;
*/

#include <iostream>
#define CHECK(tst) { if(!(tst)) { std::cout << "Test failed: " << #tst << std::endl; }}

int main() 
{
	CHECK(test(BinderTest0::create()));

	return 0;
}
