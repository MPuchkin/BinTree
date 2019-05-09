#include <iostream>
#include <set>
#include "BStree.h"
#include <iterator>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

template<typename InputIter>
void print(InputIter first, InputIter last) {

	if (std::is_same<iterator_traits<InputIter>::iterator_category, std::random_access_iterator_tag>::value) {
		cout << "Random access iterator range : ";
		while (first != last)
			cout << *first++ << " ";
		cout << endl;
	}
	else {
		cout << "Any input iterator range : ";
		while (first != last)
			cout << *first++ << " ";
		cout << endl;
	}
}

int main() {

	const size_t sz = 15;
	vector<int> v;
	v.reserve(sz);
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dis(0,+1000);
	generate_n(back_inserter(v), sz, [&]() {return (dis(gen) % 5000); });
	sort(v.begin(), v.end());
	cout << "\n -------------------------------- \n";
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	cout << " -------------------------------- \n";
	Binary_Search_Tree<int> bb(v.begin(), v.end());
	bb.PrintTree();
	cout << " -------------------------------- \n";


	/*
	Binary_Search_Tree<int> Tree = { 40,50,30,35,10,75,23,87,68 };
	Tree.PrintTree();
	cout << "=========================\n";
	for (auto it = Tree.begin(); it != Tree.end(); ++it)
		cout << *it << " ";
	cout << endl;
	cout << "First : " << *Tree.begin() << endl;
	cout << " Last : " << *(--Tree.end()) << endl;
	Tree.erase(10);
	Tree.PrintTree();
	cout << "First : " << *Tree.begin() << endl;
	cout << " Last : " << *(--Tree.end()) << endl;
	Tree.erase(40);
	cout << "=========================\n";
	Tree.PrintTree();
	cout << "First : " << *Tree.begin() << endl;
	cout << " Last : " << *(--Tree.end()) << endl;
	Binary_Search_Tree<int>::iterator it = Tree.find(23);
	Tree.insert(it, 111);
	Tree.PrintTree();
	*/
	#ifdef _WIN32
		system("pause");
	#endif //_WIN32
	return 0;
}