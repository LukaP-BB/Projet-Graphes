/*
	Gestion de la base de personnes décédées.
 */
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include "base_deces.hpp"
 // #include "base_prenoms.hpp"

 using namespace std;

void lire_base_deces(const string& nomfic, base_deces_t& bp)
{
	try {
		deces_file_t f(nomfic);
		f.read_header(io::ignore_extra_column,"prenom","sexe");
		string prenomstr;
		int sexe;
		string p_sep, prenom;
		// uint32_t annee, nombre;
    std::vector<prenom_t> liste_prenoms;
		while (f.read_row(prenomstr, sexe)) {
      int sexe_a = sexe;

			//séparation des prénoms
      separate(prenomstr, liste_prenoms, sexe_a);

			// stringstream s(prenomstr);
			//
			// while (s>>p_sep){
			//
			// 	auto prenom = prenom_t{p_sep,sexe_t(sexe)};
			// 	const auto& iter = bp.find(prenom);
			// 	if (iter != bp.end()) { // Le prénom est déjà dans la base ?
			//
			// 		// bp[prenom];
			// 		// => on ajoute l'année courante et le nombre d'occurrences
			// 	// 	++(iter->second)[annee-1900];
			// 	} else { // Le prénom n'est pas dans la base ?
			// 	// // => On l'y ajoute
			// 		bp[prenom];
			// 	}
			// }


		}

    for (prenom_t a : liste_prenoms) {
      std::cout << a << '\n';
    }

	} catch (const io::error::can_not_open_file& e) {
		cerr << e.what() << "\n";
		exit(1);
	}
}

//fonction séparant les prénoms associés et renvoyant une liste de prénoms associés
std::vector<prenom_t> separate(string prenoms_str, auto& liste_p, int sexe){
  int sexe_a = sexe;
	stringstream s(prenoms_str);
	string p_sep;
	while (s>>p_sep){
    prenom_t p_sepa = prenom_t{p_sep,sexe_t(sexe_a)};
    liste_p.push_back(p_sepa);
		// cout << p_sep << '\n';
	}
}
