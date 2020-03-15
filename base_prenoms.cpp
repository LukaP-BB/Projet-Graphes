/*
	Fonctions de lecture d'une INSEE de prénoms au format CSV:
	prénom,sexe,ddn,nombre

	Auteur: Frédéric Goualard, Université de Nantes, 2020
 */
#include <iostream>
#include <string>
#include "base_prenoms.hpp"

using namespace std;


ostream& operator<<(ostream& os, sexe_t s)
{
	switch (s) {
	case sexe_t::masculin:
		os.put('H');
		break;
	case sexe_t::feminin:
		os.put('F');
		break;
	}
	return os;
}

ostream& operator<<(ostream& os, const prenom_t& p)
{
	os << "(" << p.first << "," << p.second << ")";
	return os;
}


void lire_base_prenoms(const string& nomfic, base_prenoms_t& bp)
{
	try {
		name_file_t f(nomfic);
		f.read_header(io::ignore_extra_column,"prénom","sexe","ddn","nombre");
		
		string prenomstr;
		int sexe;
		uint32_t annee, nombre;
		while (f.read_row(prenomstr, sexe, annee,nombre)) {
			auto prenom = prenom_t{prenomstr,sexe_t(sexe)};
			const auto& iter = bp.find(prenom);
			if (iter != bp.end()) { // Le prénom est déjà dans la base ?
				// => on ajoute l'année courante et le nombre d'occurrences
				(iter->second)[annee-1900]=nombre;
			} else { // Le prénom n'est pas dans la base ?
				// => On l'y ajoute
				bp[prenom] = occurrences_t{0};
				bp[prenom][annee-1900]=nombre;
				cout << ">> " << bp[prenom][annee-1900] << endl;
			}
		}
	} catch (const io::error::can_not_open_file& e) {
		cerr << e.what() << endl;
		exit(1);
	}
}
