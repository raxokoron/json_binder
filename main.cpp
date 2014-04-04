#include <iostream>

#define TEST_ENABLED

class SmartJsonValue {};

class TestAdapter : public ParserAdapter<SmartJsonValue> 
{
public:
	virtual SmartJsonValue* getChild(SmartJsonValue* value, const std::string& name) 
	{
		std::cout << "GetChild(s): " << name << std::endl;
		return nullptr;
	}
	virtual SmartJsonValue* getChild(SmartJsonValue* value, int index) 
	{
		std::cout << "GetChild(i): " << index << std::endl;
		return nullptr;	}
};

int main() 
{
#ifdef TEST_ENABLED
	TestAdapter adapter;

	using MyPath = Path<SmartJsonValue>;

	MyPath(adapter, "a.b.c.d");
	MyPath(adapter, "a.b[5].c.d");
	MyPath(adapter, "[2]");
	MyPath(adapter, "[2].a");
	MyPath(adapter, "[2].a");
#endif

	/*
	struct A {
		int a;
	};


	json_binder binder;
	binder.bind("a.b.c.d", &A::a, true, MySmartError());
	binder.bind("a.b.c.d", &A::a, (Exists("a.b.c.e") == 5),  MySmartError());

	...
	
	A a;
	const char* jsonData;
	Error err = binder.parse(jsonData, a);

	std::cout << a.a << std::endl;
	*/

	return 0;
}
