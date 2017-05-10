// Statische Bibliothek.
#pragma comment(lib, "tree.lib")

#include <iostream>
#include "node.h"

using namespace std;

int main()
{
	// TESTBEREICH: ==============================

	// -------------------------------------------

	// 1.1.2 ; 1.1.3 Debugging;
	node* root = new node("root");
	root->add_child(new node("left child"));
	root->add_child(new node("right child"));

	// 1.2.1 Globale Knotenzählung mit node::node_id

	// 1.2.2 Rekursive Baumerstellung
	//  mit create_complete_tree( nr_child_nodes, tree_depth)
	delete root;
	root = create_complete_tree(2, 4);
	// Teste Ausgabe:
	cout << *root;
	cout << "Enter print()" << endl;
	root->print(cout);
	root->print_norec(cout);
	root->print_norec_nocycle(cout);


	// ERSTELLE BAUM MIT ZYKLUS:
	delete root;
	root = NULL;
	node* rootcyc = create_complete_tree(2, 4);
	node* child = rootcyc;
	for (int i = 0; i <= 2; i++)
	{
		child = const_cast<node*> (child->get_child(1));
		child->add_child(rootcyc);
	}
	rootcyc->add_child(rootcyc);
	// Teste Ausgabe:
	cout << "Enter print_nocycle()" << endl;
	rootcyc->print_nocycle(cout);
	rootcyc->print_norec_nocycle(cout);

	// node~ ist NICHT auf zyklischen Baum vorbereitet -> Laufzeitfehler.
	//  xxx  delete rootcyc; rootcyc = NULL;
	cout << "// Main" << endl;
	return 0;
}
