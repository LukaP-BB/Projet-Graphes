/*
	Projet de TP X2BI040
	2019/2020
	Les prénoms
 */

#include <cstdlib>
#include "base_deces.hpp"
// #include "base_prenoms.hpp"
#include <iostream>
using namespace std;

// TODO


int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Erreur: nom de fichier csv attendu merde\n";
		exit(2);
	}

	base_deces_t bp;
	std::vector<std::string> vect;

	lire_base_deces(argv[1], vect);
	int n_max = 0;
	// comptage(vect, n_max);
	std::cout << "n max = " << n_max << '\n';
	separate(vect, bp);

	// cout << bp.first.first;
	//
	for (const auto& p : bp) {
		cout << p.first << endl;
		for (const auto& p : p.second){
			cout << "\t" << p.first << " " << p.second <<endl;
		}
	}
}

