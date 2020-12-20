#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{
	TEST_CLASS(EngineTests)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			const auto a = 5;
			const auto b = 7;
			const auto sum = a + b;
			Assert::AreEqual(sum, a + b);
		}
	};
}
