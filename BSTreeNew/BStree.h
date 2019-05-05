#pragma once

//  Упрощённое описание шаблона двоичного дерева поиска – некоторые элементы (определения типов, проверки и проч.) пропущены
//  Может быть использовано для реализации сбалансированных деревьев поиска (RBT, AVL) и структур данных на их основе

//  Проект: https://github.com/MPuchkin/BinTree

//  Авторы: Бостан Игорь, Дуюнов Сергей, Иванченко Вячеслав

#include <iostream>
#include <cassert>
#include <queue>
#include <vector>
#include <string>
#include <iterator>
#include <memory>
#include <memory_resource>
#include <initializer_list>
#include <functional>

template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class Binary_Search_Tree
{
	//  Объект для сравнения ключей. Должен удовлетворять требованию строго слабого порядка, т.е. иметь свойства:
	//    1. Для любого x => cmp(x,x) == false (антирефлексивность)
	//    2. Если cmp(x,y) == true  =>  cmp(y,x) == false (асимметричность)
	//    3. Если cmp(x,y) == cmp(y,z) == true  =>  cmp(x,z) == true  (транзитивность)
	//    4. Если cmp(x,y) == cmp(y,z) == false  =>  cmp(x,z) == false  (транзитивность несравнимости)
	//  Этим свойством обладает, к примеру, оператор <. Если нужно использовать оператор <= , который не обладает
	//     нужными свойствами, то можно использовать его отрицание и рассматривать дерево как инвертированное от требуемого.
	Compare cmp = Compare();

	//  Узел бинарного дерева, хранит ключ, три указателя и признак nil для обозначения фиктивной вершины
	class Node
	{
	public:  //  Все поля открыты (public), т.к. само определение узла спрятано в private-части дерева
		Node* parent;
		Node* left;
		Node* right;
		//  Хранимый в узле ключ
		T data;
		//  true только для фиктивной вершины
		bool isNil;
		Node(T value = T(), Node* p = nullptr, Node* l = nullptr, Node* r = nullptr) : parent(p), data(value), left(l), right(r) {}
	};

	//  Стандартные контейнеры позволяют указать пользовательский аллокатор, который используется для
	//  выделения и освобождения памяти под узлы (реализует замену операций new/delete). К сожалению, есть 
	//  типичная проблема – при создании дерева с ключами типа T параметром шаблона традиционно указывается
	//  std::allocator<T>, который умеет выделять память под T, а нам нужен аллокатор для выделения/освобождения
	//  памяти под Node, т.е. std::allocator<Node>. Поэтому параметр шаблона аллокатора нужно изменить
	//  с T на Node, что и делается ниже. А вообще это одна из самых малополезных возможностей - обычно мы
	//  пользовательские аллокаторы не пишем, это редкость.

	//  Определяем тип аллокатора для Node (Allocator нам не подходит)
	using AllocType = typename std::allocator_traits<Allocator>::template rebind_alloc < Node >;
	//  Аллокатор для выделения памяти под объекты Node
	AllocType Alc;
	
	//  Рекурсивное клонирование дерева (не включая фиктивную вершину)
	//  Идея так себе - вроде пользуемся стандартной вставкой, хотя явное клонирование вершин было бы лучше
	void clone(Node * from, Node * other_dummy)
	{
		if (from == other_dummy)
			return;
		//	клонирование через insert? оно же бедет переразвешиваться
		// Это ещё и рекурсивный проход в ширину, выраждает дево в список
		insert(from->data);	
		clone(from->right, other_dummy);
		clone(from->left, other_dummy);
	}

public:
	using key_type = T;
	using key_compare = Compare;
	using value_compare = Compare;
	using value_type = typename T;
	using allocator_type = typename AllocType;
	using size_type = typename size_t;
	using difference_type = typename size_t;
	using pointer = typename T *;
	//using const_pointer = typename _Mybase::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type &;
	//using iterator = typename _Mybase::iterator;   //  Не нужно! Явно определили
	class iterator;   //  Предварительное объявление класса iterator, т.к. он определён ниже
	using const_iterator = iterator;
	//using reverse_iterator = typename _Mybase::reverse_iterator;
	//using const_reverse_iterator = reverse_iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	// Указательно на фиктивную вершину
	Node* dummy;

	//  Количесто элементов в дереве
	size_type tree_size = 0;

	// Создание фиктивной вершины - используется только при создании дерева
	inline Node* make_dummy()
	{
		// Выделяем память по размеру узла без конструирования
		dummy = Alc.allocate(1);
		
		//  Все поля, являющиеся указателями на узлы (left, right, parent) инициализируем и обнуляем
		std::allocator_traits<AllocType>::construct(Alc, &(dummy->parent));
		dummy->parent = dummy;

		std::allocator_traits<AllocType>::construct(Alc, &(dummy->left));
		dummy->left = dummy;

		std::allocator_traits<AllocType>::construct(Alc, &(dummy->right));
		dummy->right = dummy;
		
		dummy->isNil = true;

		//  Возвращаем указатель на созданную вершину
		return dummy;
	}

	// Создание узла дерева 
	inline Node* make_node(const value_type & elem, Node * parent, Node* left, Node* right)
	{
		// Создаём точно так же, как и фиктивную вершину, только для поля данных нужно вызвать конструктор
		Node * new_node = Alc.allocate(1);
		
		//  Все поля, являющиеся указателями на узлы (left, right, parent) инициализируем и обнуляем
		std::allocator_traits<AllocType>::construct(Alc, &(new_node->parent));
		new_node->parent = parent;

		std::allocator_traits<AllocType>::construct(Alc, &(new_node->left));
		new_node->left = left;

		std::allocator_traits<AllocType>::construct(Alc, &(new_node->right));
		new_node->right = right;

		//  Конструируем поле данных
		std::allocator_traits<AllocType>::construct(Alc, &(new_node->data));
		new_node->data = elem;
		
		new_node->isNil = false;


		//  Возвращаем указатель на созданную вершину
		return new_node;
	}

	// Удаление фиктивной вершины
	inline void delete_dummy(Node* node) {
		std::allocator_traits<AllocType>::destroy(Alc, &(node->parent));
		std::allocator_traits<AllocType>::destroy(Alc, &(node->left));
		std::allocator_traits<AllocType>::destroy(Alc, &(node->right));
		std::allocator_traits<AllocType>::deallocate(Alc, node, 1);
	}
	
	// Удаление вершины дерева
	inline void delete_node(Node * node) {
		//  Тут удаляем поле данных (вызывается деструктор), а остальное удаляем так же, как и фиктивную
		std::allocator_traits<AllocType>::destroy(Alc, &(node->data));
		delete_dummy(node);
	}

public:
	//  Предварительное объявление класса обратного итератора, чтобы некоторые операции можно было запретить уже
	//  при реализации прямого итератора
	class reverse_iterator;
	
	//  Класс итератора для дерева поиска
	class iterator 
	{
		friend class Binary_Search_Tree;
	protected:
		//  Указатель на узел дерева
		Node* data;

		explicit iterator(Node* d) : data(d) {	}
		
		//  Указатель на узел дерева
		inline Node* &_data()
		{
			return data;
		}

		//  Указывает ли итератор на фиктивную вершину?
		inline bool isNil() const {
			return data == nullptr || data->isNil;
		}

		//  Ненулевая вершина
		inline bool notNil() const {
			return !isNil();
		}

		inline void setParent(const iterator& newParent) {
			data->parent = newParent.data;
		}

		inline void setLeft(const iterator& newLeft) {
			data->left = newLeft.data;
		}

		inline void setRight(const iterator& newRight) {
			data->right = newRight.data;
		}
		//  Родительский узел дерева
		inline iterator Parent() const noexcept
		{
			return iterator(data->parent);
		}
		//  Левый дочерний узел (если отсутствует, то фиктивная вершина)
		inline iterator Left() const noexcept
		{
			return iterator(data->left);
		}
		//  Правый дочерний узел (если отсутствует, то фиктивная вершина)
		inline iterator Right() const noexcept
		{
			return iterator(data->right);
		}
		//  Является ли узел дерева левым у своего родителя
		inline bool IsLeft() const noexcept
		{
			return Parent().Left() == *this;
		}
		//  Является ли узел дерева правым у своего родителя
		inline bool IsRight() const noexcept
		{
			return Parent().Right() == *this;
		}
		//  Поиск «самого левого» элемента
		iterator GetMin() {
			iterator tmp(*this);
			while (tmp.Left().notNil())
				tmp = tmp.Left();
			return tmp;
		}
		//  Поиск «самого правого» элемента
		iterator GetMax() {
			iterator tmp(*this);
			while (tmp.Right().notNil())
				tmp = tmp.Right();
			return tmp;
		}
	public:
		//  Определяем стандартные типы в соответствии с требованиями стандарта к двунаправленным итераторам

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Binary_Search_Tree::value_type; // crap
		using difference_type = Binary_Search_Tree::difference_type;
		using pointer = Binary_Search_Tree::pointer;
		using reference = Binary_Search_Tree::reference;

		//  Значение в узле, на который указывает итератор
		inline const T& operator*() const
		{
			return data->data;
		}

		//  Преинкремент - следующий элемент множества
		iterator & operator++()
		{
			//  Если фиктивная вершина - надо вернуться на самую левую
			if (isNil()) {
				data = data->left;
				return *this;
			}
			if (Right().notNil())  //  есть правое поддерево
			{
				_data() = Right()._data();
				while (Left().notNil())
					_data() = Left()._data();
				return *this;
			}
			//  Нет правого поддерева, поэтому идём по родителям до того, как шагнём направо
			iterator prev = *this;
			_data() = Parent()._data();
			while (notNil() && Right() == prev) {
				prev = *this;
				_data() = Parent()._data();
			}

			return *this;
		}
		//  Предекремент - переход на предыдущий элемент множества
		iterator & operator--()
		{
			if (isNil()) {
				data = data->right;
				return *this;
			}
			if (Left().notNil())  //  есть левое поддерево
			{
				_data() = Left()._data();
				while (notNil())
					_data() = Right()._data();
				return *this;
			}
			//  Нет левого поддерева - идём по родителям до тех пор, пока не шагнём налево
			iterator prev = *this;
			_data() = Parent()._data();
			while (notNil() && Left() == prev) {
				prev = *this;
				_data() = Parent()._data();
			}
			return *this;
		}
		//  Постинкремент
		iterator operator++(int) {
			iterator it(*this);
			this->operator++();
			return it;
		}
		//  Постдекремент
		iterator operator--(int) {
			iterator it(*this);
			this->operator--();
			return it;
		}

		friend bool operator != (const iterator & it_1, const iterator & it_2)
		{
			return it_1.data != it_2.data;
		}

		friend bool operator == (const iterator & it_1, const iterator & it_2)
		{
			return it_1.data == it_2.data;
		}
		
		//  Эти операции не допускаются между прямыми и обратными итераторами
		const iterator & operator=(const reverse_iterator& it) = delete;
		bool operator==(const reverse_iterator& it) = delete;
		bool operator!=(const reverse_iterator& it) = delete;
		iterator(const reverse_iterator& it) = delete;
	};
	/*
	class reverse_iterator : public iterator {
		friend class Binary_Search_Tree;
	protected:
		reverse_iterator(Node * node) : iterator(node) {};
	public:
		
		//  Преинкремент - следующий элемент множества
		reverse_iterator& operator++()
		{
			iterator::operator--();
			return *this;
		}
			
		//  Предекремент - переход на предыдущий элемент множества
		reverse_iterator& operator--()
		{
			iterator::operator++();
			return *this;
		}

		//  Постинкремент
		reverse_iterator operator++(int) {
			reverse_iterator it(*this);
			this->operator++();
			return it;
		}
		//  Постдекремент
		reverse_iterator operator--(int) {
			reverse_iterator it(*this);
			this->operator--();
			return it;
		}

		friend bool operator != (const reverse_iterator& it_1, const reverse_iterator& it_2)
		{
			return it_1.data != it_2.data;
		}

		friend bool operator == (const reverse_iterator& it_1, const reverse_iterator& it_2)
		{
			return it_1.data == it_2.data;
		}

	};
	*/
	iterator begin() const	noexcept { return iterator(dummy->left);	}
	iterator end() const noexcept { return iterator(dummy);  }

	reverse_iterator rbegin() const	noexcept { return reverse_iterator(dummy->right); }
	reverse_iterator rend() const noexcept { return reverse_iterator(dummy); }


	Binary_Search_Tree(Compare comparator = Compare(), AllocType alloc = AllocType()) : dummy(make_dummy()), cmp(comparator), Alc(alloc)
	{		}

	Binary_Search_Tree(std::initializer_list<T> il) : dummy(make_dummy())
	{
		for (const auto &x : il)
			insert(x);
	}

	AllocType get_allocator() const noexcept { return Alc; }

	inline bool empty() const noexcept { return tree_size == 0; }

	template <class InputIterator>
	Binary_Search_Tree(InputIterator first, InputIterator last, Compare comparator = Compare(), AllocType alloc = AllocType()) : dummy(make_dummy()), cmp(comparator), Alc(alloc)
	{
		std::for_each(first, last, [this](T x){ insert(x);});
	}

	Binary_Search_Tree(const Binary_Search_Tree & tree) : dummy(make_dummy())
	{
		clone(tree.dummy->parent, tree.dummy);
	}


	private:
	Node* recur_copy_tree(const Node* &dest_dummy, Node* source, const Node* &source_dummy) 
	{
		//  Сначала создаём дочерние поддеревья
		Node* left_sub_tree;
		if (source->left != source_dummy)
			left_sub_tree = recur_copy_tree(dest_dummy, source->left, source_dummy);
		else
			left_sub_tree = dest_dummy;

		Node* right_sub_tree;
		if (source->right != source_dummy)
			right_sub_tree = recur_copy_tree(dest_dummy, source->right, source_dummy);
		else
			right_sub_tree = dest_dummy;
		
		//  Теперь создаём собственный узел
		Node* current = make_node(source->data, nullptr, left_sub_tree, right_sub_tree);
		//  Устанавливаем родителей
		if (source->right != source_dummy)
			current->right->parent = current;
		if (source->left != source_dummy)
			current->left->parent = current;
		//  Ну и всё, можно возвращать
		return current;
	}

	public:
	const Binary_Search_Tree & operator=(const Binary_Search_Tree & tree)
	{
		//Тут надо бы очистить уже запиленное дерево, а то мы просто переприсвоили указатели, память потекла
		if (this == &tree) return *this;
		clear();
		//  Вставку удобнее производить с помощью итератора-подсказки, или же явно рекурсией
		recur_copy_tree(dummy, tree.dummy->parent, tree.dummy);
		//  Размер задаём
		tree_size = tree.tree_size;
		//  Осталось установить min и max
		dummy->left = iterator(dummy->parent).GetMin();
		dummy->right = iterator(dummy->parent).GetMax();
		return *this;
	}

	//===============================================================================================================
	//  Это "самодельный" блок для тестирования
	
	bool CheckTree() const
	{
		return checkNodes(dummy->parent);
	}

	bool checkNodes(const Node* current_node) const {
		if (current_node == dummy) return true;
		if (current_node->parent != nullptr && current_node->parent != dummy) {
			assert(current_node->parent->left == current_node || current_node->parent->right == current_node);
			assert(current_node->parent->left != current_node || !(cmp(current_node->parent->data, current_node->data)));
			assert(current_node->parent->right != current_node || !cmp(current_node->data, current_node->parent->data));
		}
		if (current_node->left != nullptr && current_node->left != dummy) checkNodes(current_node->left);
		if (current_node->right != nullptr && current_node->right != dummy) checkNodes(current_node->right);

		return true;
	}
	
	//  Рекурсивная печать с отступами - в "нормальном" контейнере такого быть не должно
	void printNode(const Node* current, int width = 0) const {
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
		printNode(dummy->parent);
	}
	//==============================================================================================================
	
	size_type size() const { return tree_size; }

	// Обмен содержимым двух контейнеров
	swap(Binary_Search_Tree & other) {

		drfgdf
	}

	//  Вставка элемента по значению. 
	std::pair<iterator, bool> insert(const T & value)
	{
		iterator prev(dummy->parent);
		//  Дерево пустое - создаём новый узел, 
		if (prev.isNil())
		{
			//это в общем случае, здесь корень, конечно
			//parent - root; right - most right; left - most left
			//Node * new_node = new Node( value, dummy, dummy, dummy);
			Node* new_node = make_node(value, dummy, dummy, dummy);
			++tree_size;
			dummy->parent = new_node;
			dummy->left = new_node;
			dummy->right = new_node;
			return std::make_pair(iterator(new_node), true);
		}

		//  Дерево не пустое
		iterator current = iterator(dummy->parent);

		while (current.notNil()) {
			prev = current;
			if (cmp(value, *current)) {
				current = current.Left();
				continue;
			}
			if (cmp(*current, value)) {
				current = current.Right();
				continue;
			}
			//  Для set - возврат итератора на элемент, препятствующий вставке
			return std::make_pair(iterator(current), false);
		}
		
		//  Выделяем память под узел - это нужно делать по аналогии с созданием фиктивной вершины
		Node* new_node = make_node(value, prev._data(), dummy, dummy);
		++tree_size;

		if (cmp(value, *prev)) 
		{
			//  налево
			prev._data()->left = new_node;
			//  Если prev был минимальным элементом дерева
			if (dummy->left == prev._data()) dummy->left = new_node;
		}
		else
		{
			// направо		
			prev._data()->right = new_node;
			if (dummy->right == prev._data()) dummy->right = new_node;
		}
		return std::make_pair(iterator(new_node), true);
	}	

	iterator find(const value_type& value) const {
		
		iterator current = iterator(dummy->parent);

		while (current.notNil()) {

			if (cmp(value, *current)) {
				current = current.Left();
				continue;
			}
			if (cmp(*current, value)) {
				current = current.Right();
				continue;
			}
			//  Элемент найден, выход из цикла
			break;
		}
		return current;
	}

protected:
	//  Удаление листа дерева. Возвращает количество удалённых элементов
	size_type delete_leaf(iterator leaf) {
		#ifdef _DEBUG
		if (leaf.isNil()) return 0; // Стоит потом убрать, так как уже проверяем, что итератор валидный в erase
		#endif
		if (leaf.Parent().notNil()) {
			if (leaf.Parent().Right() == leaf) 
				leaf.Parent()._data()->right = dummy;
			else
				leaf.Parent()._data()->left = dummy;
		}

		if (dummy->parent == leaf._data()) {
			dummy->parent = dummy;
			dummy->right = dummy;
			dummy->left = dummy;
		}
		else 
			if (dummy->right == leaf._data()) { 
				dummy->right = leaf.Parent()._data();
			}
			else
				if (dummy->left == leaf._data()) {
					dummy->left = leaf.Parent()._data();
			}		
		//  удалить узел
		delete_node(leaf._data());
		return 1;
	}

	//  Меняет местами текущий узел и максимальный в левом поддеревею Возвращает тот же итератор, но теперь он без правого поддерева
	iterator replace_with_max_left(iterator node)
	{
		//  node имеет обоих дочерних - левое и правое поддерево, т.е. из особых вершин может быть только корнем

		//  Находим максимальный элемент слева. У него нет правого дочернего, и он не может быть корнем или самым правым
		iterator left_max = node.Left().GetMax();

		//  Рассмотрим случай, когда левый максимальный является прямым потомком node
		if (node.Left() == left_max) {
			left_max.setParent(node.Parent());
			node.setLeft(left_max.Left());
			
			if (left_max.Left().notNil())
				left_max.Left().setParent(node);
			else
				//  Если left_max был самым левым
				if (dummy->left == left_max._data())
					dummy->left = node._data();
			left_max.setLeft(node);

			//  Устанавливаем родителя для правого поддерева (оно должно быть)
			node.Right().setParent(left_max);
			//  И на него тоже
			left_max.setRight(node.Right());
			//  Обнуляем правую ссылку у node
			node._data()->right = dummy;
			//  Проверяем, был ли родитель у node
			if (node.Parent().notNil()) {
				if (node.IsRight())
					node.Parent().setRight(left_max);
				else
					node.Parent().setLeft(left_max);
			}
			else
				if (dummy->parent == node._data())
					dummy->parent = left_max._data();

			node.setParent(left_max);
			return node;
		}
		
		//  Случай когда они не являются прямыми потомками
		//  Из проблем - только node может быть корнем
		
		//  Обрабатываем родителей
		left_max.Parent().setRight(node);

		//  Если node - корень
		if (node.Parent().isNil())
			dummy->parent = left_max._data();
		else
			if (node.IsRight())
				node.Parent().setRight(left_max);
			else
				node.setLeft(left_max);
		//  Правое поддерево node
		node.Right().setParent(left_max);
		left_max.setRight(node.Right());
		node._data()->right = dummy;

		iterator tmpLeft = node.Left();

		if (left_max.Left().notNil())
			left_max.Left().setParent(node);
		node.setLeft(left_max.Left());
		
		tmpLeft.setParent(left_max);
		left_max.setLeft(tmpLeft);

		iterator tmp = node.Parent();
		node.setParent(left_max.Parent());
		left_max.setParent(tmp);
		return node;
	} 	


public:
	//  Удаление элемента, заданного итератором. Возвращает количество удалённых элементов (для set - 0/1)
	size_type erase(iterator elem) {
		//  Если фиктивный элемент, то ошибка - такого не должно происходить
		if (elem.isNil()) return 0;
		
		//  Если элемент - лист
		if (elem.Right().isNil() && elem.Left().isNil()) {
			return delete_leaf(elem);
		}

		//  Элемент не лист. Случай, когда у него один дочерний - есть только левое поддерево
		if (elem.Right().isNil()) {
			if (elem.Parent().isNil()) {  //  Удаляем корень дерева, у которого только левое поддерево
				dummy->parent = elem.Left()._data();
				elem.Left()._data()->parent = dummy;				
				dummy->right = elem.Left().GetMax()._data();
				delete_node(elem._data());
				return 1;
			}
			else {  //  Удаляем не корень, у которого только левое поддерево
				//  Меняем дочерний
				elem.Left()._data()->parent = elem.Parent()._data();
				
				//if (elem.Parent().Right() == elem) {
				if (elem.IsRight()) {				
					//  Родителя меняем
					elem.Parent()._data()->right = elem.Left()._data();
					if (elem._data() == dummy->right) // Если элемент - максимальный, то переприсваиваем
						dummy->right = elem.Left().GetMax()._data();
				}
				else {
					elem.Parent()._data()->left = elem.Left()._data();
				}

				delete_node(elem._data());
				return 1;
			}
		}

		//  Элемент не лист. Случай, когда у него один дочерний - есть только правое поддерево
		if (elem.Left().isNil()) {
			if (elem.Parent().isNil()) {  //  Удаляем корень дерева, у которого только правое поддерево
				dummy->parent = elem.Right()._data();
				elem.Right()._data()->parent = dummy;
				dummy->left = elem.Right().GetMin()._data();
				delete_node(elem._data());
				return 1;
			}
			else {  //  Удаляем не корень, у которого только правое поддерево
				//  Меняем дочерний
				elem.Right()._data()->parent = elem.Parent()._data();
				//if (elem.Parent().Right() == elem) {
				if (elem.IsRight()) {				
					//  Родителя меняем
					elem.Parent()._data()->right = elem.Right()._data();
				}
				else {
					elem.Parent()._data()->left = elem.Right()._data();
					if (elem._data() == dummy->left)
						dummy->left = elem.Right().GetMin()._data();
				}

				delete_node(elem._data());
				return 1;
			}
		}
		//  случай когда есть оба дочерних поддерева		
		// Вообще можно и в случаях с одним поддеревом использовать swap

		replace_with_max_left(elem);

		return erase(elem);
	}
	
	size_type erase(const value_type& elem) {
		return erase(find(elem));
	}

	

	//Если передавать по ссылкам,все хорошо. Конструктор копий принескольких деревьях ломается.
	friend bool operator== (const Binary_Search_Tree<T> &tree_1, const Binary_Search_Tree<T> & tree_2)
	{
		auto i = tree_1.begin(), ii = tree_2.begin();
		for (; (i != tree_1.end()) && (ii != tree_2.end()); ++i, ++ii)
		{
			if (*i != *ii)
				return false;
		}
		return i == tree_1.end() && ii == tree_2.end();
	}

	//  Очистка дерева (без удаления фиктивной вершины)
	void clear() {
		Free_nodes(dummy->parent);
		tree_size = 0;
		dummy->parent = dummy->left = dummy->right = dummy;
	}

private:
	//  Рекурсивное удаление узлов дерева, не включая фиктивную вершину
	void Free_nodes(Node* node)
	{ 
		if (node != dummy)
		{
			Free_nodes(node->left);
			Free_nodes(node->right);
			delete_node(node);
		}
	}
	
public:
	~Binary_Search_Tree()
	{
		clear(); // рекурсивный деструктор
		delete_dummy(dummy);
	}
};