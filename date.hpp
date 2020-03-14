/*
	Fonctions de gestion de dates de naissance ou de décès.

	Auteur: Frédéric Goualard <Frederic.Goualard@univ-nantes.fr>, 2020

	Ce fichier n'est pas nécessaire pour la réalisation du projet mais peut 
	être utilisé pour d'éventuelles extensions.
 */

#ifndef __date_hpp__
#define __date_hpp__

#include <iosfwd>
#include <string>

#include <iostream>
/*
	Date de naissance ou de décès. La création se fait sous la forme JJ MM AAAA:

	date_t d{14,07,1789};
 */
struct date_t {
	date_t() {}
	date_t(uint32_t j, uint32_t m, uint32_t a) : jour(j), mois(m), annee(a) {}
	date_t(const date_t& d) : jour(d.jour), mois(d.mois), annee(d.annee) {}
	uint32_t jour;
	uint32_t mois;
	uint32_t annee;
};

// Durée entre deux dates
struct duration_t {
	duration_t() {}
	duration_t(uint32_t j, uint32_t w, uint32_t a) : jour(j), semaines(w), annee(a) {}
	uint32_t jour;
	uint32_t semaines;
	uint32_t annee;
};

/*
	Création d'une date à partir d'une chaîne de caractères de la forme:

	AAAA-MM-JJ
 */
date_t str2date(const std::string& s);

/*
	Création d'une représentation textuelle d'une date `date_t` sous la forme:
	AAAA-MM-JJ
 */
std::string date2str(const date_t& d);

/*
	Opérateur d'envoi d'une date dans un flux permettant d'écrire:

	date_t d{14,07,1789};
	cout << d << endl;

	// Résultat: "1789-07-14"
 */
std::ostream& operator<<(std::ostream& os, const date_t& d);

// Comparaison de deux dates
//{
bool operator<(const date_t& d1, const date_t& d2);
bool operator<=(const date_t& d1, const date_t& d2);
bool operator>(const date_t& d1, const date_t& d2);
bool operator>=(const date_t& d1, const date_t& d2);
bool operator==(const date_t& d1, const date_t& d2);
bool operator!=(const date_t& d1, const date_t& d2);
//}

/*
	Calcul du nombre de jours, mois et années entre deux dates.

	L'opérateur calcule un intervalle de temps entre `begin` et `end`, aucun ordre n'étant
	requis pour les deux dates. L'intervalle compte les jours de début et de fin.
 */
duration_t operator-(const date_t& end, const date_t& begin);

std::string duration2str(const date_t& d);
std::ostream& operator<<(std::ostream& os, const duration_t& d);


#endif // __date_hpp__
