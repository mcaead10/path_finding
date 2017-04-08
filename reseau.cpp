//  INF3105 - Structures de données et algorithmes
//  UQAM / Département d'informatique
//  Été 2015 / TP3 (http://info.uqam.ca/~beaudry/INF3105/tp3/)

#include <cassert>
#include <cstdlib>
#include "coordonnee.h"
#include "reseau.h"

istream& operator >> (istream& is, Parcours& parcours){
    is >> parcours.circuit;
    is >> parcours.destination;
    char c;
    is >> c;
    while(c != ';'){
        if(c!='#') cout << "'" << c << "'" << endl << "destination: " << parcours.destination << endl;
        assert(c=='#');
        EtapeParcours e;
        is >> e.arret;
        is >> c;
        assert(c=='@');
        is >> e.heure;
        parcours.etapes.push_back(e);
        is >> c;
    }
    return is;
}

istream& operator >> (istream& is, Reseau& reseau)
{
    //Lire les circuits
    string sid, nom;
    while(is && !is.eof()){
        is >> sid;
        if(sid=="---") break;
        int id = atoi(sid.c_str());
        is >> reseau.circuits[id];
    }
    
    // Lire les arrêts
    while(is && !is.eof()){
        is >> sid;
        if(sid=="---") break;
        int id = atoi(sid.c_str());
        Arret& a = reseau.arrets[id];
        is >> a.coor;
        is >> a.nom;
    }
    
    // Lire les parcours
    while(is && !is.eof()){
        Parcours parcours;
        is >> parcours >> ws;
        reseau.parcours.push_back(parcours);
    }
    /*
    cerr << "Réseau chargé : "
         << reseau.circuits.size() << " circuits, " 
         << reseau.arrets.size() << " arrêts, "
         << reseau.parcours.size() << " parcours."
         << endl;
    */
    
    return is;
}

