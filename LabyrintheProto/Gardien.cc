#include "Gardien.h"


void Gardien::update(void){
    move((double) (rand()%100) / 100. , ((double) (rand()%100) / 100. ));
}

bool Gardien::is_legit_move(double dx, double dy){
    return EMPTY == _l -> data(
		(int)((_x + dx) / Environnement::scale),
		(int)((_y + dy) / Environnement::scale));
}

bool Gardien::try_move(double dx, double dy ){
    if(! is_legit_move(dx, dy))
	return false;
    _x += dx;
    _y += dy;
    return true;    
}

bool Gardien::move(double dx, double dy){
    return try_move(dx, dy);// || try_move(dx, 0.0) || try_move(0.0, dy);
}

void Gardien::fire(int angle_vertical){
    return;
}

bool Gardien::process_fireball(float dx, float dy){
    return false;
}
