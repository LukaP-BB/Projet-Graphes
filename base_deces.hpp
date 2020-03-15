/*
	Gestion de la base de personnes décédées pour en extraire les prénoms.
 */

#ifndef __base_deces_hpp__
#define __base_deces_hpp__

#include <iosfwd>
#include <string>
#include <unordered_map>
#include <array>
#include <csv/csv.h>

using deces_file_t =  io::CSVReader<1, io::trim_chars<' '>,
										 io::double_quote_escape<',','\"'> >;

enum class sexe_t {
   masculin = 1,
	 feminin = 2
};

std::ostream& operator<<(std::ostream& os, sexe_t s);

using prenom_t = std::pair<std::string, sexe_t>;
std::ostream& operator<<(std::ostream& os, const prenom_t& p);

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		return hash1;
	}
};

struct equality_prenom_t {
	bool operator()(const prenom_t& p1, const prenom_t& p2) const {
		return p1.first == p2.first && p1.second == p2.second;
	}
};

//Une table qui contiendra des prénoms et un poids associé
using branche_t = std::unordered_map<std::string,int>;
//Une table qui contiendra pour chaque prénom, la table des prénoms associés branche_t
using base_deces_t = std::unordered_map<std::string,branche_t>;

//fonction lisant le fichier et englobant les autres fonctions
void lire_base_deces(const std::string& nomfic, base_deces_t& bp);
//fonction séparant les prénoms associés et renvoyant une paire prenom/prénoms associés
void separate(std::string prenoms_str, auto& liste_p);
//fonction insérant ces prénoms dans la base
void prenom_add(std::vector<std::string> liste_prenoms, base_deces_t& base_deces);






#endif // __base_deces_hpp__
