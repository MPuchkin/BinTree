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
			Assert::AreEqual(Tree.size(), Tree2.size(), L"Неверно указывается размер после копирования!");
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
			Logger::WriteMessage("Ого, кажется всё хорошо!");
		}
		TEST_METHOD(TreesEqualityTest2)
		{
			//  Тестирование оператора == для дерева
			Binary_Search_Tree<int> T1;
			Assert::AreEqual(T1.size(), Binary_Search_Tree<int>::size_type(0), L"Пустое дерево имеет ненулевой размер!");
			Binary_Search_Tree<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			Assert::AreEqual(Tree.size(), Binary_Search_Tree<int>::size_type(9), L"Неверно указывается размер дерева!");
			Binary_Search_Tree<int> Tree2(Tree);
			Assert::AreEqual(Tree.size(), Tree2.size(), L"Неверно указывается размер после копирования!");
		}
	};
	
	TEST_CLASS(SetTests)
	{
		//  Тесты стандартного контейнера std::set, из книги "The C++ Standard template library" Плаугера, Степанова и др.
	public:

		using Myal = std::allocator<char>;
		using Mypred = std::less<char>;
		
		//  Для того, чтобы выполнить тестирование одного из указанных контейнеров (std::set или Binary_Tree_Search)
		//    должна быть раскомментирована одна из следующих строк:
		template<typename T> using ContainerTemplate = std::set<T, Mypred, Myal>;
		//template<typename T> using ContainerTemplate = Binary_Search_Tree<T, Mypred, Myal>;

		using Mycont = ContainerTemplate<char>;

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

			Mycont v3(carr, carr + 3, pred, al);
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
			Mycont::const_iterator p_cit(v4.begin());
			Mycont::reverse_iterator p_rit(v1.rbegin());
			Mycont::const_reverse_iterator p_crit(v4.rbegin());

			Assert::IsTrue(*p_it == 'a' && *--(p_it = v1.end()) == 'c', L"Декремент end() не корректен?");
			Assert::IsTrue(*p_cit == 'a' && *--(p_cit = v4.end()) == 'c', L"Декремент для const iterator на end() не корректен?");
			Assert::IsTrue(*p_rit == 'c' && *--(p_rit = v1.rend()) == 'a', L"Reverse iterator не корректен?");
			Assert::IsTrue(*p_crit == 'c' && *--(p_crit = v4.rend()) == 'a', L"Const reverse iterator не корректен?");
		}

		TEST_METHOD(SetInsertEraseTests)
		{
			char carr[] = "abc", carr2[] = "def";
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
			Mycont::const_iterator p_cit(v4.begin());
			Mycont::reverse_iterator p_rit(v1.rbegin());
			Mycont::const_reverse_iterator p_crit(v4.rbegin());

			v0.clear();
			std::pair<Mycont::iterator, bool> pib = v0.insert('d');
			Assert::IsTrue(*pib.first == 'd' && pib.second);
			Assert::IsTrue(*--v0.end() == 'd');
			pib = v0.insert('d');
			Assert::IsTrue(*pib.first == 'd' && !pib.second);
			Assert::IsTrue(*v0.insert(v0.begin(), 'e') == 'e');
			v0.insert(carr, carr + 3);
			Assert::IsTrue(v0.size() == 5 && *v0.begin() == 'a');
			v0.insert(carr2, carr2 + 3);
			Assert::IsTrue(v0.size() == 6 && *--v0.end() == 'f');
			Assert::IsTrue(*v0.erase(v0.begin()) == 'b' && v0.size() == 5);
			Assert::IsTrue(*v0.erase(v0.begin(), ++v0.begin()) == 'c' && v0.size() == 4);
			Assert::IsTrue(v0.erase('x') == 0 && v0.erase('e') == 1);
		}

		TEST_METHOD(SetSwapAndCompTests)
		{
			char carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Mycont v1(carr, carr + 3);

			v0.clear();
			std::pair<Mycont::iterator, bool> pib = v0.insert('d');
			pib = v0.insert('d');
			v0.insert(v0.begin(), 'e');
			v0.insert(carr, carr + 3);
			v0.insert(carr2, carr2 + 3);
			v0.erase(v0.begin());
			v0.erase(v0.begin(), ++v0.begin());
			v0.erase('x');
			v0.erase('e');
			v0.clear();
			Assert::IsTrue(v0.empty());
			v0.swap(v1);
			Assert::IsTrue(!v0.empty() && v1.empty());
			std::swap(v0, v1);
			Assert::IsTrue(v0.empty() && !v1.empty());
			Assert::IsTrue(v1 == v1 && v0 < v1, L"Сравнение множеств некорректно!");
			Assert::IsTrue(v0 != v1 && v1 > v0, L"Сравнение множеств некорректно!");
			Assert::IsTrue(v0 <= v1 && v1 >= v0, L"Сравнение множеств некорректно!");
		}

		TEST_METHOD(SetComparatorTests)
		{
			Mycont v0;
			Assert::IsTrue(v0.key_comp()('a', 'c') && !v0.key_comp()('a', 'a'), L"Некорректный компаратор!");
			Assert::IsTrue(v0.value_comp()('a', 'c') && !v0.value_comp()('a', 'a'), L"Некорректный компаратор!");
		}

		TEST_METHOD(SetAlgTests)
		{
			char carr[] = "abc";
			const Mycont v4(carr, carr+3);

			Assert::IsTrue(*v4.find('b') == 'b');
			Assert::IsTrue(v4.count('x') == 0 && v4.count('b') == 1);
			Assert::IsTrue(*v4.lower_bound('a') == 'a', L"Метод lower_bound");
			Assert::IsTrue(*v4.upper_bound('a') == 'b', L"Метод upper_bound");
			std::pair<Mycont::const_iterator, Mycont::const_iterator> pcc = v4.equal_range('a');
			Assert::IsTrue(*pcc.first == 'a' && *pcc.second == 'b', L"Ошибка метода equal_range");
		}
	};

	TEST_CLASS(MultisetTests)
	{
		///  Тесты стандартного контейнера std::multiset, из книги "The C++ Standard template library" Плаугера, Степанова и др.
	public:

		using Myal = std::allocator<char>;
		using Mypred = std::less<char>;

		//  Для того, чтобы выполнить тестирование одного из указанных контейнеров (std::set или Binary_Tree_Search)
		//    должна быть раскомментирована одна из следующих строк:
		template<typename T> using ContainerTemplate = std::multiset<T, Mypred, Myal>;
		//template<typename T> using ContainerTemplate = Binary_Search_Tree<T, Mypred, Myal>;

		using Mycont = ContainerTemplate<char>;

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

			Mycont v3(carr, carr + 3, pred, al);
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
			Mycont::const_iterator p_cit(v4.begin());
			Mycont::reverse_iterator p_rit(v1.rbegin());
			Mycont::const_reverse_iterator p_crit(v4.rbegin());

			Assert::IsTrue(*p_it == 'a' && *--(p_it = v1.end()) == 'c', L"Декремент end() не корректен?");
			Assert::IsTrue(*p_cit == 'a' && *--(p_cit = v4.end()) == 'c', L"Декремент для const iterator на end() не корректен?");
			Assert::IsTrue(*p_rit == 'c' && *--(p_rit = v1.rend()) == 'a', L"Reverse iterator не корректен?");
			Assert::IsTrue(*p_crit == 'c' && *--(p_crit = v4.rend()) == 'a', L"Const reverse iterator не корректен?");
		}
		 


		/*TEST_METHOD(SetInsertEraseTests)
		{
			char carr[] = "abc", carr2[] = "def";
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
			Mycont::const_iterator p_cit(v4.begin());
			Mycont::reverse_iterator p_rit(v1.rbegin());
			Mycont::const_reverse_iterator p_crit(v4.rbegin());

			v0.clear();
			std::pair<Mycont::iterator, bool> pib = v0.insert('d');
			Assert::IsTrue(*pib.first == 'd' && pib.second);
			Assert::IsTrue(*--v0.end() == 'd');
			pib = v0.insert('d');
			Assert::IsTrue(*pib.first == 'd' && !pib.second);
			Assert::IsTrue(*v0.insert(v0.begin(), 'e') == 'e');
			v0.insert(carr, carr + 3);
			Assert::IsTrue(v0.size() == 5 && *v0.begin() == 'a');
			v0.insert(carr2, carr2 + 3);
			Assert::IsTrue(v0.size() == 6 && *--v0.end() == 'f');
			Assert::IsTrue(*v0.erase(v0.begin()) == 'b' && v0.size() == 5);
			Assert::IsTrue(*v0.erase(v0.begin(), ++v0.begin()) == 'c' && v0.size() == 4);
			Assert::IsTrue(v0.erase('x') == 0 && v0.erase('e') == 1);
		}

		TEST_METHOD(SetSwapAndCompTests)
		{
			char carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Mycont v1(carr, carr + 3);

			v0.clear();
			std::pair<Mycont::iterator, bool> pib = v0.insert('d');
			pib = v0.insert('d');
			v0.insert(v0.begin(), 'e');
			v0.insert(carr, carr + 3);
			v0.insert(carr2, carr2 + 3);
			v0.erase(v0.begin());
			v0.erase(v0.begin(), ++v0.begin());
			v0.erase('x');
			v0.erase('e');
			v0.clear();
			Assert::IsTrue(v0.empty());
			v0.swap(v1);
			Assert::IsTrue(!v0.empty() && v1.empty());
			std::swap(v0, v1);
			Assert::IsTrue(v0.empty() && !v1.empty());
			Assert::IsTrue(v1 == v1 && v0 < v1, L"Сравнение множеств некорректно!");
			Assert::IsTrue(v0 != v1 && v1 > v0, L"Сравнение множеств некорректно!");
			Assert::IsTrue(v0 <= v1 && v1 >= v0, L"Сравнение множеств некорректно!");
		}

		TEST_METHOD(SetComparatorTests)
		{
			Mycont v0;
			Assert::IsTrue(v0.key_comp()('a', 'c') && !v0.key_comp()('a', 'a'), L"Некорректный компаратор!");
			Assert::IsTrue(v0.value_comp()('a', 'c') && !v0.value_comp()('a', 'a'), L"Некорректный компаратор!");
		}

		TEST_METHOD(SetAlgTests)
		{
			char carr[] = "abc";
			const Mycont v4(carr, carr + 3);

			Assert::IsTrue(*v4.find('b') == 'b');
			Assert::IsTrue(v4.count('x') == 0 && v4.count('b') == 1);
			Assert::IsTrue(*v4.lower_bound('a') == 'a', L"Метод lower_bound");
			Assert::IsTrue(*v4.upper_bound('a') == 'b', L"Метод upper_bound");
			std::pair<Mycont::const_iterator, Mycont::const_iterator> pcc = v4.equal_range('a');
			Assert::IsTrue(*pcc.first == 'a' && *pcc.second == 'b', L"Ошибка метода equal_range");
		}*/
	};

}