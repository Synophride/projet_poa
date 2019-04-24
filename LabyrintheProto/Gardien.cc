#include "Gardien.h"


using namespace std;

bool cmp(const node a, const node &b){
    return ((a.x < b.x) || (a.x == b.x && a.y < b.y));
}


struct node_comparator {
    bool operator()(const node & a, const node & b) const {
        return ((a.x < b.x) || (a.x == b.x && a.y < b.y));
    }
};


//bool operator<(const node &a, const node &b){
//    return cmp(a, b);
//}

int get(map<node, int> &m, node &key, int def){
    if(m.count(key))
	return m[key];
    else
	return def;
}


list<node> build_solution(){

}

// Implémentation du A* Algorithme
list<node> get_shortest_path(node start, node goal,
			     char** data, int height, int width){
    set<node,node_comparator> finished_nodes;
    set<node,node_comparator> frontiere;
    // frontiere.insert(start);
    /*  map<node, node, node_comparator> pred;
    
    pred[start] = start;
    
    map<node, int, node_comparator> dist_from_start;

    map<node, int, node_comparator> estimated_dist;
    
    while(! frontiere.empty()){
	// 1. choisir le noeud de la frontière ayant la valeur estimée
	// la plus basse
	node current_node;
	int distance = -100000;
	//	auto it = frontiere.begin();
	//for(node n : it){
	//  int new_d = get(estimated_dist, n, 100000);
	//}
		
	} */
}

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
