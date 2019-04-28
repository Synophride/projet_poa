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
    static const int BASE_PV = 10;
    bool dead = false;
    Labyrinthe * l;
    int _pv = BASE_PV;
    
    /**
    * \brief teste si le mouvement de coordonnées (dx, dy) est acceptable, 
    * id est que ça implique pas de marcher au travers d'un mur
    **/
    bool is_legit_move(double dx, double dy);

    /**
     * \brief tente d'accomplir le mouvement en paramètre
     * 
     **/
    bool try_move(double dx, double dy);

    bool avancer();

    bool move_to_treasure();

    void die();
    
    public:
    Gardien (Labyrinthe* laby, const char* modele) : Mover (120, 80, laby, modele){
	l = (Labyrinthe*) _l;
    }

    ~Gardien(){}
    
    // mon gardien pense très mal!
    void update(void);
    
    void hurt();
    
    /**
     * \brief Tente d'accomplir le mouvement en paramètre. Meme principe 
     * que try_move, mais va faire en sorte de glisser sur les murs 
     * \param dx mouvement en x tenté par le gardien 
     * \param dy mouvement en y tenté par le gardien
     * \return true si le mouvement a été accompli, false sinon
     **/
    bool move (double dx, double dy);

    
    // ne sait pas tirer sur un ennemi.
    void fire (int angle_vertical);

    // quand a faire bouger la boule de feu...
    bool process_fireball (float dx, float dy);
};
#endif
