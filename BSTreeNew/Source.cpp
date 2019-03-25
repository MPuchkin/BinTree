#include <iostream>
#include <set>

#include "BStree.h"

using namespace std;

int main() {
	Binary_Search_Tree<int> Tree = {40,50,30,35,10,75,23,87,68};
	Tree.PrintTree();
	#ifdef _WIN32
		system("pause");
	#endif //_WIN32
	return 0;
}