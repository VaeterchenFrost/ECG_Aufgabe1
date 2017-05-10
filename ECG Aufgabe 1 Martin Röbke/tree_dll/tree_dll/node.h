#ifndef __NODE_H__
#define __NODE_H__

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <stack>

/** Dynamic Link Library Header-File für die Klasse 'node'.
* Einführung in die Computergrafik Aufgabe 1
* 11.05.2017
* Autor: Martin Röbke
*/

#ifdef TREE_DLL_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	void DLL_EXPORT SomeFunction(const LPCSTR sometext);

	class node
	{
	public:

		DLL_EXPORT node(const std::string& = "");          // Konstruktor
		virtual DLL_EXPORT ~node();                        // ~Löschen aller Kindknoten

														   // Getter
		DLL_EXPORT unsigned int get_nr_children() const;
		DLL_EXPORT static int get_node_id();
		DLL_EXPORT std::string get_name() const;
		// - Gibt einen Zeiger auf den i-ten direkten Kindknoten zurück.
		DLL_EXPORT node* get_child(const unsigned int) const;
		// Setter
		DLL_EXPORT void set_name(const std::string&);
		// - Fügt am Ende einen neuen direkten Kindnoten hinzu.
		DLL_EXPORT void add_child(const node*);

		// Ausgabemethoden:
		DLL_EXPORT void print(std::ostream&, unsigned int depth = 0, unsigned int intent = 3, const char ws = ' ') const;
		DLL_EXPORT void print_nocycle(std::ostream&, unsigned int depth = 0, std::unordered_set<node*> * p_myset = NULL, \
			unsigned int intent = 3, const char ws = ' ') const;
		DLL_EXPORT void print_norec(std::ostream&, unsigned int intent = 3, const char ws = ' ') const;

		DLL_EXPORT void print_norec_nocycle(std::ostream&, unsigned int intent = 3, const char ws = ' ') const;

	protected:

	private:
		std::string name;
		std::vector<node*> v_children;  // Vektor der Kinder
		DLL_EXPORT static int node_id;             // Negativ zu Testzwecken möglich.

	};

	DLL_EXPORT node* create_complete_tree(unsigned int, unsigned int);
	extern DLL_EXPORT std::ostream& operator<<(std::ostream&, const node&);

#ifdef __cplusplus
}
#endif

#endif // __NODE_H__
