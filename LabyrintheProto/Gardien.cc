#include "Gardien.h"

using namespace std;

void Gardien::update(){
    move_to_treasure();
}

bool Gardien::move_to_treasure(){
    Labyrinthe * l = (Labyrinthe*) _l;
    
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

bool Gardien::move(double dx, double dy){
    _angle = ( (int) (90 +  ((atan2(dy, dx)+M_PI) * 180) / M_PI)) % 360;
    return try_move(dx, dy) || try_move(dx, 0.0) || try_move(0.0, dy);
}

void Gardien::fire(int angle_vertical){
    return;
}

bool Gardien::process_fireball(float dx, float dy){
    return false;
}

bool Gardien::is_legit_move(double dx, double dy){
    int new_x = ((_x + dx) / Environnement::scale);
    int new_y = ((_y + dy) / Environnement::scale);
    return (      new_x >= 0 
	       && new_x <  _l->width()
	       && new_y >= 0
	       && new_y <  _l->height()
	       && EMPTY == _l->data(new_x, new_y));
}

bool Gardien::try_move(double dx, double dy ){
    if(! is_legit_move(dx, dy))
	return false;
    _x += dx;
    _y += dy;
    return true;
}
