#include "Gardien.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;

float m = 1;

bool Gardien::close_of_treasure(){
    float tx =(float) _l->_treasor._x,
	ty = (float)_l->_treasor._y;

    float gx = _x / Environnement :: scale,
	gy = _y / Environnement :: scale;

    return (sqrt((gx - tx) * (gx - tx) + (gy - ty) * (gy - ty)) < 10);
}

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
    float seuil_haut = 3;
    float seuil_bas  = -3;
    float rand_modif = 5.;
    float random = 2 * rand_modif * (float) (rand()/RAND_MAX);
    float modif = random - rand_modif;
    float pot_d = get_potentiel_defense();

    if(pot_d + modif > seuil_bas)
	return DEFENSE;
    else if (pot_d + modif < seuil_haut)
	return ATTAQUE;
    else return EXPLORATION;
	    
    float a = (float)rand() / (float)RAND_MAX;
    
    if (a > 0.9){
        float b = 0;
        b = (float)rand() / (float)RAND_MAX;

        if (b < 0.5)
            return DEFENSE;
        else 
            return ATTAQUE;
    }
    
    if(close_of_treasure()) //IF gardien dans le groupe près du trésor
        return DEFENSE;
    else 
	return ATTAQUE; 
}


float Gardien::get_potentiel_defense(){
    float LOWER_BOUND = 0;
    float UPPER_BOUND = 0;
    int x = (_x/Environnement::scale);
    int y = (_y/Environnement::scale);
    
    float distance_tresor((float) l -> dist_of_treasure(x, y));
    float dist_player((float) l ->dist_of_player(x, y));
    float nb_dead_guards((float) (l->_nguards - l->nb_alive()));
    float nb_guards_alive= (float) (l->nb_alive());
    float dist_chass_treasure((float)
			      (l->dist_of_treasure(
						   l -> _guards[0]->_x/Environnement::scale,
						   l -> _guards[0]->_y/Environnement::scale) ));
    float potentiel = 0;
    potentiel =
	distance_tresor/10
	+ nb_dead_guards * 5
	- nb_guards_alive * 6
	- dist_chass_treasure / 7;
    return potentiel;

}


void Gardien::update(){    
    if(dead)
	return;
    if(can_see_player()){
	int x_player = _l -> _guards[0] -> _x / Environnement :: scale,
	    y_player = _l -> _guards[0] -> _y / Environnement :: scale;
	if(l -> dist_of_player(x_player, y_player) > 15)
	    l -> maj_player_dist(x_player, y_player);
	watch_player();
	fire(0);
    }
    
    if(reloading){
	reload ++;
	if(reload == RELOAD_TIME){
	    reloading = false;
	    reload = 0;
	}
    }

    _tours_avant_question --;
    
    if(_tours_avant_question == 0){
	_strategie = decision();
	_tours_avant_question = rand() % 2000 + 500;
    }

    switch(_strategie){
    case EXPLORATION:
	exploration();
	break;
    case ATTAQUE:
	move_to_player();
	
	break;
    case DEFENSE:
	move_to_treasure();
	break;
    }
}

void Gardien::exploration(){
    if(!avancer())
	_angle = rand()%360;
}

void Gardien :: watch_player(){
    float
	x_player = _l -> _guards[0]->_x,
	y_player = _l -> _guards[0]->_y,
	angle = atan2((x_player - _x), (y_player - _y) );
    
    _angle = ((int)-(angle * 180./M_PI)) % 360;

}

bool Gardien::can_see_player(){
    float
	x_player = _l -> _guards[0]->_x,
	y_player = _l -> _guards[0]->_y,
	angle = atan2((x_player - _x), (y_player - _y) );


    
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

    int estimated_dist = l -> dist_of_treasure(x, y);    

    int d_xpos = l -> dist_of_treasure(x+1, y),
	d_xneg = l -> dist_of_treasure(x-1, y),
	d_ypos = l -> dist_of_treasure(x, y+1),
	d_yneg = l -> dist_of_treasure(x, y-1);

    int min = estimated_dist;
    float move_x = 0;
    float move_y = 0;
    
    if( l -> dist_of_treasure(x+1, y) < min && l -> dist_of_treasure(x+1, y) != -1){
	move_x = 1; move_y = 0;
	min = l -> dist_of_treasure(x+1, y);
    }
    if(l -> dist_of_treasure(x-1, y) < min && l -> dist_of_treasure(x-1, y) != -1){
	move_x = -1; move_y = 0;
	min = l -> dist_of_treasure(x-1, y);
    }
    if(l -> dist_of_treasure(x, y+1) < min && l -> dist_of_treasure(x, y+1) != -1){
	move_x = 0; move_y = 1;
	min = l -> dist_of_treasure(x, y+1);
    }
    if(l -> dist_of_treasure(x, y-1) < min && l -> dist_of_treasure(x, y-1) != -1){
	move_x = 0;
	move_y = -1;
    	min = l -> dist_of_treasure(x, y-1);
    }
    printf("moving to treasure, move = (%f, %f)\n", move_x, move_y);
    return move(move_x, move_y);
    
    // Pê virer ça plus tard
    // Diagonales
    if( l-> dist_of_treasure(x+1, y+1) < min && l -> dist_of_treasure(x+1, y+1) != -1){
	move_x = m;
	move_y = m;
	min = l-> dist_of_treasure(x+1, y+1);
    }
    if( l-> dist_of_treasure(x+1, y-1) < min && l -> dist_of_treasure(x+1, y-1) != -1){
	move_x =  m;
	move_y = -m;
	min = l-> dist_of_treasure(x+1, y-1);
    }
    
    if( l-> dist_of_treasure(x-1, y+1) < min && l -> dist_of_treasure(x-1, y+1) != -1){
	move_x = -m;
	move_y = m;
	min = l-> dist_of_treasure(x-1, y+1);
    }
    
    if( l-> dist_of_treasure(x-1, y-1) < min && l -> dist_of_treasure(x-1, y-1) != -1){
	move_x = -m;
	move_y = m;
	min = l-> dist_of_treasure(x-1, y+1);
    }

    move(move_x, move_y);
    return true;
}

bool Gardien::avancer(){
    double angle_rad = (_angle * M_PI)/180;
    double sped = 1;
    double dx = sped * (-sin(angle_rad));
    double dy = sped * (cos(angle_rad));
    return try_move(dx, dy);
}


void Gardien::die(){
    rester_au_sol();
    l->iamdying();
    dead=true;
    l->set_data(_x/Environnement::scale, _y / Environnement ::scale, EMPTY);    
}

bool Gardien::process_fireball(float dx, float dy){
    float   x = (_x - _fb -> get_x ()) / Environnement::scale;
    float   y = (_y - _fb -> get_y ()) / Environnement::scale;
    float   dist2 = x*x + y*y;

    int new_x = (int)((_fb -> get_x () + dx) / Environnement::scale),
	new_y = (int)((_fb -> get_y () + dy) / Environnement::scale);	    
    char contenu_case = _l -> data(new_x, new_y);
    if(EMPTY ==  contenu_case || GARDE == contenu_case)
	return true;

    fired = false;
    if (JOUEUR == contenu_case){
	Labyrinthe * l = (Labyrinthe*) _l;
	l -> hurt_joueur();
    }
    return false;
}


void Gardien::hurt(){

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

    int estimated_dist = l -> dist_of_player(x, y);

    int min = estimated_dist;
    float move_x = 0;
    float move_y = 0;
    
    if(    l -> dist_of_player(x+1, y) <  min
	&& l -> dist_of_player(x+1, y) != -1 ){
	
	move_x = 1; move_y = 0;
	min = l -> dist_of_player(x+1, y);
    }
    if(    l -> dist_of_player(x-1, y) <  min
	&& l -> dist_of_player(x-1, y) != -1){
	move_x = -1; move_y = 0;
	min = l -> dist_of_player(x-1, y);
    }
    if(   l -> dist_of_player(x, y+1) < min
       && l -> dist_of_player(x, y+1) != -1){
	
	move_x = 0; move_y = 1;
	min = l -> dist_of_player(x, y+1);
    }
    if(   l -> dist_of_player(x, y-1) <  min
       && l -> dist_of_player(x, y-1) != -1){
	
	move_x = 0; move_y = -1;
    	min = l -> dist_of_player(x, y-1);
    }
    
    printf("attack : moving(%f, %f) \n", move_x, move_y);
    return move(move_x, move_y);
    // Diagonales
    if( l-> dist_of_player(x+1, y+1) < min
	&& l -> dist_of_player(x+1, y+1) != -1){
	
	move_x = m;
	move_y = m;
	min = l-> dist_of_player(x+1, y+1);
    }
    if(    l -> dist_of_player(x+1, y-1) < min
	&& l -> dist_of_player(x+1, y-1) != -1){
	
	move_x =  m;
	move_y = -m;
	min = l-> dist_of_player(x+1, y-1);
    }
    if(    l -> dist_of_player(x-1, y+1) < min
	&& l -> dist_of_player(x-1, y+1) != -1){
	
	move_x = -m;
	move_y = m;
	min = l-> dist_of_player(x-1, y+1);
    }
    if(    l -> dist_of_player(x-1, y-1) <  min
	&& l -> dist_of_player(x-1, y-1) != -1){
	
	move_x = -m;
	move_y = m;
	min = l-> dist_of_player(x-1, y+1);
    }

    return move(move_x, move_y);
}


