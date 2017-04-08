//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include "heure.h"

Heure Heure::infini(){
    return Heure(INT_MAX);
}

bool Heure::operator !=(const Heure& h) const{
    return nbsecondes != h.nbsecondes;
}
bool Heure::operator <(const Heure& h) const{
    return nbsecondes < h.nbsecondes;
}
bool Heure::operator <=(const Heure& h) const{
    return nbsecondes <= h.nbsecondes;
}
bool Heure::operator ==(const Heure& h) const{
    return nbsecondes == h.nbsecondes;
}

Heure& Heure::operator +=(const Heure& h){
    nbsecondes += h.nbsecondes;
    return *this;
}

Heure Heure::operator +(const Heure& h) const{
    Heure r = *this;
    r+= h;
    return r;
}


istream& operator >> (istream& is, Heure& heure){
    int heures, minutes, secondes;
    char m, h, s;
    is >> heures >> h >> minutes >> m >> secondes >> s;
    if(!is) return is;
    assert(h=='h' && m=='m' && s=='s');
    heure.nbsecondes += heures;
    heure.nbsecondes *= 60;
    heure.nbsecondes += minutes;
    heure.nbsecondes *= 60;
    heure.nbsecondes += secondes;
    return is;
}

ostream& operator << (ostream& os, const Heure& h){
    int s = h.nbsecondes;
    int secondes = s % 60;
    s /= 60;
    int minutes = s % 60;
    s /= 60;
    int heures = s;
    char chaine[40];
    sprintf(chaine, "%02dh%02dm%02ds", heures, minutes, secondes);
    os << chaine;
    return os;
}

