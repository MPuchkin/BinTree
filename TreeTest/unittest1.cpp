#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\BSTreeNew\BStree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TreeTest
{		
	TEST_CLASS(BaseTests)
	{
	public:
		TEST_METHOD(TreeSizeTest)
		{
			Binary_Search_Tree<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			Assert::AreEqual(Tree.size(), Binary_Search_Tree<int>::size_type(9)); 
		}

	};
}