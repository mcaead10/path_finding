//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#if !defined(_INF3105_HEURE_H_)
#define _INF3105_HEURE_H_

#include <iostream>
using namespace std;

class Heure{
  public:
    Heure(int nbsec=0) : nbsecondes(nbsec){}

    bool operator <(const Heure& h) const;
    bool operator <=(const Heure& h) const;
    bool operator ==(const Heure& h) const;
    bool operator !=(const Heure& h) const;

    Heure& operator +=(const Heure& h);
    Heure operator +(const Heure& h) const;
    
    static Heure infini();
  
  private:
    int nbsecondes;

  friend istream& operator >> (istream&, Heure&);
  friend ostream& operator << (ostream&, const Heure&);
};

#endif

