#pragma once
#include <iostream>
#include <cassert>
#include <queue>
#include <vector>
#include <string>


template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class Binary_Tree_Search
{
	//Могу писать
	Compare cmp;

	class Tree_Node
	{
	public:

		Tree_Node* parent;
		Tree_Node* left;
		Tree_Node* right;
		
		T data;
		Tree_Node(T value, Tree_Node* p, Tree_Node* l, Tree_Node* r) : parent(p), data(value), left(l), right(r) {}
	};
		
	std::allocator<Tree_Node> alc;
	
	//  Указатель на корень дерева
	Tree_Node* root, *dummy;

	/// Создание фиктивной вершины
	Tree_Node* make_dummy() {
		
		/// Всё неправильно!!!!
		
		void* place = operator new(sizeof(Tree_Node<T>));
		dummy = static_cast<Tree_Node *>(place);
		
		dummy->parent = nullptr;
		dummy->left = nullptr;
		dummy->right = nullptr;

		return dummy;
	}
	
	/// Удаление фиктивной вершины
	void delete_dummy() {

		/// Всё плохо!
		operator delete(static_cast<Tree_Node *>(dummy), sizeof(Node));
	}


	int64_t length = 0;
public:

	class iterator
	{
		Tree_Node<T1>* data;

	public:
		iterator(Tree_Node<T1>* d) : data(d)
		{

		}

		Tree_Node<T1>* _data()
		{
			return data;
		}

		Tree_Node<T1>* _parent()
		{
			return data->parent;
		}

		Tree_Node<T1>* _left()
		{

			return data->left;
		}

		Tree_Node<T1>* _right()
		{
			return data->right;
		}

		Tree_Node<T1>* _parent_right()
		{
			return data->parent->right;
		}

		T& operator*()
		{
			return data->data;
		}

		iterator<T1>& _To_parent()
		{
			data = data->parent;
			return *this;
		}

		iterator<T1>& _Toleft()
		{

			data = data->left;
			return *this;
		}

		iterator<T1>& _Toright()
		{
			data = data->right;
			return *this;
		}

		//3.6
		void operator++()
		{
			if (_right() != nullptr)
			{
				if (_right()->right == data)
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
		}

		//3.7
		void operator--()
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
		}

		void operator+(int64_t num)
		{
			for (size_t i = 0; i < num; i++)
			{
				data = data->right;
			}
		}

		void operator-(int64_t num)
		{
			for (size_t i = 0; i < num; i++)
			{
				data = data->left;
			}
		}

		friend bool operator != (iterator<T1> it_1, iterator<T1> it_2)
		{
			return it_1.data != it_2.data;
		}

		friend bool operator == (iterator<T1> it_1, iterator<T1> it_2)
		{
			return it_1.data == it_2.data;
		}

		~iterator()
		{
		}

	private:

	};

	iterator<T> begin()
	{
		return iterator<T>(begin_data);
	}

	iterator<T> end()
	{
		return iterator<T>(end_data);
	}

	iterator<T> rbegin()
	{
		return iterator<T>(end_data->right);
	}

	iterator<T> rend()
	{
		return iterator<T>(end_data);
	}

	Binary_Tree_Search()
	{
		root = nullptr;
		begin_data = nullptr;
	}

	Binary_Tree_Search(std::vector<T>& v)
	{
		root = nullptr;
		begin_data = nullptr;
		for (auto x : v)
		{
			this->Add_Leaf(x);
		}
	}

	Binary_Tree_Search(Binary_Tree_Search<T>& tree)
	{
		root = nullptr;
		begin_data = nullptr;
		for (auto i = tree.begin(); i != tree.end(); ++i)
		{
			Add_Leaf(*i);
		}
	}
	//===============================================================================================================
	bool CheckTree() const
	{
		return checkNodes(root);
	}

	bool checkNodes(const Tree_Node<T>* current_node) const {
		if (current_node->parent != nullptr) {
			assert(current_node->parent->left == current_node || current_node->parent->right == current_node);
			assert(current_node->parent->left != current_node || current_node->data <= current_node->parent->data);
			assert(current_node->parent->right != current_node || current_node->data >= current_node->parent->data);
		}
		if (current_node->left != nullptr && current_node->left != end_data) checkNodes(current_node->left);
		if (current_node->right != nullptr && current_node->right != end_data) checkNodes(current_node->right);

		return true;
	}

	void printNode(const Tree_Node<T>* current, int width = 0) const {
		std::string spaces = "";
		for (int i = 0; i < width; ++i) spaces += "  ";
		if (current == nullptr) {
			std::cout << spaces << "NIL\n";
			return;
		}
		if (current == end_data) {
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
	void Add_Leaf(T data)
	{
		if (root == nullptr)
		{
			root = new Tree_Node<T>(nullptr, data, nullptr, nullptr);
			begin_data = root;
			root->right = end_data;
		}
		else
		{
			bool pure_left = true;
			bool pure_right = true;

			auto p = iterator<T>(root);

			while (p._data() != nullptr)
			{
				if (*p > data) // left
				{
					if (p._left() == nullptr || p._left() == end_data)
					{
						p._data()->left = new Tree_Node<T>(p._data(), data, nullptr, nullptr);
						++length;
						if (pure_left)
						{
							begin_data = p._data()->left;
							p._data()->left->left = end_data;
							end_data->left = p._data()->left;
						}
						break;
					}
					else
					{
						pure_right = false;
						p._Toleft();
					}
				}
				else //right
				{
					if (p._right() == nullptr || p._right() == end_data)
					{
						p._data()->right = new Tree_Node<T>(p._data(), data, nullptr, nullptr);
						++length;
						if (pure_right)
						{
							p._data()->right->right = end_data;
							end_data->right = p._data()->right;
						}
						break;
					}
					else
					{
						pure_left = false;
						p._Toright();
					}
				}

			}
		}
	}

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
	}

	void Destroy_Node(Tree_Node<T>* node)
	{ //иногда тут вылезает stackoverflow
		if (node != nullptr && node != end_data)
		{
			Destroy_Node(node->left);
			Destroy_Node(node->right);
			delete node;
		}
	}

	~Binary_Tree_Search()
	{
		Destroy_Node(root); // рекурсивный деструктор
		operator delete(static_cast<void *>(place), sizeof(Tree_Node<T>));



		/*auto p = root;
		std::queue<Tree_Node<T>*> del;
		del.push(p);
		while (!del.empty()) //больше всего ошибка проявляется тут
		{
			std::queue<Tree_Node<T>*> new_del;
			while (!del.empty())
			{
				auto x = del.front();
				del.pop();

				if (x->left != nullptr)
				{
					if (x->left != end_data)
					{
						new_del.push(x->left);
					}
				}
				if (x->right != nullptr)
				{
					if (x->right != end_data)
					{
						new_del.push(x->right);
					}
				}
				delete x;
			}
			del = new_del;
		}
		delete end_data; //ошибка иногда возникает тут*/
	}
};