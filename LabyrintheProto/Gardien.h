#ifndef GARDIEN_H
#define GARDIEN_H


#include "Mover.h"
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <map>
#include <list>
#include <limits.h>
#include <set>

class Labyrinthe;


void remove_searchs();

struct node{
    int x;
    int y;
};

std::list<node> get_shortest_path(node, node, char ** , int, int);


class Gardien : public Mover {
private:
    /**
    * \brief teste si le mouvement de coordonnées (dx, dy) est acceptable, 
    * id est que ça implique pas de marcher au travers un mur
    **/
    bool is_legit_move(double dx, double dy);

    /**
     * \brief tente d'accomplir le mouvement en paramètre
     * 
     **/
    bool try_move(double dx, double dy);

    
    // int get_potentiel_defense();
    
public:
    Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele){}

    ~Gardien(){}
    
    // mon gardien pense très mal!
    void update(void);

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
