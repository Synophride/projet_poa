#include "Gardien.h"

#include <cstdlib>
#include <cmath>

void Gardien::update(void){
    // move((double) (rand()%100) / 100. , ((double) (rand()%100) / 100. ));
    float speed = 1.;
/*    bool moved =
	   move(speed * std::cos(( _angle) /180),
		speed * std::sin(( _angle) /180))
      	|| move(speed * std::cos(((( 180 + _angle)%180) /180 )),
	speed * std::sin(((( 180 + _angle)%180) /180 )));*/
    double dx = (double)(rand()%10 - 5) / 10.,
	   dy = (double)(rand()%10 - 5) / 10.;
    move(dx, dy);
    // Todo 
}

bool Gardien::is_legit_move(double dx, double dy){
    int new_x = (int)((_x + dx) / Environnement::scale);
    int new_y = (int)((_y + dy) / Environnement::scale);
    return (new_x >= 0 
	    && new_x < _l->width()
	    && new_y >= 0
	    && new_y < _l->height()
	    && (EMPTY == _l -> data(new_x, new_y)));
}

bool Gardien::try_move(double dx, double dy ){
    if(! is_legit_move(dx, dy))
	return false;
    _x += dx;
    _y += dy;
    _angle = std::atan2(dx, dy) * 180;
    return true;
}

bool Gardien::move(double dx, double dy){
    return try_move(dx, dy) || try_move(dx, 0.0) || try_move(0.0, dy);
}

void Gardien::fire(int angle_vertical){
    return;
}

bool Gardien::process_fireball(float dx, float dy){
    return false;
}
