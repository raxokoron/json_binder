#include <iostream>

class Path {
public:
	constexpr Path(const char* path) {
	}
	Path(const std::string& path) {
	}
private:
	void parseSimple() {
	}
};

#define TEST_ENABLED

int main() {
#ifdef TEST_ENABLED
	Path("a.b.c.d");
	Path("a.b[5].c.d");
	Path("[2]");
	Path("[2].a");
	Path("[2].a");
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
