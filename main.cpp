#include <iostream>
#include <map>
#include <memory>
#include <cassert>
#include <functional>

typedef std::function<void (const char *)> Handler;

class Switchboard
{
public:
	inline void AddHandler(Handler handler, int id) { handlers[id] = handler; }
	inline void RemoveHandler(int type) { handlers.erase(type); }

	void Call(const char *message, int id) { handlers[id](message); }

private:
	std::map<int, Handler> handlers;
};

class A
{
public:
	A(const char *arg1, const char *arg2) : arg1(arg1), arg2(arg2) {}

	void Func1(const char *msg) { std::cout << msg << arg1 << std::endl; }
	void Func2(const char *msg) { std::cout << msg << arg2 << std::endl; }

private:
	const char *arg1;
	const char *arg2;
};

int main(int argc, char **argv)
{
	auto a = std::make_shared<A>("Hello, ", "World!");

	Switchboard board;
	board.AddHandler(std::bind(&A::Func1, a, std::placeholders::_1), 1);
	board.AddHandler(std::bind(&A::Func2, a, std::placeholders::_1), 2);

	// The functions have their own shared_ptrs
	assert(a.use_count() == 3);
	a.reset();

	board.Call(":: ", 1);
	board.Call(";; ", 2);

	board.RemoveHandler(1);
	board.RemoveHandler(2);

	system("pause");
	return 0;
}