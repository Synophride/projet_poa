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
    bool fired = false; //!< Indique si une boule de feu a déjà été tirée. Si c'est le cas, il est impossible pour le gardien d'en tirer une autre.  

    static const int RELOAD_TIME = 100; //!< Temps de rechargement pour les gardiens (en nombre d'appels de update())
    bool reloading = false; //!< indique si le gardien est en train de recharger (si c'est le cas, il ne peut pas tirer)
    int reload = 0; //!< indique le statut du rechargement

    int perte_precision = 5; //!< indique la perte de précision, en degrés, du gardien
    static const int BASE_PV = 10; //!< Nombre de PV de base du gardien
    bool dead = false; //!< indique si le gardien est mort
    Labyrinthe * l; //!< pointeur vers le labyrinthe
    int _pv = BASE_PV; //!< Nombre de PV restants au gardien

    /**
     * \brief indique si le gardien peut voir le joueur 
     * (ie s'il n'y a ni mur ni obstacle entre les deux). 
     * \returns true si pas d'obstacle entre le garde et le joueur,
     * false inon
     **/
    bool can_see_player(); 
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

    /**
     * \brief fait marcher le gardien vers l'avant
     **/
    bool avancer();

    /**
     * \brief fait en sorte que le gardien aille vers le trésor
     */
    bool move_to_treasure();

    /**
     * \brief fonction appellée lors de la mort du gardien
     **/
    void die();
    
    public:
    Gardien (Labyrinthe* laby, const char* modele) : Mover (120, 80, laby, modele){
	l = (Labyrinthe*) _l;
    }

    ~Gardien(){}
    
    // mon gardien pense très mal!
    void update(void);


    /**
     * \brief fonction indiquant au gardien qu'il a été touché par une 
     * boule de feu provenant du joueur. Baissera sa précision et son nombre
     * de points de vie
     *
     **/
    void hurt();
    
    /**
     * \brief Tente d'accomplir le mouvement en paramètre. Meme principe 
     * que try_move, mais va faire en sorte de glisser sur les murs 
     * \param dx mouvement en x tenté par le gardien 
     * \param dy mouvement en y tenté par le gardien
     * \return true si le mouvement a été accompli, false sinon
     **/
    bool move (double dx, double dy);

    
    /**
     * \brief initialisera une boule de feu, qui sera envoyée
     *
     **/
    void fire (int angle_vertical);

    /**
     * \brief Gère le déplacement de la boule de feu venant du garde
     * \param dx le déplacement sur la coordonnée x
     * \param dy le déplacement sur la coordonnée y
     * 
     */
    bool process_fireball (float dx, float dy);
};
#endif
