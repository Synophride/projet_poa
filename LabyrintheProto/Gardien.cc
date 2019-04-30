#include "Gardien.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;

char Gardien::basic_decision(){
    if(! (l->is_spotted()))
	return EXPLORATION;
    int alive = l->nb_alive();
    
    if(alive < 2){
	if(rand()%4 == 0)
	    return ATTAQUE;
	return DEFENSE;
    } else if ( (_l->_nguards) > 2 * alive) {
	if(rand()%2 == 0)
	    return DEFENSE;
	return ATTAQUE;
    } else {
	if(rand()%4 == 0)
	    return DEFENSE;
	else return ATTAQUE;
    }
    
}

char Gardien::decision(){
    if(! (l -> is_spotted()) )
	return EXPLORATION;
    
    float a = 0;
    a = (float)rand() / (float)RAND_MAX;
    
    if (a > 0.9){
        float b = 0;
        b = (float)rand() / (float)RAND_MAX;

        if (b < 0.5)
            return 0;
        else 
            return 1;
    }

    /* if() //IF gardien dans le groupe près du trésor
        return 0;
    else 
    return 1; */
}

void Gardien::attaque(){
    int x = (_x) / Environnement::scale,
	y = (_y) / Environnement::scale;
    
    int estimated_dist = l -> dist_of_player(x, y);
	
    int d_xpos = l -> dist_of_player(x+1, y),
	d_xneg = l -> dist_of_player(x-1, y),
	d_ypos = l -> dist_of_player(x, y+1),
	d_yneg = l -> dist_of_player(x, y-1);

    if(d_xpos == estimated_dist -1){
	move(1, 0);
    } else if(d_xneg == estimated_dist -1){
	move(-1,0);
    } else if(d_ypos == estimated_dist -1){
	move(0,1);
    } else if(d_yneg == estimated_dist -1){
	move(0,-1);
    }
    
}

void Gardien::update(){
    if(dead)
	return;
    if(can_see_player())
	; //fire(0);
    attaque();
    return;
	
    if(reloading){
	reload ++;
	printf("Reloading...%d/%d\n", reload, RELOAD_TIME);
	if(reload == RELOAD_TIME){
	    reloading = false;
	    reload = 0;
	}
    }

    _tours_avant_question --;
    if(_tours_avant_question == 0){
	_strategie = basic_decision();
	_tours_avant_question = 2000;
    }
    
    switch(_strategie){
    case EXPLORATION:
	exploration();
	break;
    case ATTAQUE:
	move_to_player();
    case DEFENSE:
	move_to_treasure();
	break;
    }
}

void Gardien::exploration(){
    printf("J'explore wow \n");
}
bool Gardien::can_see_player(){
    float
	x_player = _l -> _guards[0]->_x,
	y_player = _l -> _guards[0]->_y,
	angle = atan2((x_player - _x), (y_player - _y) );

    _angle = ((int)-(angle * 180./M_PI)) % 360;
    
    for(float dist = 0; true; dist += 1.){	
	float
	    cur_x = sin(angle) * dist + _x,
	    cur_y = cos(angle) * dist + _y;
	
	int x = (int) (cur_x / Environnement :: scale),
	    y = (int) (cur_y / Environnement :: scale);
	
	if(_l->data(x, y) == JOUEUR){
	    return true;
	} else if (_l -> data(x, y) == WALL){
	    return false; 
	}	
    }
}


bool Gardien::move_to_treasure(){
    int x = (_x) / Environnement::scale,
	y = (_y) / Environnement::scale;

    cout << "Position du gardien : \t" << x << ',' << y << endl;
    
    int estimated_dist = l -> dist_of_treasure(x, y);
    
    cout << "Distance estimée : \t" << estimated_dist << endl;

    int d_xpos = l -> dist_of_treasure(x+1, y),
	d_xneg = l -> dist_of_treasure(x-1, y),
	d_ypos = l -> dist_of_treasure(x, y+1),
	d_yneg = l -> dist_of_treasure(x, y-1);
    
    cout << "Valeur du tableau vers x+ \t" << d_xpos << endl
	 << "Valeur du tableau vers x- \t" << d_xneg << endl
	 << "Valeur du tableau vers y+ \t" << d_ypos << endl
	 << "Valeur du tableau vers y- \t" << d_yneg << endl;

    if(d_xpos == estimated_dist -1){
	move(1, 0);
	cout << "Choix : x+" <<endl;
    } else if(d_xneg == estimated_dist -1){
	move(-1,0);
	cout << "Choix : x-" <<endl;
    } else if(d_ypos == estimated_dist -1){
	move(0,1);
	cout << "Choix : y+" <<endl;
    } else if(d_yneg == estimated_dist -1){
	move(0,-1);
	cout << "Choix : y-" <<endl;
    }
    
    return false;
}

bool Gardien::avancer(){
    double angle_rad = (_angle * M_PI)/180;
    double sped = 1;
    double dx = sped * (-sin(angle_rad));
    double dy = sped * (cos(angle_rad));
    return try_move(dx, dy);
}

// Appelé quand le gardien doit mourir
void Gardien::die(){
    message("je suis mort fdp");
    rester_au_sol();
    l->iamdying();
    dead=true;
    l->set_data(_x/Environnement::scale, _y / Environnement ::scale, EMPTY);    
}

bool Gardien::process_fireball(float dx, float dy){
    // calculer la distance entre le chasseur et le lieu de l'explosion.
    float   x = (_x - _fb -> get_x ()) / Environnement::scale;
    float   y = (_y - _fb -> get_y ()) / Environnement::scale;
    float   dist2 = x*x + y*y;

    int new_x = (int)((_fb -> get_x () + dx) / Environnement::scale),
	new_y = (int)((_fb -> get_y () + dy) / Environnement::scale);	    
    char contenu_case = _l -> data(new_x, new_y);
    if(EMPTY ==  contenu_case || GARDE == contenu_case){
		message ("Woooshh ..... %d", (int) dist2);
		// il y a la place.
		return true;
	}

    // collision...
    fired = false;
    // calculer la distance maximum en ligne droite.
    float dmax2 = (_l -> width ())*(_l -> width ()) + (_l -> height ())*(_l -> height ());
    if (JOUEUR == contenu_case){
	Labyrinthe * l = (Labyrinthe*) _l;
	l -> hurt_joueur();
    }
    return false;
}


void Gardien::hurt(){
    message("Aie fdp");
    perte_precision += 5;
    _pv--;
    if(_pv == 0)
	die();
    
}


bool Gardien::move(double dx, double dy){
    _angle = ( (int) (90 +  ((atan2(dy, dx)+M_PI) * 180) / M_PI)) % 360;
    return try_move(dx, dy) || try_move(dx, 0.0) || try_move(0.0, dy);
}


void Gardien::fire(int angle_vertical){
    if(fired || reloading)
	return;
    fired = true;
    reloading = true;
    int true_ang = - _angle;
    true_ang = (true_ang +  (rand() % (2 * perte_precision) - perte_precision)) % 360;
    angle_vertical = (angle_vertical +  (rand() % (perte_precision) - (perte_precision/2))) % 360;
    _fb -> init (/* position initiale de la boule */ _x, _y, 10.,
		 /* angles de visée */ angle_vertical, true_ang);
}

bool Gardien::is_legit_move(double dx, double dy){
    int new_x = ((_x + dx) / Environnement::scale);
    int new_y = ((_y + dy) / Environnement::scale);
    return (      new_x >= 0 
	       && new_x <  _l->width()
	       && new_y >= 0
	       && new_y <  _l->height()
	       && (EMPTY == _l->data(new_x, new_y) || GARDE == _l->data(new_x, new_y)));
}


bool Gardien::try_move(double dx, double dy ){
    if(! is_legit_move(dx, dy))
	return false;
    int old_x = _x / Environnement::scale,
	old_y = _y / Environnement::scale;

    _x += dx;
    _y += dy;

    int new_x = _x / Environnement::scale,
	new_y = _y / Environnement::scale;

    l -> set_data(old_x, old_y, EMPTY);
    l -> set_data(new_x, new_y, GARDE);
    
    return true;
}

bool Gardien::move_to_player(){
    int x = (_x) / Environnement::scale,
	y = (_y) / Environnement::scale;

    cout << "Position du gardien : \t" << x << ',' << y << endl;
    
    int estimated_dist = l -> dist_of_player(x, y);
    
    cout << "Distance estimée : \t" << estimated_dist << endl;

    int d_xpos = l -> dist_of_player(x+1, y),
	d_xneg = l -> dist_of_player(x-1, y),
	d_ypos = l -> dist_of_player(x, y+1),
	d_yneg = l -> dist_of_player(x, y-1);
    
    cout << "Valeur du tableau vers x+ \t" << d_xpos << endl
	 << "Valeur du tableau vers x- \t" << d_xneg << endl
	 << "Valeur du tableau vers y+ \t" << d_ypos << endl
	 << "Valeur du tableau vers y- \t" << d_yneg << endl;

    if(d_xpos == estimated_dist -1){
	move(1, 0);
	cout << "Choix : x+" <<endl;
    } else if(d_xneg == estimated_dist -1){
	move(-1,0);
	cout << "Choix : x-" <<endl;
    } else if(d_ypos == estimated_dist -1){
	move(0,1);
	cout << "Choix : y+" <<endl;
    } else if(d_yneg == estimated_dist -1){
	move(0,-1);
	cout << "Choix : y-" <<endl;
    }
    
    return false;
}


