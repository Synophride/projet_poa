#include "Gardien.h"
using namespace std;

int MIN = -1000000;
int MAX =  1000000;

struct node_comparator {
    bool operator()(const node & a, const node & b) const {
        return ((a.x < b.x) || (a.x == b.x && a.y < b.y));
    }
};

bool operator==(const node &a, const node &b){
    return a.x == b.x && a.y == b.y;
}

int get(map<node, int, node_comparator> &m, node &key, int def){
  if(m.count(key))
      return m[key];
  else
      return def;
}

list<node> build_solution(const node &goal,
			  map<node, node, node_comparator> &predecesseurs){
    list<node> ret = list<node>();
    node n = goal;
    while(! (n == predecesseurs[n])){
	ret.push_front(n);
	n = predecesseurs[n];
    }
    return ret;
}


int estimation_dist(const node &a, const node& b){
    return (int)
	sqrt(  ((float) a.x - b.x)*((float) a.x - b.x)
	     + ((float)a.y - b.y) * ((float)a.y - b.y));
}

// Bug probable : pas de trouvage étant donné que le trésor est sur une case où data = 1
// Implémentation du A* Algorithme
list<node> get_shortest_path(node start, node goal, Environnement* evt){
    
    int width = evt->width(),
	height= evt->height();
    
    set<node,node_comparator> finished_nodes=set<node, node_comparator>();
    set<node,node_comparator> frontiere=set<node, node_comparator>();
    frontiere.insert(start);

    map<node, node, node_comparator> pred = map<node, node, node_comparator>();
    pred[start] = start;
    
    map<node, int, node_comparator> dist_from_start = map<node, int, node_comparator>();
    map<node, int, node_comparator> estimated_dist= map<node, int, node_comparator>();
    dist_from_start[start] = 0;
    
    
    while(! frontiere.empty()){
	// 1. choisir le noeud de la frontière ayant la valeur estimée
	// la plus basse
	node current_node;
	int estimation_min = MAX;
	set<node, node_comparator>::iterator iter;
	for(iter = frontiere.begin(); iter != frontiere.end(); iter++){
	    node n = *iter;
	    int estimation = get(estimated_dist, n, -1);
	    if(estimation < estimation_min){
		current_node = n;
		estimation_min = estimation;
	    }
	}


	frontiere.erase(current_node);
	finished_nodes.insert(current_node);

	for(int i = 0; i<9; i++){ // Pê tester la présence du trésor ici 
	    node neighbor;
	    neighbor.x = current_node.x - 1 + (i/3);
	    neighbor.y = current_node.y - 1 + (i%3);
	    
	    if(neighbor == goal){
		return build_solution(current_node, pred);
	    }

	    if(evt->data(neighbor.x,neighbor.y) != EMPTY || finished_nodes.count(neighbor))
		continue;
	    
	    int dist = dist_from_start[current_node] + 1;
	    if(! (frontiere.count(neighbor)))
		frontiere.insert(neighbor);
	    else if (dist >= dist_from_start[neighbor])
		continue;
	    
	    // 
	    pred[neighbor] = current_node;
	    dist_from_start[neighbor] = dist;
	    estimated_dist[neighbor] = dist + estimation_dist(neighbor, goal);
	}
    }

    throw "Gardien.cc - Fin de la fonction A* et pas de chemin trouvé";
}

void Gardien::update(void){
    node curr_pos;
    curr_pos.x = (int)((_x) / Environnement::scale);
    curr_pos.y = (int)((_y) / Environnement::scale);
    
    if(!inited){
	printf("Calcul du pcc vers le trésor\n");	
	node goal;
	goal.x = _l->_treasor._x;
	goal.y = _l->_treasor._y;
	path_to_follow = get_shortest_path(curr_pos, goal, _l);
	inited =true;
    } else if(path_to_follow.empty()){
	printf("g fini de me déplassé gneu \n");
	_angle = (_angle + 5) % 360;
    } else {
	node n = path_to_follow.front();
	move_to(n);
	// Màj de la position courante
	curr_pos.x = (int)((_x) / Environnement::scale);
	curr_pos.y = (int)((_y) / Environnement::scale);

	if(curr_pos == n)
	    path_to_follow.pop_front();
	
    }
	
}

bool Gardien::is_legit_move(double dx, double dy){
    int new_x = (int)((_x + dx) / Environnement::scale);
    int new_y = (int)((_y + dy) / Environnement::scale);
    return (      new_x >= 0 
	       && new_x <  _l->width()
	       && new_y >= 0
	       && new_y <  _l->height()
	       && EMPTY == _l->data(new_x, new_y));
}

bool Gardien::move_to(node n){
    double x, y, cur_x, cur_y, dx, dy, norm;
    
    x = Environnement::scale * (double) n.x;
    y = Environnement::scale * (double) n.y;
    cout << "Moving to "<< x << ',' << y <<endl;
    
    cur_x =(double) _x;
    cur_y =(double) _y;
    
    cout << "position courante : "<< _x << '\t' << _y << endl;
    
    dx = x - cur_x;
    dy = y - cur_y;
    cout << dx << "\t" <<  dy << endl;
    norm = 1.2;
    cout << "norm : \t" << norm << endl;
    
    dx /= norm;
    dy /= norm;
    cout << dx << "\t" <<  dy << endl << endl;
    if(move(speed * dx, speed * dy))
	printf("Mové fdp\n");
    else
	printf("Pas changement \n");
}

bool Gardien::try_move(double dx, double dy ){
    if(! is_legit_move(dx, dy))
	return false;
    _x += dx;
    _y += dy;
    _angle = std::atan2(dx, dy) * 180;
    return true;
}

// 
bool Gardien::move(double dx, double dy){
    return try_move(dx, dy) || try_move(dx, 0.0) || try_move(0.0, dy);
}

void Gardien::fire(int angle_vertical){
    return;
}

bool Gardien::process_fireball(float dx, float dy){
    return false;
}
