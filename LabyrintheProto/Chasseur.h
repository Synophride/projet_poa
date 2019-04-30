#ifndef CHASSEUR_H
#define CHASSEUR_H

#include <stdio.h>
#include "Mover.h"
#include "Sound.h"
#include "Labyrinthe.h"

class Chasseur : public Mover {
private:
    /**
     * \brief accepte ou non un d�placement
     **/
    bool move_aux (double dx, double dy);
    int _pv = 10; //!< nombre de points de vie du chasseur
    Labyrinthe* l; //!< Labyrinthe correspondant � l'environnement
    int perte_precision = 0; //!< perte de pr�cision du chasseur

    /**
     * \brief fonction indiquant au chasseur qu'il est mort
     * 
     * Va simplement entra�ner la fin de la partie
     */ 
    void die();
	
public:
	/*
	 *	Le son...
	 */
	static Sound*	_hunter_fire;	//!< bruit de l'arme du chasseur.
	static Sound*	_hunter_hit;	//!< cri du chasseur touch�.
	static Sound*	_wall_hit;      //!< on a tap� un mur.

	/**
	 * \brief constructeur de chasseur, en prenant en param un pointeur de Labyrinthe
	 **/
	Chasseur (Labyrinthe* l);

	/**
	 * \brief fonction indiquant au chasseur qu'il a �t� touch�
	 * 
	 * Va baisser son nombre de points de vie et sa pr�cision
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
	 * \brief fonction permettant le d�placement des boules de feu
	 * \param dx le d�placement de la boule de feu sur l'axe x
	 * \param dy le d�placement de la boule de feu sur l'axe y
	 * \return true si la boule de feu n'a pas explos�,
	 * false si la boule de feu a explos� (par exemple � la rencontre d'un mur)
	 **/
	bool process_fireball (float dx, float dy);
	
	/**
	 * \brief fonction de tir. Cr�e une boule de feu et initie son mouvement
	 * \param angle_vertical l'angle vertical
	 **/
	void fire (int angle_vertical);
	
	// clic droit.
	void right_click (bool shift, bool control);
};

#endif
