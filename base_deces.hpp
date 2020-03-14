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

using deces_file_t =  io::CSVReader<2, io::trim_chars<' '>,
										 io::double_quote_escape<',','\"'> >;

enum class sexe_t {
   masculin = 1,
	 feminin = 2
};

std::ostream& operator<<(std::ostream& os, sexe_t s);

using prenom_t = std::pair<std::string, sexe_t>;

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
		return p1.first == p2.first;
	}
};

using base_deces_t = std::unordered_map<prenom_t,int,hash_pair,equality_prenom_t>;

void lire_base_deces(const std::string& nomfic, base_deces_t& bp);
using branche_t = std::pair<int, prenom_t>;

//pour la fonction sépararnt les prénoms et les associants:
using separes_t = std::pair<std::string, std::vector<std::string> >;
//fonction séparant les prénoms associés et renvoyant une paire prenom/prénoms associés
separes_t separate(std::string prenoms_str);






#endif // __base_deces_hpp__
