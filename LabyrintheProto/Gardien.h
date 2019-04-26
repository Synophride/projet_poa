#ifndef GARDIEN_H
#define GARDIEN_H

#include "Mover.h"

#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <map>
#include <list>
#include <limits.h>
#include <set>
#include <iterator>
#include <stdexcept>
#include "Labyrinthe.h"

// class Labyrinthe;

struct node {
    int x;
    int y;
};




class Gardien : public Mover {
private:
    bool inited = false;
    double speed = 1;
    
    /**
    * \brief teste si le mouvement de coordonn�es (dx, dy) est acceptable, 
    * id est que �a implique pas de marcher au travers d'un mur
    **/
    bool is_legit_move(double dx, double dy);

    /**
     * \brief tente d'accomplir le mouvement en param�tre
     * 
     **/
    bool try_move(double dx, double dy);

    bool avancer();

    bool move_to_treasure();
    
    public:
    Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele){}

    ~Gardien(){}
    
    // mon gardien pense tr�s mal!
    void update(void);

    /**
     * \brief Tente d'accomplir le mouvement en param�tre. Meme principe 
     * que try_move, mais va faire en sorte de glisser sur les murs 
     * \param dx mouvement en x tent� par le gardien 
     * \param dy mouvement en y tent� par le gardien
     * \return true si le mouvement a �t� accompli, false sinon
     **/
    bool move (double dx, double dy);

    
    // ne sait pas tirer sur un ennemi.
    void fire (int angle_vertical);

    // quand a faire bouger la boule de feu...
    bool process_fireball (float dx, float dy);

};
#endif
