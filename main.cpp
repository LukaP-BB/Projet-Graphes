/*
	Projet de TP X2BI040
	2019/2020
	Les prénoms
 */

#include <cstdlib>
#include "base_deces.hpp"
#include "base_prenoms.hpp"
#include <iostream>
using namespace std;

// TODO


int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Erreur: nom de fichier csv attendu merde\n";
		exit(2);
	}

	base_prenoms_t bp;

	lire_base_deces(argv[1],bp);

	// cout << bp.first.first;
	//
	for (const auto& p : bp) {
		cout << p.first.first << endl;
	}
}

