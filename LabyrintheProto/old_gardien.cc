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

// Sur 8
int Gardien::get_direction(int x, int y){
    Labyrinthe * l = (Labyrinthe*) _l;
    // Passer à huit ? 
    int dist_courante = l -> dist_of_treasure(x, y);
    int dist_min = l -> dist_of_treasure(x, y+1);
    int	direction = 0;
    
    if(dist_min == -1 || dist_min > l -> dist_of_treasure(x+1, y)){
	dist_min = l -> dist_of_treasure(x+1, y);
	direction = 1;	
    }
    if(dist_min == -1 || dist_min > l -> dist_of_treasure(x, y-1)){
	dist_min = l -> dist_of_treasure(x, y-1);
	direction = 2;	
    }
    if(dist_min == -1 || dist_min > l -> dist_of_treasure(x-1, y)){
	dist_min = l -> dist_of_treasure(x-1, y);
	direction = 3;
    }

    return direction;
}


void Gardien::update(void){
    int curr_x = (int)((_x + .5) / Environnement::scale);
    int curr_y = (int)((_y + .5) / Environnement::scale);
    int d = get_direction(curr_x, curr_y);
    switch(d){
    case 0:
	move(0, speed);
	break;
    case 1:
	move(speed, 0);
	break;
    case 2:
	move(-speed, 0);
	break;
    case 3:
	move(0, -speed);
    default:
	break;
    }    
    return;
    
    node curr_pos;
    curr_pos.x = 0;
    curr_pos.y = 0;
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


