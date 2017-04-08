//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#if !defined(_INF3105_RESEAU_H_)
#define _INF3105_RESEAU_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "heure.h"
#include "coordonnee.h"

using namespace std;

// Représentation d'un arrêt d'autobus ou une station de métro.
struct Arret{
    Coordonnee coor;  // Position de l'arrêt sur la carte.
    string nom;       // Nom, correspondand au champ nom dans stops.txt au format GTFS.

    // À modifier : (choisir un TYPE approprié)
    //mutable TYPE noeudproche; // Voir carte.cpp / Carte::integrerReseau / étape #1
    //mutable TYPE noeudcarte;  // Voir carte.cpp / Carte::integrerReseau / étape #2
};

// Représentation d'une étape d'un parcours de circuit d'autobus ou de train
struct EtapeParcours{
    Heure heure;  // Heure de passage
    int   arret;  // Numéro d'arrêt
};

// Représentation d'un parcours de circuit d'autobus ou de train.
struct Parcours{
    int circuit;
    string destination;
    vector<EtapeParcours> etapes;
    friend istream& operator >> (istream& is, Parcours& reseau);
};

// Représentation d'un réseau de transport en commun
class Reseau{
  public:
  
  protected:
    map<int, string> circuits;
    map<int, Arret>  arrets;
    vector<Parcours>  parcours;

  friend class Carte; // Pour être intégré dans la carte.
  friend istream& operator >> (istream& is, Reseau& reseau);
};

#endif

