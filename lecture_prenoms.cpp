/*
	Exemple de lecture du fichier INSEE de prénoms.
 */
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "base_prenoms.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Erreur: nom de fichier csv attendu\n";
		exit(2);
	}

	base_prenoms_t bp;
	
	lire_base_prenoms(argv[1],bp);

	for (const auto& p : bp) {
		cout << p.first.first << " (" << p.first.second << ") :";
		for (uint32_t i = 0; i < OFFSET; ++i) {
			if (p.second[i] != 0) { // Des occurrences pour cette année ?
				cout << "(" << (1900+i) << "," << p.second[i] << ") ";
			}
		}
		cout << endl;
	}
}
