#pragma once
#include <iostream>
#include <cassert>
#include <queue>
#include <vector>
#include <string>
#include <memory_resource>
#include <initializer_list>
#include <functional>

template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class Binary_Tree_Search
{
	//Объект для сравнения значений
	Compare cmp = Compare();

	class Tree_Node
	{
	public:

		Tree_Node* parent;
		Tree_Node* left;
		Tree_Node* right;

		T data;
		Tree_Node(T value = T(), Tree_Node* p = nullptr, Tree_Node* l = nullptr, Tree_Node* r = nullptr) : parent(p), data(value), left(l), right(r) {}
	};

	//  Жуть из стандартной библиотеки
	using AllocType = typename std::allocator_traits<Allocator>::template rebind_alloc < Tree_Node >;
	AllocType Alc;

	void clone(Tree_Node * from, Tree_Node * other_dummy)
	{
		if (from == nullptr || from == other_dummy)
			return;
		add(from->data);
		clone(from->right, other_dummy);
		clone(from->left, other_dummy);
	}

public:
	using key_type = T;
	using key_compare = Compare;
	//using value_compare = typename _Mybase::value_compare;
	using value_type = typename T;
	using allocator_type = typename AllocType;
	using size_type = typename size_t;//Надо одинаково делать, иначе переполнение )
	using difference_type = typename int64_t;
	//using pointer = typename _Mybase::pointer;
	//using const_pointer = typename _Mybase::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;
	//using iterator = typename _Mybase::iterator;   //  Не нужно! Явно определили
	//using const_iterator = typename _Mybase::const_iterator;
	//using reverse_iterator = typename _Mybase::reverse_iterator;
	//using const_reverse_iterator = typename _Mybase::const_reverse_iterator;

	//Вижу, пофиксили))


private:
	//  Указатель на корень дерева
	Tree_Node* root;

	// Указательно на фиктивную вершину
	Tree_Node* dummy;

	/// Создание фиктивной вершины
	Tree_Node* make_dummy()
	{
		/// Выделяем память без конструирования
		dummy = Alc.allocate(1);

		allocator_traits<AllocType>::construct(Alc, dummy->parent);
		dummy->parent = nullptr;

		allocator_traits<AllocType>::construct(Alc, dummy->left);
		dummy->left = nullptr;

		allocator_traits<AllocType>::construct(Alc, dummy->right);
		dummy->right = nullptr;

		return dummy;
	}

	/// Удаление фиктивной вершины
	void delete_dummy() {
		allocator_traits<AllocType>::destroy(Alc, dummy->parent);
		allocator_traits<AllocType>::destroy(Alc, dummy->left);
		allocator_traits<AllocType>::destroy(Alc, dummy->right);
		allocator_traits<AllocType>::deallocate(Alc, dummy, 1);
	}

	int64_t length = 0;
public:

	//  Класс итератора для дерева поиска
	class iterator
	{
		//У меня вызывает сомнения типизация
		friend class Binary_Tree_Search<T>;
	protected:
		Tree_Node* data;

		explicit iterator(Tree_Node* d) : data(d)
		{
		}
		
		Tree_Node* _data()//Что за Т1? Зачем?
		{
			return data;
		}

		Tree_Node* _parent()
		{
			return data->parent;
		}

		Tree_Node* _left()
		{

			return data->left;
		}

		Tree_Node* _right()
		{
			return data->right;
		}

		Tree_Node* _parent_right()
		{
			return data->parent->right;
		}

		T& operator*()
		{
			return data->data;
		}

	public:

		const T& operator*()
		{
			return data->data;
		}

		iterator & _To_parent()
		{
			data = data->parent;
			return *this;
		}

		iterator & _Toleft()
		{

			data = data->left;
			return *this;
		}

		iterator & _Toright()
		{
			data = data->right;
			return *this;
		}

		//3.6
		iterator & operator++()
		{
			if (_right() != nullptr)
			{
				if (_right()->right == data)
					//Немного глаза режут дополнительные блоки там, где их можно опустить(
				{
					_Toright();
				}
				else
				{
					_Toright();
					while (_left() != nullptr)
					{
						_Toleft();
					}
				}
			}
			else
			{
				while (_parent()->right == data)
				{
					_To_parent();
				}
				_To_parent();
			}
			return *this;
		}

		//3.7
		iterator & operator--()
		{
			if (_left() != nullptr)
			{
				if (_left()->left == data)
				{
					_Toleft();
				}
				else
				{
					_Toleft();
					while (_right() != nullptr)
					{
						_Toright();
					}
				}
			}
			else
			{
				while (_parent()->left == data)
				{
					_To_parent();
				}
				_To_parent();
			}
			return *this;
		}

		friend bool operator != (const iterator & it_1, const iterator & it_2)
		{
			return it_1.data != it_2.data;
		}

		friend bool operator == (const iterator & it_1, const iterator & it_2)
		{
			return it_1.data == it_2.data;
		}
	};

	iterator begin() const
	{
		if(dummy->left == nullptr) return iterator(dummy);
		return iterator(dummy->right);
	}

	iterator end() const
	{
		return iterator(dummy);
	}

	Binary_Tree_Search() : root(nullptr)
	{
		make_dummy();
	}

	//инишиалайзер лист бы и дипозоном да, два варианта
	Binary_Tree_Search(std::initializer_list<T>& il)//Его бы, наверно, диапазоном инициализировать, чтобы любой контейнер запихать можно было
	{
		make_dummy();
		root = nullptr;
		for (auto x : il)
		{
			this->Add_Leaf(x);
		}
	}
	template <class InputIterator>
	Binary_Tree_Search(InputIterator first, InputIterator last)
	{
		make_dummy();
		std::for_each(first, last, [this](T x){ Add_Leaf(x);});
	}

	Binary_Tree_Search(const Binary_Tree_Search & tree)
	{
		//Это я спёр из своего кода, оно работает. По крайней мере, у меня
		root = nullptr;
		make_dummy();
		clone(tree.root, tree.dummy);
	}

	const Binary_Tree_Search & operator=(const Binary_Tree_Search & tree)
	{
		//Тут надо бы очистить уже запиленное дерево, а то мы просто переприсвоили указатели, память потекла
		if (this == &tree) return *this;
		for (auto i = tree.begin(); i != tree.end(); ++i)
		{
			Add_Leaf(*i);   ///  Кошмар!!!
		}
	}

	//===============================================================================================================
	bool CheckTree() const
	{
		return checkNodes(root);
	}

	bool checkNodes(const Tree_Node* current_node) const {
		if (current_node->parent != nullptr) {
			assert(current_node->parent->left == current_node || current_node->parent->right == current_node);
			assert(current_node->parent->left != current_node || !(cmp(current_node->parent->data, current_node->data)));
			assert(current_node->parent->right != current_node || !cmp(current_node->data, current_node->parent->data));
		}
		if (current_node->left != nullptr && current_node->left != dummy) checkNodes(current_node->left);
		if (current_node->right != nullptr && current_node->right != dummy) checkNodes(current_node->right);

		return true;
	}

	void printNode(const Tree_Node* current, int width = 0) const {
		std::string spaces = "";
		for (int i = 0; i < width; ++i) spaces += "  ";
		if (current == nullptr) {
			std::cout << spaces << "NIL\n";
			return;
		}
		if (current == dummy) {
			std::cout << spaces << "Dummy\n";
			return;
		}
		if (current->right != nullptr) printNode(current->right, width + 3);
		std::cout << spaces << current->data << std::endl;
		if (current->left != nullptr) printNode(current->left, width + 3);
	}

	void PrintTree() const {
		printNode(root);
	}






	//3.1
	iterator Add_Leaf(const T & value)
	{
		if (root == nullptr)
		{
			//это в общем случае, здесь корень, конечно
			//parent - most right; right - most left; left - root
			root = new Tree_Node(nullptr, value, nullptr, nullptr);
			root->right = dummy;
			dummy->parent = root;
			dummy->left = root;
			dummy->right = root;
		}
		else
		{
			iterator prev(nullptr);
			iterator current = iterator(root);
			while (current._data() != nullptr && current._data() != dummy) {
				prev = current;
				if (cmp(value, *current))
					current._Toleft();
				else
					current._Toright();
			}

			//  Выделяем память под узел
			auto tmp = new Tree_Node(prev._data(), value, nullptr, nullptr);

			if (cmp(value, *prev)) 
			{
				//  налево
				prev._data()->left = tmp;
			}
			else
			{
				// направо
				prev._data()->right = tmp;
			}
			
			//  Если новый узел - максимальный?
			if (dummy->parent == prev) {
				//  Если новый узел - максимальный 
				dummy->parent = tmp;
				tmp->right = dummy;
			}
			else
				//  Если новый узел - минимальный
				if (dummy->left == prev)
					dummy->left = tmp;
		}
	}
	/*
	void Add_Leaf(Tree_Node<T>* data, iterator<T> it_begin)
	{
		if (root == nullptr)
		{
			root = data;
		}
		else
		{
			auto p = it_begin;

			while (true)
			{
				if (*p > data->data) // left
				{

					if (p._left() == nullptr)
					{
						data->parent = p._data();
						p._data()->left = data;
						++length;
						break;
					}
					else
					{
						p._Toleft();
					}
				}
				else //right
				{
					if (p._right() == nullptr)
					{
						data->parent = p._data();
						p._data()->right = data;
						++length;
						break;
					}
					else
					{
						p._Toright();
					}
				}

			}
		}
	}

	//3.2
	bool Is_Item_In_BTS(T data)
	{
		auto p = root;
		while (p != nullptr && p != end_data)
		{
			if (p->data == data)
			{
				return true;
			}
			else if (p->data > data)
			{
				p = p->left;

			}
			else
			{
				p = p->right;
			}

		}
		return false;
	}
	*/
	/*
	//3.3
	iterator<T> Find_Min()
	{
		return iterator<T>(begin_data);
	}

	//3.3
	iterator<T> Find_Max()
	{
		return iterator<T>(end_data->right);
	}

	//3.4
	iterator<T> Find_Less_Or_Equal(T data)
	{
		auto p = iterator<T>(root);

		while (p != nullptr && p != end_data)
		{
			if (*p <= data)
				return p;
			else --p;

		}
		return end();
	}

	//3.4
	iterator<T> Find_Grater_Or_Equal(T data)
	{
		auto p = iterator<T>(root);

		while (p != nullptr && p != end_data)
		{
			if (*p >= data)
				return p;
			else ++p;

		}
		return end();
	}

	iterator<T> Find_Equal(T& data)
	{
		auto p = iterator<T>(root);

		while (p != nullptr && p != end_data)
		{
			if (*p == data)
			{
				return p;
			}
			if (*p > data) --p;
			else ++p;

		}
		return end();
	}

	//3.5
	void Delete_Leaf(T& data)
	{
		auto p = Find_Equal(data);
		if (p != end())
		{
			auto p_old = p._data();

			if (p_old == root)
			{
				Tree_Node<T>* pp = p._right();

				if (p_old->left == p_old)
				{
					root = p_old->left;
				}
				else
				{
					root = p_old->left;
				}
				this->Add_Leaf(pp, p_old->left);
			}
			else if (p._left() != end() && p._right() != end() && p._left() != nullptr && p._right() != nullptr)
			{
				Tree_Node<T>* pp = p._right();

				if (p_old->parent->left == p_old)
				{
					p_old->parent->left = p_old->left;
					p_old->left->parent = p_old->parent;
				}
				else
				{
					p_old->parent->right = p_old->left;
					p_old->left->parent = p_old->parent;
				}
				this->Add_Leaf(pp, p_old->left);
			}
			else if (p._left() != end() && p._left() != nullptr)
			{
				if (p_old->parent->left == p_old)
				{
					p_old->parent->left = p_old->left;
					p_old->left->parent = p_old->parent;
				}
				else
				{
					p_old->parent->right = p_old->left;
					p_old->left->parent = p_old->parent;
				}

				if (p_old->right == end_data)
				{
					auto end = p_old->parent;
					while (end->right != nullptr)
					{
						end = end->right;
					}
					end_data->right = end;
					end->right = end_data;
				}
			}
			else if (p._right() != end() && p._right() != nullptr)
			{
				if (p_old->parent->left == p_old)
				{
					p_old->parent->left = p_old->right;
					p_old->right->parent = p_old->parent;
				}
				else
				{
					p_old->parent->right = p_old->right;
					p_old->right->parent = p_old->parent;
				}

				if (p_old == begin_data)
				{

					auto begin = p_old->parent;
					while (begin->left != nullptr)
					{
						begin = begin->left;
					}
					begin_data = begin;
					end_data->left = begin;
					begin->left = end_data;
				}
			}
			else
			{
				if (p_old == begin_data || p_old->right == end_data)
				{
					if (p_old->parent->left == p_old)
					{
						p_old->parent->left = end_data;
						begin_data = p_old->parent;
						end_data->left = p_old->parent;
					}
					else
					{
						p_old->parent->right = end_data;
						end_data->right = p_old->parent;
					}
				}
				else
				{
					if (p_old->parent->left == p_old)
					{
						p_old->parent->left = nullptr;
					}
					else
					{
						p_old->parent->right = nullptr;
					}
				}
			}
			delete p_old;
		}
	}

	//3.8
	void Cout_Tree_Witdh()
	{
		auto p = iterator<T>(root);
		auto _spacer = length;
		std::queue<iterator<T>> print;
		print.push(p);
		while (!print.empty())
		{
			for (auto i = 0; i < _spacer; i++)
			{
				std::cout << " ";
			}
			std::queue<iterator<T>> new_print;
			while (!print.empty())
			{
				auto x = print.front();
				print.pop();
				if (x._parent() != nullptr && x._parent()->right == x && x._parent()->left == nullptr)
				{
					std::cout << "  ";
					_spacer += 2;
				}
				std::cout << *x;
				if (x._left() != nullptr)
				{
					if (x._left() != end_data)
					{
						new_print.push(iterator<T>(x._left()));
					}
				}
				if (x._right() != nullptr)
				{
					if (x._right() != end_data)
					{
						new_print.push(iterator<T>(x._right()));
						std::cout << "  ";
					}
				}
				std::
					cout << " ";
			}
			std::cout << std::endl;
			--_spacer;
			print = new_print;
		}
	}

	//3.9 //Если передавать по ссылкам,все хорошо. Конструктор копий принескольких деревьях ломается.
	friend bool operator== (Binary_Tree_Search<T> tree_1, Binary_Tree_Search<T> tree_2)
	{
		for (auto i = tree_1.begin(), ii = tree_2.begin(); (i != tree_1.end()) && (ii != tree_2.end()); ++i, ++ii)
		{
			if (*i != *ii)
				return false;
		}
		return true;
	}

	//3.10
	iterator<T> Find_Next_For_Iterator(iterator<T> it)
	{
		if (it._right() != nullptr)
		{
			++it;
		}
		else if (it._left() != nullptr)
		{
			--it;
		}
		else
		{
			it._To_parent();
		}
		return it;
	}*/

	void Destroy_Node(Tree_Node* node)
	{ 
		if (node != nullptr && node != dummy)
		{
			Destroy_Node(node->left);
			Destroy_Node(node->right);
			delete node;
		}
	}

	~Binary_Tree_Search()
	{
		Destroy_Node(root); // рекурсивный деструктор
		delete_dummy();
	}
};