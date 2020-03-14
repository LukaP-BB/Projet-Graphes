/*
	Gestion de dates au format ISO

	Ce fichier n'est pas nécessaire pour la réalisation du projet mais peut 
	être utilisé pour d'éventuelles extensions.
 */
#include <sstream>
#include <iostream>
#include <cstdio>
#include <utility>
#include <assert.h>
#include "date.hpp"

using namespace std;


date_t str2date(const std::string& s)
{
	uint32_t j, m, a;
	sscanf(s.c_str(),"%u-%u-%u",&a,&m,&j);
	return date_t{j,m,a};
}

std::string date2str(const date_t& d)
{
	ostringstream buf;
	buf << d.annee << "-" << d.mois << "-" << d.jour;
	return buf.str();
}

std::string duration2str(const duration_t& d)
{
	ostringstream buf;
	buf << d.annee << "a., " << d.semaines << "w., " << d.jour << "d.";
	return buf.str();
}

std::ostream& operator<<(std::ostream& os, const date_t& d)
{
	os << date2str(d);
	return os;
}

std::ostream& operator<<(std::ostream& os, const duration_t& d)
{
	os << duration2str(d);
	return os;
}

// Transformation d'une date du calendrier grégorien en un jour Julien.
uint32_t date2julian(const date_t& dd)
{ // https://stason.org/TULARC/society/calendars/2-15-1-Is-there-a-formula-for-calculating-the-Julian-day-nu.html
	uint32_t y = dd.annee, m = dd.mois, d = dd.jour;

	uint32_t a = (14-m)/12;
	y += 4800-a;
	m += 12*a - 3;
	return d + (153*m+2)/5 + y*365 + y/4 - y/100 + y/400 - 32045;
	
}



duration_t operator-(const date_t& end, const date_t& begin)
{
	uint32_t j1 = date2julian(begin);
	uint32_t j2 = date2julian(end);
	if (j1 > j2) {
		swap(j1,j2);
	}

	uint32_t diff = j2 - j1 + 1;
	uint32_t years = uint32_t(diff / 365.25);
	uint32_t weeks = uint32_t((365.25*((diff / 365.25) - years)) / 7); 
	uint32_t days = diff - years * 365.25 - weeks * 7;
	return duration_t{days,weeks,years};
}

bool operator<(const date_t& d1, const date_t& d2)
{
	return date2julian(d1) < date2julian(d2);
}

bool operator<=(const date_t& d1, const date_t& d2)
{
	return date2julian(d1) <= date2julian(d2);
}

bool operator>(const date_t& d1, const date_t& d2)
{
	return date2julian(d1) > date2julian(d2);
}

bool operator>=(const date_t& d1, const date_t& d2)
{
	return date2julian(d1) >= date2julian(d2);
}

bool operator==(const date_t& d1, const date_t& d2)
{
	return date2julian(d1) == date2julian(d2);
}

bool operator!=(const date_t& d1, const date_t& d2)
{
	return date2julian(d1) != date2julian(d2);
}
