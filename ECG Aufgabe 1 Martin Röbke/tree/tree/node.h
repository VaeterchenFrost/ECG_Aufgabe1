#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <stack>

/** Statische Bibliothek Header-File für die Klasse 'node'.
* Einführung in die Computergrafik Aufgabe 1
* 11.05.2017
* Autor: Martin Röbke
*/

class node
{
public:

	node(const std::string& = "");          // Konstruktor
	virtual ~node();                        // ~Löschen aller Kindknoten

											// Getter
	unsigned int get_nr_children() const;
	static int get_node_id();
	std::string get_name() const;
	// - Gibt einen Zeiger auf den i-ten direkten Kindknoten zurück.
	node* get_child(const unsigned int) const;
	// Setter
	void set_name(const std::string&);
	// - Fügt am Ende einen neuen direkten Kindnoten hinzu.
	void add_child(const node*);

	// Ausgabemethoden:
	void print(std::ostream&, unsigned int depth = 0, unsigned int intent = 3, const char ws = ' ') const;
	void print_nocycle(std::ostream&, unsigned int depth = 0, std::unordered_set<node*> * p_myset = NULL, \
		unsigned int intent = 3, const char ws = ' ') const;
	void print_norec(std::ostream&, unsigned int intent = 3, const char ws = ' ') const;

	void print_norec_nocycle(std::ostream&, unsigned int intent = 3, const char ws = ' ') const;

protected:

private:
	std::string name;
	std::vector<node*> v_children;  // Vektor der Kinder
	static int node_id;             // Negativ zu Testzwecken möglich.
									//    // Taboo... this should just never happen!!!
									//    // Here is a declaration, but no implementation will be written
									//    node(const node& other);       // Copy constructor
									//    node& operator=(const node&);  // Assignment
};

node* create_complete_tree(unsigned int, unsigned int);
extern std::ostream& operator<<(std::ostream&, const node&);

#endif // NODE_H
