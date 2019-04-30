#ifndef CHASSEUR_H
#define CHASSEUR_H

#include <stdio.h>
#include "Mover.h"
#include "Sound.h"
#include "Labyrinthe.h"

class Chasseur : public Mover {
private:
    /**
     * \brief accepte ou non un déplacement
     **/
    bool move_aux (double dx, double dy);
    int _pv = 10; //!< nombre de points de vie du chasseur
    Labyrinthe* l; //!< Labyrinthe correspondant à l'environnement
    int perte_precision = 1; //!< perte de précision du chasseur

    /**
     * \brief fonction indiquant au chasseur qu'il est mort
     * 
     * Va simplement entraîner la fin de la partie
     */ 
    void die();
	
public:
	/*
	 *	Le son...
	 */
	static Sound*	_hunter_fire;	//!< bruit de l'arme du chasseur.
	static Sound*	_hunter_hit;	//!< cri du chasseur touché.
	static Sound*	_wall_hit;      //!< on a tapé un mur.

	/**
	 * \brief constructeur de chasseur, en prenant en param un pointeur de Labyrinthe
	 **/
	Chasseur (Labyrinthe* l);

	/**
	 * \brief fonction indiquant au chasseur qu'il a été touché
	 * 
	 * Va baisser son nombre de points de vie et sa précision
	 **/
	void hurt();


	/**
	 * \brief fonction de mouvement du chasseur
	 **/
	bool move (double dx, double dy) {
	    return move_aux (dx, dy) || move_aux (dx, 0.0) || move_aux (0.0, dy);
	}
	
	// le chasseur ne pense pas!
	void update (void) {};
	
	/**
	 * \brief fonction permettant le déplacement des boules de feu
	 * \param dx le déplacement de la boule de feu sur l'axe x
	 * \param dy le déplacement de la boule de feu sur l'axe y
	 * \return true si la boule de feu n'a pas explosé,
	 * false si la boule de feu a explosé (par exemple à la rencontre d'un mur)
	 **/
	bool process_fireball (float dx, float dy);
	
	/**
	 * \brief fonction de tir. Crée une boule de feu et initie son mouvement
	 * \param angle_vertical l'angle vertical
	 **/
	void fire (int angle_vertical);
	
	// clic droit.
	void right_click (bool shift, bool control);
};

#endif
