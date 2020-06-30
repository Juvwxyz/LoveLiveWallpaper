#include <iostream>
#include "pch.h"
#include "../LoveLiveWallpaper/src/EventSystem.h"

namespace LLWP
{


	typedef Delegate<int(int, int)> TestEventHandler;

	int testFunc1(int a, int b)
	{
		std::cout << "testFunc1 " << a + b << std::endl;
		return a + b;
	}

	int testFunc2(int a, int b)
	{
		std::cout << "testFunc2 " << a + b << std::endl;
		return a + b;
	}

	class Sender
	{
	private:
		TestEventHandler testEventHandler;
	public:
		Sender() : TestEvent(testEventHandler) {}
		int Invoke(int a, int b) { return testEventHandler.invoke(a, b); }
		Event<TestEventHandler> TestEvent;
	};

	class Reciver
	{
	private:
		int id;
	public:
		Reciver(int i) :id(i) {}

		int testHandler(int a, int b)
		{
			std::cout << "Reciver " << id << ": Recv " << a + b << std::endl;
			return a + b;
		}
	};

	class EventTest : public ::testing::Test
	{
	public:

	};

	TEST_F(EventTest, MemberFunctionTest)
	{
		Sender s;
		Reciver r1(1);
		Reciver r2(2);
		s.TestEvent += (r1, &Reciver::testHandler);
		s.TestEvent += (r2, &Reciver::testHandler);
		EXPECT_EQ(s.Invoke(1, 1), 2);
		s.TestEvent -= (r2, &Reciver::testHandler);
		EXPECT_EQ(s.Invoke(2, 2), 4);
	}

	TEST_F(EventTest, NonMemberFunctionTest)
	{
		TestEventHandler testEventHandler;
		testEventHandler += testFunc1;
		testEventHandler += testFunc2;
		EXPECT_EQ(testEventHandler.invoke(1, 1), 2);
		testEventHandler -= testFunc2;
		EXPECT_EQ(testEventHandler.invoke(2, 2), 4);
	}
}