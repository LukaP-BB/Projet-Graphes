/*
	Fonctions de lecture d'une INSEE de prénoms au format CSV
	(https://www.insee.fr/fr/statistiques/2540004#documentation):

	prénom,sexe,ddn,nombre


	Auteur: Frédéric Goualard, Université de Nantes, 2020
 */
#ifndef __base_prenoms_hpp__
#define __base_prenoms_hpp__

#include <iosfwd>
#include <string>
#include <unordered_map>
#include <array>
#include <csv/csv.h>

// Type fichier INSEE avec 4 champs d'intérêt
using name_file_t =  io::CSVReader<4, io::trim_chars<' '>,
																	 io::double_quote_escape<',','\"'> >;

/*
	Le fichier INSEE attribue la valeur 1 aux prénoms donnés à un homme et 2 à ceux
	donnés à une femme.
*/
enum class sexe_t {
   masculin = 1,
	 feminin = 2
};

/*
 Spécialisation de l'opérateur `<<` pour afficher naturellement un sexe:
 'H' pour `masculin`
 'F' pour `feminin`
*/
std::ostream& operator<<(std::ostream& os, sexe_t s);

/*
	Un 'prénom' est composé du prénom lui-même et du sexe qui lui est associé:
	Exemples:
	   {"dominique", masculin}
		 {"dominique", feminin}
 */
using prenom_t = std::pair<std::string, sexe_t>;

/*
 Spécialisation de l'opérateur `<<` pour afficher naturellement un prénom de
 type `prenom_t`:
 ("prenom",sexe)
*/
std::ostream& operator<<(std::ostream& os, const prenom_t& p);


/*
 Fonction de hachage pour le type `prenom_t` pour pouvoir utiliser
 un objet de type `prenom_t` comme clé d'un `unordered_map`

 Pour pallier les erreurs de classification des prénoms en fonction du sexe
 dans la base de prénoms, on ne considère pas le sexe pour le hachage.

 @warning Une conséquence de cette clé est qu'il ne faut pas tenir compte
 du sexe lors de l'exploitation de la base de prénoms.
*/
struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		return hash1;
    }
};

/*
	On rédéfinit l'égalité de deux prénoms sans tenir compte du sexe car
	la base des prénoms contient des erreurs de classification en fonction du
	sexe. On choisit donc de ne pas en tenir compte du tout pour cette base.
 */
struct equality_prenom_t {
	bool operator()(const prenom_t& p1, const prenom_t& p2) const {
		return p1.first == p2.first;
	}
};

/*
	Le nombre d'occurrences de chaque prénom par année sera stocké dans un tableau
	statique avec à la position `i` le nombre d'occurrences pour l'année 1900+i.
	Comme l'on dispose d'une base de prénoms de 1900 à 2018, le tableau doit
	contenir 2018-1900+1=119 cases.

	S'il apparaît que la plupart des prénoms ne sont utilisés que sur des plages
	réduites de temps, on pourra remplacer le tableau statique par un
	`unordered_map`.
*/
const uint32_t OFFSET = 119;
using occurrences_t = std::array<uint32_t,OFFSET>;

/*
	Une base de prénoms contient pour chaque 'prénom' (couple prénom/sexe) un tableau
	indiquant pour chaque année de 1900 à 2018 le nombre de naissances portant ce
	prénom.
 */
using base_prenoms_t = std::unordered_map<prenom_t,occurrences_t,
																					hash_pair,equality_prenom_t>;

/*
	Lecture d'un fichier INSEE de prénoms au format CSV.

	@param[in] nomfic: nom du fichier contenant la base
	@param[inout] bp: base de données de prénoms remplie avec le contenu du fichier.

	@note En cas de problème d'ouverture du fichier, la fonction affiche un
	message d'erreur sur le canal d'erreur standard et quitte le programme avec un
	code d'erreur 1.
 */
void lire_base_prenoms(const std::string& nomfic, base_prenoms_t& bp);


//##############################################################################


#endif // __base_prenoms_hpp__
