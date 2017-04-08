//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#if !defined(_INF3105_CARTE_H_)
#define _INF3105_CARTE_H_

#include <cassert>
#include <istream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "coordonnee.h"
#include "reseau.h"
#include <queue>



using namespace std;

class Carte {
public:
    Carte();

    void ajouterNoeud(const string &nom, const Coordonnee &c);

    const string &getNomNoeudPlusProche(const Coordonnee &c) const;

    void ajouterRoute(const string &nom,  list<string> &noms);

    double calculerChemin(const string &origine, const string &destination,
                          std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes)  ;

    double calculerChemin(const Coordonnee &origine, const Coordonnee &destination,
                          std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) ;

    void integrerReseau(const Reseau &);

    Heure calculerItineraire(Heure depart, const string &origine, const string &destination,
                             std::list<string> &out_itinerairenoeuds,
                             std::list<string> &out_itineraireinstructions) const;

    Heure calculerItineraire(Heure depart, const Coordonnee &origine, const Coordonnee &destination,
                             std::list<string> &out_itinerairenoeuds,
                             std::list<string> &out_itineraireinstructions) const;

    struct Noeud {
        Noeud();
        string nom;
        Coordonnee coor;
        bool visite;
        double distanceLaPlusCourte;
        string parent;

        bool operator<(const Noeud &autre) const;
    };
private:

    struct Arete {
        Arete();

        string nomRue;
        string depart;
        string arrive;
        double distanceMetre;

        bool operator<(const Arete &autre) const;
    };

    // À compléter (vous pouvez changer tout ce que vous voulez)
    vector<Arete> aretes;
    vector<Noeud> noeuds;
    map<string, int> indexnoeuds;
    map<string, vector<Arete> > aretesortante;

    friend istream &operator>>(istream &is, Carte &carte);

    void remplirQueue(string noeudActuel, std::priority_queue<Arete> &queue);

    void construireChemin(const string &nomdestination, const string &nomorigine, list<string> &out_cheminnoeuds,
                          list<string> &out_cheminroutes);

    Noeud setSortant(const Noeud &noeudActuel, const Noeud &noeudArrive, const Noeud &noeudDepart,
                                   string &nomNoeudArrive) ;

    Noeud setEntrant(const Noeud &noeudActuel, const Noeud &noeudArrive, const Noeud &noeudDepart,
                                   string &nomNoeudDepart);

    double getDistance(const Arete &areteLaPlusCourte, const Noeud &noeudParentLocal) const;

    void resetCarte() ;
};

#endif

