#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\BSTreeNew\BStree.h"
#include <set>
#include <functional>
#include <memory_resource>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TreeTest
{		
	TEST_CLASS(BaseTests)
	{
	public:
		
		TEST_METHOD(TreeSizeTest)
		{
			//  Тестирование размера дерева при разных сценариях использования
			Binary_Search_Tree<int> T1;
			Assert::AreEqual(T1.size(), Binary_Search_Tree<int>::size_type(0), L"Пустое дерево имеет ненулевой размер!");
			Binary_Search_Tree<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			Assert::AreEqual(Tree.size(), Binary_Search_Tree<int>::size_type(9), L"Неверно указывается размер дерева!");
			Binary_Search_Tree<int> Tree2(Tree);
			Assert::AreEqual(Tree.size(),Tree2.size(), L"Неверно указывается размер после копирования!");
			Logger::WriteMessage("Всё чудесно!");
		}

		TEST_METHOD(TreesEqualityTest)
		{
			//  Тестирование оператора == для дерева
			Binary_Search_Tree<int> T1;
			Assert::AreEqual(T1.size(), Binary_Search_Tree<int>::size_type(0), L"Пустое дерево имеет ненулевой размер!");
			Binary_Search_Tree<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			Assert::AreEqual(Tree.size(), Binary_Search_Tree<int>::size_type(9), L"Неверно указывается размер дерева!");
			Binary_Search_Tree<int> Tree2(Tree);
			Assert::IsTrue(Tree.size() == Tree2.size(), L"Неверно указывается размер после копирования!");
			Logger::WriteMessage("Всё чудесно!");
		}

	};

	TEST_CLASS(SetTests)
	{
		///  Тесты стандартного контейнера std::set, из книги "The C++ Standard template library" Плаугера, Степанова и др.
	public:

		typedef std::allocator<char> Myal;
		typedef std::less<char> Mypred;
		typedef std::set<char, Mypred, Myal> Mycont;

		TEST_METHOD(SetSize)
		{
			Mycont v0;
			Myal al = v0.get_allocator();
			Mypred pred;
			Mycont v0a(pred), v0b(pred, al);
			Assert::IsTrue(v0.empty() && v0.size() == 0, L"Неверный размер пустого set");
			Assert::IsTrue(v0a.size() == 0 && v0a.get_allocator() == al, L"Неверный размер или аллокатор");
			Assert::IsTrue(v0b.size() == 0 && v0b.get_allocator() == al, L"Неверный размер или аллокатор");
		}

		TEST_METHOD(SetCreation)
		{
			char carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Myal al = v0.get_allocator();
			Mypred pred;
			Mycont v0a(pred);

			Mycont v1(carr, carr + 3);
			Assert::IsTrue(v1.size() == 3 && *v1.begin() == 'a', L"Неверно создаётся set символов");
			
			Mycont v2(carr, carr + 3, pred);
			Assert::IsTrue(v2.size() == 3 && *v2.begin() == 'a', L"Неверно создаётся set символов");

			Mycont v3(carr, carr + 3, pred,al);
			Assert::IsTrue(v3.size() == 3 && *v3.begin() == 'a', L"Неверно создаётся set символов");

			const Mycont v4(carr, carr + 3);
			v0 = v4;
			Assert::IsTrue(v1.size() == 3 && *v1.begin() == 'a', L"Неверно работает оператор присваивания для set");
		}

		TEST_METHOD(SetIterators)
		{
			char ch('a'), carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Myal al = v0.get_allocator();
			Mypred pred;
			Mycont v0a(pred);

			Mycont v1(carr, carr + 3);
			Mycont v2(carr, carr + 3, pred);
			Mycont v3(carr, carr + 3, pred, al);
			const Mycont v4(carr, carr + 3);
			v0 = v4;

			Mycont::iterator p_it(v1.begin());

			char ch('a'), carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Myal al = v0.get_allocator();
			Mypred pred;
			Mycont v0a(pred);

			Mycont v1(carr, carr + 3);
			Assert::IsTrue(v1.size() == 3 && *v1.begin() == 'a', L"Неверно создаётся set символов");

			Mycont v2(carr, carr + 3, pred);
			Assert::IsTrue(v2.size() == 3 && *v2.begin() == 'a', L"Неверно создаётся set символов");

			Mycont v3(carr, carr + 3, pred, al);
			Assert::IsTrue(v3.size() == 3 && *v3.begin() == 'a', L"Неверно создаётся set символов");

			const Mycont v4(carr, carr + 3);
			v0 = v4;
			Assert::IsTrue(v1.size() == 3 && *v1.begin() == 'a', L"Неверно работает оператор присваивания для set");
		}

		TEST_METHOD(TreesEqualityTest)
		{
			//  Тестирование оператора == для дерева
			Binary_Search_Tree<int> T1;
			Assert::AreEqual(T1.size(), Binary_Search_Tree<int>::size_type(0), L"Пустое дерево имеет ненулевой размер!");
			Binary_Search_Tree<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			Assert::AreEqual(Tree.size(), Binary_Search_Tree<int>::size_type(9), L"Неверно указывается размер дерева!");
			Binary_Search_Tree<int> Tree2(Tree);
			Assert::AreEqual(Tree.size(), Tree2.size(), L"Неверно указывается размер после копирования!");
			Logger::WriteMessage("Всё чудесно!");
		}

	};
}