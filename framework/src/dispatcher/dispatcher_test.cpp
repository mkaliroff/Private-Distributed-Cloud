// #include <iostream> // cout, endl
// #include <unistd.h> // sleep
// #include "dispatcher.hpp" // API


// using namespace ilrd_rd141;

// void TestOneSubscriber(void);
// void TestOneSubscriber1(void);
// void TestThreeSubscribers(void);
// void TestKillingsubscriber(void);
// void TestkillingDispatcher(void);

// void Notify(int bla);
// void Notify1(int bla);
// void Notify2(int bla);
// void Death(void);

// class KillCallback
// {
// public:
// 	KillCallback(Callback<int> *call) : m_call(call) {}
// 	void operator()(int check) {puts("Killing callback"); (void)check; delete m_call; m_call = 0;}
// private:
// 	Callback<int> *m_call;
// };

// class KillDispatcher
// {
// public:
// 	KillDispatcher(Dispatcher<int> *dis) : m_dis(dis) {}
// 	void operator()(int check) {puts("Killing dispatcher"); (void)check; delete m_dis; m_dis = 0;}
// private:
// 	Dispatcher<int> *m_dis;
// };

// int main(void)
// {
// 	TestOneSubscriber();
// 	puts("TestOneSubscriber passed!");
// 	puts("");
// 	TestOneSubscriber1();
// 	puts("TestOneSubscriber1 passed!");
// 	puts("");
// 	TestThreeSubscribers();
// 	puts("TestThreeSubscribers passed!");
// 	puts("");
// 	TestKillingsubscriber();
// 	puts("TestKillingsubscriber passed!");
// 	puts("");
// 	TestkillingDispatcher();
// 	puts("TestkillingDispatcher passed!");
// 	puts("");

// 	return 0;
// }

// void TestOneSubscriber()
// {
// 	Dispatcher<int> dispatcher;
// 	std::function<void(int)> notify = Notify;
// 	std::function<void(void)> death = Death;

// 	Callback<int> callback( notify, death);

// 	sleep(1);

// 	dispatcher.Notify(5);
// }

// void TestOneSubscriber1(void)
// {
// 	Dispatcher<int> dispatcher;
// 	std::function<void(int)> notify = Notify;
// 	std::function<void(void)> death = Death;

// 	{
// 		Callback<int> callback( notify, death);
// 	}

// 	dispatcher.Notify(5);
// }

// void TestThreeSubscribers(void)
// {
// 	Dispatcher<int> dispatcher;
// 	std::function<void(int)> notify = Notify;
// 	std::function<void(int)> notify1 = Notify1;
// 	std::function<void(int)> notify2 = Notify2;
// 	std::function<void(void)> death = Death;

// 	Callback<int> callback( notify, death);
// 	Callback<int> callback1( notify1, death);
// 	Callback<int> callback2( notify2, death);

// 	sleep(1);

// 	dispatcher.Notify(5);
// }

// void TestKillingsubscriber(void)
// {
// 	Dispatcher<int> dispatcher;
// 	std::function<void(int)> notify = Notify;
// 	std::function<void(int)> notify2 = Notify2;
// 	std::function<void(void)> death = Death;

// 	Callback<int> *callback = new Callback<int>( notify, death);
// 	Callback<int> callback1( KillCallback(callback), death);
// 	Callback<int> callback2( notify2, death);

// 	sleep(1);

// 	dispatcher.Notify(5);
// 	dispatcher.Notify(3);

// }

// void TestkillingDispatcher(void)
// {
// 	Dispatcher<int> *dispatcher = new Dispatcher<int>();
// 	std::function<void(int)> notify = Notify;
// 	std::function<void(int)> notify2 = Notify2;
// 	std::function<void(void)> death = Death;

// 	Callback<int> callback( notify, death);
// 	Callback<int> callback2( notify2, death);
// 	Callback<int> callback1( KillDispatcher(dispatcher), death);
	

// 	sleep(1);

// 	dispatcher->Notify(5);
// }

// /************************************************************/

// void Notify(int bla)
// {
// 	(void)bla;
// 	puts("I am after Notify function - Notify");
// }

// void Notify1(int bla)
// {
// 	(void)bla;
// 	puts("I am after Notify function - Notify1");
// }

// void Notify2(int bla)
// {
// 	(void)bla;
// 	puts("I am after Notify function - Notify2");
// }

// void Death()
// {
// 	puts("I am Death of despatcher function");
// }


#include <iostream> // cout, endl
#include <unistd.h> // sleep
#include "dispatcher.hpp" // API


using namespace ilrd_rd141;

void TestOneSubscriber(void);
void TestOneSubscriber1(void);
void TestThreeSubscribers(void);
void TestKillingsubscriber(void);
void TestkillingDispatcher(void);
void TestAddingSubscriber(void);

void Notify(int bla);
void Notify1(int bla);
void Notify2(int bla);
void Death(void);
void SpecialDeath(void);

class KillCallback
{
public:
	KillCallback(Callback<int> *call) : m_call(call) {}
	void operator()(int check) {puts("Killing callback"); (void)check; delete m_call; m_call = 0;}
private:
	Callback<int> *m_call;
};

Callback<int> *g_callback1;
bool g_done = false;

class CreateCallback
{
public:
	CreateCallback(Dispatcher<int> &dis) : m_dis(dis) {}
	void operator()(int check) 
	{
		(void)check;
		if(g_done == false) 
		{
			puts("Adding callback with notify1"); 
			std::function<void(int)> notify1 = Notify1;
			std::function<void(void)> death = SpecialDeath;
			g_callback1 = new Callback<int>(notify1, death);
			g_callback1->SetDispatcher(m_dis);
			g_done = true;
		}
	}
private:
	Dispatcher<int> &m_dis;
};

class KillDispatcher
{
public:
	KillDispatcher(Dispatcher<int> *dis) : m_dis(dis) {}
	void operator()(int check) {puts("Killing dispatcher"); (void)check; delete m_dis; m_dis = 0;}
private:
	Dispatcher<int> *m_dis;
};

int main(void)
{
	TestOneSubscriber();
	puts("TestOneSubscriber passed!");
	puts("");
	TestOneSubscriber1();
	puts("TestOneSubscriber1 passed!");
	puts("");
	TestThreeSubscribers();
	puts("TestThreeSubscribers passed!");
	puts("");
	TestKillingsubscriber();
	puts("TestKillingsubscriber passed!");
	puts("");
	/*TestkillingDispatcher();
	puts("TestkillingDispatcher passed!");
	puts("");*/ // test that shouldn't work
	TestAddingSubscriber();
	puts("TestAddingSubscriber passed!");
	puts("");

	return 0;
}

void TestOneSubscriber()
{
	Dispatcher<int> dispatcher;
	std::function<void(int)> notify = Notify;
	std::function<void(void)> death = Death;

	Callback<int> callback(notify, death);
	callback.SetDispatcher(dispatcher);

	sleep(1);

	dispatcher.Notify(5);
}

void TestOneSubscriber1(void)
{
	Dispatcher<int> dispatcher;
	std::function<void(int)> notify = Notify;
	std::function<void(void)> death = Death;

	{
		Callback<int> callback(notify, death);
		callback.SetDispatcher(dispatcher);
	}

	dispatcher.Notify(5);
}

void TestThreeSubscribers(void)
{
	Dispatcher<int> dispatcher;
	std::function<void(int)> notify = Notify;
	std::function<void(int)> notify1 = Notify1;
	std::function<void(int)> notify2 = Notify2;
	std::function<void(void)> death = Death;

	Callback<int> callback(notify, death);
	callback.SetDispatcher(dispatcher);
	Callback<int> callback1(notify1, death);
	callback1.SetDispatcher(dispatcher);
	Callback<int> callback2(notify2, death);
	callback2.SetDispatcher(dispatcher);

	sleep(1);

	dispatcher.Notify(5);
}

void TestKillingsubscriber(void)
{
	Dispatcher<int> dispatcher;
	std::function<void(int)> notify = Notify;
	std::function<void(int)> notify2 = Notify2;
	std::function<void(void)> death = Death;

	Callback<int> *callback = new Callback<int>(notify, death);
	callback->SetDispatcher(dispatcher);
	Callback<int> callback1(KillCallback(callback), death);
	callback1.SetDispatcher(dispatcher);
	Callback<int> callback2(notify2, death);
	callback2.SetDispatcher(dispatcher);

	sleep(1);

	dispatcher.Notify(5);
	dispatcher.Notify(3);

}

void TestkillingDispatcher(void)
{
	Dispatcher<int> *dispatcher = new Dispatcher<int>();
	std::function<void(int)> notify = Notify;
	std::function<void(int)> notify2 = Notify2;
	std::function<void(void)> death = Death;

	Callback<int> callback(notify, death);
	callback.SetDispatcher(*dispatcher);
	Callback<int> callback2(notify2, death);
	callback2.SetDispatcher(*dispatcher);
	Callback<int> callback1(KillDispatcher(dispatcher), death);
	callback1.SetDispatcher(*dispatcher);

	sleep(1);

	dispatcher->Notify(5);
}

void TestAddingSubscriber(void)
{
	Dispatcher<int> dispatcher;
	std::function<void(int)> notify = Notify;
	std::function<void(int)> notify2 = Notify2;
	std::function<void(void)> death = Death;

	Callback<int> callback(CreateCallback(dispatcher), death);
	callback.SetDispatcher(dispatcher);
	Callback<int> callback1(notify, death);
	callback1.SetDispatcher(dispatcher);
	Callback<int> callback2(notify2, death);
	callback2.SetDispatcher(dispatcher);

	sleep(1);

	dispatcher.Notify(5);
	dispatcher.Notify(3);

}

/************************************************************/

void Notify(int bla)
{
	(void)bla;
	puts("I am after Notify function - Notify");
}

void Notify1(int bla)
{
	(void)bla;
	puts("I am after Notify function - Notify1");
}

void Notify2(int bla)
{
	(void)bla;
	puts("I am after Notify function - Notify2");
}

void Death()
{
	puts("I am Death of despatcher function");
}

void SpecialDeath(void)
{
	delete g_callback1;
	g_callback1 = 0;
	puts("I am Death of despatcher function");
}