//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#if !defined(_INF3105_COORDONNEE_H_)
#define _INF3105_COORDONNEE_H_

#include <iostream>

class Coordonnee {

  public:
    Coordonnee(){}
    Coordonnee(double latitude_, double longitude_);
    Coordonnee(const Coordonnee&);

    double distance(const Coordonnee&) const;

  private:
    double latitude;
    double longitude;

  friend std::ostream& operator << (std::ostream&, const Coordonnee&);
  friend std::istream& operator >> (std::istream&, Coordonnee&);
};

#endif

