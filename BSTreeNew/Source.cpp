#include <iostream>
#include <set>
#include "BStree.h"

using namespace std;

int main() {
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
	
	#ifdef _WIN32
		system("pause");
	#endif //_WIN32
	return 0;
}