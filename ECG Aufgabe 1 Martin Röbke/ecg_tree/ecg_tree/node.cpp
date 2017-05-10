#include "node.h"
#include <cassert>                      // Assert
#include <sstream>                      // Stringstream


/** Standard C++-File für die Klasse 'node'.
* Einführung in die Computergrafik Aufgabe 1
* 11.05.2017
* Autor: Martin Röbke
*/

#define TEST_EIN                        // Debug-Makros
#ifdef TEST_EIN
#define PRINT2(X, Y) cout << X << Y << endl;
#else
#define PRINT2(X, Y) /*nothing*/
#endif // TEST_EIN


using namespace std;                    // Std-Namensraum


										// Initialisierung der klassenspezifischen Variablen:
int     node::node_id = 0;

// --------FUNKTIONALITÄTEN:---------------

int node::get_node_id()
{
	return node_id;
}

string node::get_name() const
{
	return name;
}

void node::set_name(const string& new_name)
{
	name = new_name;
}

unsigned int node::get_nr_children() const
{
	return v_children.size();
}

/** \brief Gibt einen Zeiger auf den i-ten direkten Kindknoten zurück.
NULL wenn nicht vorhanden.
*
* \param i : 0 <= i < v_children.size()
* \return const node* i-ter Kindknoten.
*
*/
node* node::get_child(const unsigned int position) const
{
	if (position<v_children.size())
	{
		return v_children[position];
	}
	// Problemfall / Nicht gefunden:
	return NULL;
}

/** \brief Registriert ein Kindknoten am Ende des Vektors der Node.
*
* \param child const node*
* \return void
*
*/
void node::add_child(const node* child)
{
	v_children.push_back(const_cast<node*>(child));
}

// ----------Ostream Methods:---------------
/** \brief Schreibe den Baum mit Hilfe von whitespace
und Zeilenumbrüchen in gegebenen out-stream.
*
* \param str ostream&
* \param depth unsigned int
* \param intent unsigned int
* \param ws const char
* \return void
*
*/
void node::print(ostream& str, unsigned int depth, unsigned int intent, const char ws) const
{
	const string r(intent * depth, ws); // Erstelle WhiteSpace

	str << r << name << endl;           // Aktueller Knoten
	for (auto c : v_children)             // Seine Kinder
	{
		c->print(str, depth + 1);
	}
}

/** \brief Traversierung, die Zyklen detektiert, markiert ausgibt
und eine unendliche Rekursion vermeidet.
*
* \param std::ostream&
Stream, in den die Ausgabe gegeben wird.
* \param depth unsigned int = 0
Aktuelle Tiefe im Baum.
* \param p_myset unordered_set<node*> * = NULL
Set
* \param intent unsigned int = 3
Einrückung durch 'intent' Zeichen.
* \param ws const char = ' '
Zur Absetzung genutzter Whitespace.
* \return void
*
*/
void node::print_nocycle(ostream& str, unsigned int depth, unordered_set<node*> * p_myset,
	unsigned int intent, const char ws) const
{
	bool pointer_constr = false;
	const string r(intent * depth, ws); // Erstelle WhiteSpace
	const string zyklus("||>");        // Markierung Zyklus
	if (!p_myset)                        // Wenn Set noch nicht erstellt -> Erstellen.
	{
		// Leeres Set Konstruieren.
		p_myset = new unordered_set<node*>;
		pointer_constr = true;
	}
	// Selbst besucht:
	p_myset->insert(const_cast<node*>(this));  // Trage selbst ein.

	str << r << name << endl;           // Ausgabe in Stream.
										// Liste der Kinder:
	for (auto c : v_children)
	{
		if (p_myset->count(c)>0)         // Kind bereits besucht:
		{
			// Ausgabe Zyklus. -> Eins eingeschoben.
			str << string(intent*(depth + 1), ws) << zyklus << c->get_name() << endl;
			continue;                   // Schleife weiter!!
		}
		else                            // Kind nicht besucht:
		{
			c->print_nocycle(str, depth + 1, p_myset);
		}
	}
	// Speicher Aufräumen.
	if (pointer_constr)
	{
		delete p_myset;
		PRINT2("print_nocycle: Deleted set with ", to_string(sizeof(*p_myset)) + " Bytes\n");
	}
}

/** \brief Iterative Traversierungsmethode, die dieselbe Ausgabe erzeugt, jedoch
ohne Rekursion auskommt. Legt dazu einen Stack an, der die zu bearbeitenden Elemente
enthält.

*
* \param str std::ostream&
* \param intent unsigned int
* \param ws const char
* \return void
*
*/
void node::print_norec(ostream& str, unsigned int intent, const char ws) const
{
	PRINT2("Enter print_norec:", "\n");
	stack<node*> mystack;                   // Initialisierung
	stack<int> n_stack;                     // Notiere abzuarbeitende Knoten
	node* current;
	int depth = 0;
	int i = 0;
	const string r(intent, ws);             // Erstelle EINEN WhiteSpace

	mystack.push(const_cast<node*>(this));  // Root pushen.
	n_stack.push(1);                        // Nodes auf aktueller Ebene.

	while (!mystack.empty())
	{
		current = mystack.top();
		mystack.pop();
		n_stack.top() -= 1;                  // Aktuelles Abhängen
											 // Ausgabeblock:
		for (i = 1; i <= depth; i++)
		{
			str << r;                       // Whitespace
		}
		str << current->get_name() << endl;     // Name, endl.
												// Kinder anhängen -> umgekehrte Reihenfolge
		if (current->v_children.empty())        // Aktuell Blattknoten
		{
			while ((!n_stack.empty()) && n_stack.top() == 0)
			{
				--depth;                // Aktuelle Tiefe um 1 verringern
				n_stack.pop();
			}

		}
		else                            // Aktuell nicht Blattknoten
		{
			++depth;                    // Aktuelle Tiefe um 1 erhöhen.
			n_stack.push(current->v_children.size());

			for (i = current->v_children.size() - 1; i >= 0; i--)
			{
				mystack.push(current->v_children[i]);
			}
		}                              // Alle Kinder auf Stack gelegt.
	}
	PRINT2("Leaving print_norec", "");
}


/** \brief Iterative Traversierungsmethode, die dieselbe Ausgabe erzeugt, jedoch
ohne Rekursion auskommt. Legt dazu einen Stack an, der die zu bearbeitenden Elemente
enthält.
Zusätzlich: Die Vermeidung von unendlichen Zyklen erreicht mittels std::unordered_set<node*>

*
* \param str std::ostream&
* \param intent unsigned int
* \param ws const char
* \return void
*
*/
void node::print_norec_nocycle(std::ostream& str, unsigned int intent, const char ws) const
{
	PRINT2("Enter print_norec_nocycle:", "\n");
	stack<node*> st_search;                   // Initialisierung
	stack<int> st_depth;                      // Hält #(zu bearbeitende Knoten) jeder Tiefe
	unordered_set<node*> set_nodes;           // Set besuchter Nodes um Zyklen zu umgehen
	node* current;
	int depth = 0;
	int i = 0;

	const string r(intent, ws);             // Erstelle EINEN WhiteSpace
	const string zyklus("|>");              // Ausgabe Zyklus

	st_search.push(const_cast<node*>(this));         // Root pushen.
	st_depth.push(1);                       // Nodes auf oberster Ebene.

	while (!st_search.empty())
	{
		current = st_search.top();
		st_search.pop();
		st_depth.top() -= 1;                 // Aktuelles wegnehmen
											 // ========================
											 // ===Wenn bereits gefunden: Nur Ausgabe und weiter:
											 // ========================
		if (set_nodes.count(current)>0)      // Knoten bereits besucht:
		{
			// Ausgabeblock:
			for (i = 1; i <= depth; i++)
			{
				str << r;                   // Whitespace
			}
			str << zyklus << current->get_name() << endl;       // Name, endl.
			while ((!st_depth.empty()) && st_depth.top() == 0)
			{
				--depth;                    // Aktuelle Tiefe um 1 verringern
				st_depth.pop();
			}
			continue;
		}
		// ========================
		// ===Noch nicht gefunden:=
		// ========================
		set_nodes.insert(current);
		// Ausgabeblock:
		for (i = 1; i <= depth; i++)
		{
			str << r;           // Whitespace
		}
		str << current->get_name() << endl;                // Name, endl.

														   // Kinder anhängen -> umgekehrte Reihenfolge
		if (current->v_children.empty())        // Aktuell Blattknoten
		{
			while ((!st_depth.empty()) && st_depth.top() == 0)
			{
				--depth;                // Aktuelle Tiefe um 1 verringern
				st_depth.pop();
			}
		}
		else                            // Aktuell nicht Blattknoten
		{
			++depth;                    // Aktuelle Tiefe um 1 erhöhen.
			st_depth.push(current->v_children.size());

			for (i = current->v_children.size() - 1; i >= 0; i--)
			{
				st_search.push(current->v_children[i]);
			}
		}                              // Alle Kinder auf Stack.
	}
	PRINT2("Leaving print_norec_nocycle", "");
}

// ----------ERSTELLUNG:-----------------
/** @brief Erstellt rekursiv einen Baum , bei dem alle Knoten bis auf die Blattknoten genau nr_child_nodes
Kindknoten haben und bei dem die Pfade von der Wurzel bis zu den Blättern genau tree_depth
Knoten enthalten (dabei ist der Wurzelknoten mitzuzählen).
*
* @param nr_child_nodes unsigned int
* @param tree_depth unsigned int
* @return node*
*
* Tiefe = 0 -> Widerspruch
* Tiefe = 1 -> Nur Wurzel
* Tiefe >=2 -> nr_childs >= 1.
*/
node* create_complete_tree(unsigned int nr_child_nodes, unsigned int tree_depth)
{
	node* root = new node();            // Wurzelknoten

	assert(tree_depth>0);               // Tiefe > 0

	if (tree_depth == 1)
	{
		return root;                    // Tiefe = 1
	}

	assert(nr_child_nodes>0);           // Tiefe > 1

										// Erstelle alle 'nr_child_nodes' Kinder
	for (int i = 1; i <= nr_child_nodes; i++)
	{
		// Rufe kompletten Baum mit einer Tiefe weniger auf:
		root->add_child(create_complete_tree(nr_child_nodes, tree_depth - 1));
	}
	return root;
}

ostream& operator<<(ostream& str, const node& n)
{
	PRINT2("Shift <<", " operator.")
		n.print(str);
	return str;
}

/** \brief Konstruktor für node.
leeren String als Defaultparameterwert
Setze im Konstruktor den Knotennamen auf "node_<node_id>", falls kein Knotenname
angegeben wurde. Dabei sollen die automatisch erzeugten Knotennamen mit "node_1" beginnen.
*
* \param in_name const string&
*
*/
node::node(const string& in_name)
{
	//ctor
	node_id++;                          // Erhöhe node_id um 1
	assert(node_id>0);

	if (in_name.empty())                 // Leerer String -> Benenne
	{
		stringstream str_sm;
		str_sm << "node_" << node_id;
		set_name(str_sm.str());
	}
	else
	{
		set_name(in_name);              // Setze Name
	}

	v_children = vector<node*>{};      // Initialisiere Liste für Kinder

	PRINT2("Created node: ", get_name());
}

/** \brief Destructor
*  !!NOT handling cyclic graphs!!
*
*/
node::~node()
{
	PRINT2("enter ~node() of ", name);
	//dtor
	for (auto c : v_children)
	{
		delete c;
	}
	PRINT2("leave ~node() of ", name);
}
