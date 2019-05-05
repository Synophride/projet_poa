#include "Chasseur.h"

/*
 *	Tente un deplacement.
 */
bool Chasseur::move_aux (double dx, double dy)
{
    char contenu_case = l -> data ((int)((_x + dx) / Environnement::scale),
				   (int)((_y + dy) / Environnement::scale));

    int old_x = _x / Environnement::scale,
    	old_y = _y / Environnement::scale;
    
    if (EMPTY == contenu_case || JOUEUR == contenu_case){
	
	_x += dx;
	_y += dy;
	int new_x = _x / Environnement::scale,
	    new_y = _y / Environnement::scale;
	
	l -> set_data(old_x, old_y, EMPTY);
	l -> set_data(new_x, new_y, JOUEUR);
	return true;
    }
    
    return false;
}

/*
 *	Constructeur.
 */
Chasseur::Chasseur (Labyrinthe* laby) : Mover (100, 80, laby, 0){
    l = laby;
    _hunter_fire = new Sound ("sons/hunter_fire.wav");
    _hunter_hit = new Sound  ("sons/hunter_hit.wav");
    if (_wall_hit == 0)
	_wall_hit = new Sound ("sons/hit_wall.wav");
}

/*
 * Fait bouger la boule de feu
 *    (ceci est une exemple, à vous de traiter les collisions spécifiques...)
 */
bool Chasseur::process_fireball(float dx, float dy){
	// calculer la distance entre le chasseur et le lieu de l'explosion.
	float	x = (_x - _fb -> get_x ()) / Environnement::scale;
	float   y = (_y - _fb -> get_y ()) / Environnement::scale;
	float	dist2 = x*x + y*y;
	int new_x = (int)((_fb -> get_x () + dx) / Environnement::scale),
	    new_y = (int)((_fb -> get_y () + dy) / Environnement::scale);	    
	char contenu_case = _l -> data(new_x, new_y);

	// on bouge que dans le vide ou dans le joueur !
	if(EMPTY ==  contenu_case || JOUEUR == contenu_case){
		// il y a la place.
		return true;
	}
	// collision...
	// calculer la distance maximum en ligne droite.
	float	dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
	// faire exploser la boule de feu avec un bruit fonction de la distance.
	_wall_hit -> play (1. - dist2/dmax2);
	
	// teste si on a touché le trésor: juste pour montrer un exemple de la
	// fonction « partie_terminee ».
	if (TREASURE == contenu_case)
	{
		partie_terminee (true);
	} else if (GARDE == contenu_case){
	    Labyrinthe * l = (Labyrinthe*) _l;
	    l -> hurt_gardien_at(new_x, new_y);
	} else if (BOX == contenu_case){
	    l -> hurt_box_at(new_x, new_y);
	}
	return false;
}


/*
 *	Tire sur un ennemi.
 */
void Chasseur::fire (int angle_vertical){    
    int true_ang = _angle;
    if(perte_precision != 0){
	true_ang += rand() % (2 * perte_precision);
	true_ang -= perte_precision;
	true_ang = true_ang % 360;
	angle_vertical = (angle_vertical + (rand() % (perte_precision) - (perte_precision/2))) % 360;
    }
    
    int x = _x / Environnement ::scale,
	y = _y / Environnement ::scale;
    
    l -> spot(x, y);
    _hunter_fire -> play ();
    _fb -> init (/* position initiale de la boule */ _x, _y, 10.,
		 /* angles de visée */ angle_vertical, true_ang);
}


/*
 *	Clic droit: par défaut fait tomber le premier gardien.
 *
 *	Inutile dans le vrai jeu, mais c'est juste pour montrer
 *	une utilisation des fonctions « tomber » et « rester_au_sol »
 */
void Chasseur::right_click (bool shift, bool control) {
    if (shift)
	_l -> _guards [1] -> rester_au_sol ();
    else
	_l -> _guards [1] -> tomber ();
}


void Chasseur::hurt(){
    perte_precision ++;
    _pv--;
    message("PV : (%d, %d)", _pv, PVMAX);
    if(_pv == 0)
	die();
}

void Chasseur::die(){
    partie_terminee(false);
}
