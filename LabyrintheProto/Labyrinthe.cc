#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include <sstream>

Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

Environnement* Environnement::init (char* filename){
    return new Labyrinthe(filename);
}

bool is_blank(const string &str){
    for(int i = 0; i < str.length(); i++){
	char c = str[i];
	if(c == '#')
	    return true;
	else if( c != ' ' && c != '\t' && c != '\n' && c != '\0')
	    return false;
    }
    return true;
}

char first_char(const string &str){
    for(int i = 0; i<str.length(); i++){
	if(str[i] != ' ' && str[i] != '\t'){
	    return str[i];
	}
    }
    return 0;
}

void build_map(char* path, vector<string> &terrain, map<char, string> &tex_list){
    ifstream in = ifstream();
    string line = "";
    bool is_in_laby = false;
	
    in.open(path);
    terrain = vector<string>();
    tex_list = map<char, string>();
    
    while(getline(in, line)){
	if(is_blank(line)) // si ligne blanche, ne rien faire
	    continue;
	char c = first_char(line);
	
	if(c == '+' && !is_in_laby) // Début du labyrinthe
	    is_in_laby = true;
	
	if(is_in_laby) // Si on est dans le terrain ; ajout de la nouvelle ligne de terrain
	    terrain.push_back(line);
	else { 
	    string line_end = line.substr(2, line.length());
	    
	    for(int i = 0; i<line_end.length(); i++){
		if(line_end[i] == ' ' || line_end[i] == '\t' || line_end[i] =='#'){
		    tex_list[c] = line_end.substr(0, i);
		    break;
		}
	    }
	}
    }
}

void parse_wall_extr(const vector<string> &terrain,
		     const coord &base_coord,
		     list<coord> &extr_list,
		     list<tuple<coord, char,  bool>> &text_list){
    
    extr_list = list<coord>();
    
    for(int x = base_coord.x+1; x<terrain[base_coord.y].length(); x++){
	char c = terrain[base_coord.y][x];
	
	if( c == ' ' || c == 'T' || c == 'X' || c == 'C' || c == 'G')
	    break;
	
	else if( c == '+'){
	    coord co;
	    co.x = x;
	    co.y = base_coord.y;
	    extr_list.push_front(co);
	    break;
	} else if(c >= 'a' && c <= 'z'){
	    coord co;
	    co.x = x;
	    co.y = base_coord.y;
	    tuple<coord, char, bool> p = make_tuple(co, c, HORIZONTAL);
	    text_list.push_front(p);
	}
    }
    
    for(int y = base_coord.y+1; y<terrain.size(); y++){
        if(terrain[y].length() <= base_coord.x)
	    break;
	char c = terrain[y][base_coord.x];
	if( c == ' ' || c == 'T' || c == 'X' || c == 'C' || c == 'G')
	    break;
	else if( c == '+'){
	    coord co;
	    co.x = base_coord.x;
	    co.y = y;
	    extr_list.push_front(co);
	    break;
	} else if(c >= 'a' && c <= 'z'){
	    coord co;
	    co.x = base_coord.x;
	    co.y = y;
	    tuple<coord, char, bool> p = make_tuple(co, c, VERTICAL);
	    text_list.push_front(p);
	}
    }
}

void add_to_wall_list(const coord &base_coord,
		     list<coord> other_extr,
		     list<pair<coord, coord>> &wall_lst){
    
    while(! other_extr.empty()){
	coord other = other_extr.front();
	other_extr.pop_front();
	
	pair<coord, coord> p = make_pair(base_coord, other);
	wall_lst.push_front(p);
    }
}

void parse_map(const vector<string> &terrain, 
	       list<coord> &guard_list,
	       list<pair<coord, coord>> &wall_list,
	       list<tuple<coord, char, bool >> &text_list,
	       list<coord> &box_list,
	       coord &player_pos,
	       coord &treasure_pos,
	       int &height,
	       int &width){
    
    height = terrain.size();
    width = 0;
    guard_list= list<coord>();
    box_list  = list<coord>();
    wall_list = list<pair<coord, coord>>();
    text_list = list<tuple<coord, char, bool>>();
    
    
    for(int y = 0; y < terrain.size(); y++){
	int len = terrain[y].length();
	width = width<len ? len : width; // width <- max(width, len)
	
	for(int x = 0; x < terrain[y].length(); x++){
	    char kr = terrain[y][x];
	    coord c; c.x = x; c.y = y;
	    
	    switch(kr){
	    case '+': // extrémité de mur
	    {
		list<coord> extr_wall_list;
		parse_wall_extr(terrain, c, extr_wall_list, text_list);
		
		add_to_wall_list(c, extr_wall_list, wall_list);
	        break;
	    }
	    case 'G':
	    {
		guard_list.push_front(c);
		break;
	    }
	    case 'C':
	    {
		player_pos = c;
		break;
	    }
	    case 'T':
	    {
		treasure_pos = c;
		break;
	    }
	    case 'X':
	    {
		box_list.push_front(c);
		break;
	    }
	    default:
		break;
	    }
	    
	}
    }
}


Labyrinthe::Labyrinthe(char* path){
    vector<string> terrain;
    list<pair<coord, coord>> wall_list;
    list<coord> guard_list,
	  box_list;
    coord player_pos,
	  treasure_pos;
    map<char, string> text_map;
    list<tuple<coord, char, bool >> text_list;

    build_map(path, terrain, text_map);

    parse_map(terrain,
	      guard_list,
	      wall_list,
	      text_list,
	      box_list,
	      player_pos,
	      treasure_pos,
	      lab_height,
	      lab_width);

    init_data();
    build_guards(guard_list, player_pos);
    build_walls(wall_list);
    build_boxes(box_list);
    build_treasure(treasure_pos);
    build_text(text_map, text_list);
    init_vector_dist();
    
}

void Labyrinthe::init_data(){
    _data = new char* [lab_width];
    for (int i = 0; i < lab_width; ++i){
	_data [i] = new char [lab_height];
	for(int j = 0; j<lab_height; j++)
	    _data[i][j] = EMPTY;
    }
}

void Labyrinthe::build_guards(list<coord> guards, const coord &player_pos){
    _nguards = guards.size() + 1;
    _nb_alive = guards.size();
    _guards = new Mover* [_nguards];
    _guards[0] = new Chasseur(this);
    _guards[0]->_x = (float) scale * player_pos.x;
    _guards[0]->_y = (float) scale * player_pos.y;
    _data[player_pos.x][player_pos.y] = JOUEUR;
    int n = 1;
    
    while(!guards.empty()){
	coord c = guards.front();
	guards.pop_front();

	//_data[c.x][c.y] = 1;
	_guards[n] = new Gardien(this, "drfreak");
	_guards[n] -> _x = (float) scale * c.x;
	_guards[n] -> _y = (float) scale * c.y;
	_data[c.x][c.y] = GARDE;
	n++;
    }
}

void Labyrinthe::build_boxes(list<coord> box_list){
    _nboxes = box_list.size();
    _boxes = new Box[_nboxes];
    
    int n = 0;
    while(!box_list.empty()){
	coord c = box_list.front();
	box_list.pop_front();
	_boxes[n]._x = c.x;
	_boxes[n]._y = c.y;
	_boxes[n]._ntex = 0;
	_data[c.x][c.y] = BOX;
	
	n++;
    }
}


void Labyrinthe::build_walls(list<pair<coord, coord>> wall_list){
    _nwall = wall_list.size();
    _walls = new Wall [_nwall];
    int n = 0;
    printf("La liste de murs est de taille %d\n", _nwall);
    while(!wall_list.empty()){
	pair<coord, coord> wall_extrs = wall_list.front();
	wall_list.pop_front();
	
	coord c1 = get<0>(wall_extrs); // On a la certitude que cette coord = plus à haut ou plus en gauche
	coord c2 = get<1>(wall_extrs);
	
	_walls[n]._x1 = c1.x;
	_walls[n]._y1 = c1.y;
	_walls[n]._x2 = c2.x;
	_walls[n]._y2 = c2.y;	
	_walls[n]._ntex = 0;
	
	for(int y = c1.y; y<=c2.y; y++)
	    for(int x = c1.x; x<=c2.x; x++)
		_data[x][y] = WALL;
	    
	n++;
    }
}

void Labyrinthe::build_text(map<char, string> text_map, list<tuple<coord, char, bool>> text_list){
    _npicts = text_list.size();
    _picts = new Wall[_npicts];
    int n = 0;
    
    while(!text_list.empty()){
	tuple<coord, char, bool> elt = text_list.front();
	text_list.pop_front();

	coord c = get<0>(elt);
	char kr = get<1>(elt);
	bool incl=get<2>(elt);
	
	string path_texture = text_map[kr];

	char* chaine = (char*) path_texture.c_str();
	printf("%s\tchar=%c\n", chaine, kr);
	
	char tmp[128];
	sprintf (tmp, "%s/%s", texture_dir, chaine);

	int id_texture = wall_texture(tmp);
	
	if(incl == VERTICAL){
	    _picts[n]._x1 = c.x;
	    _picts[n]._y1 = c.y-1;

	    _picts[n]._x2 = c.x;
	    _picts[n]._y2 = c.y+1;
	} else {
	    _picts[n]._x1 = c.x-1;
	    _picts[n]._y1 = c.y;
	    
	    _picts[n]._x2 = c.x+1;
	    _picts[n]._y2 = c.y;
	}

	_picts[n]._ntex = id_texture;
	n++;
    }
}

void Labyrinthe::build_treasure(coord c){
    _treasor._x = c.x;
    _treasor._y = c.y;
    _data[c.x][c.y] = TREASURE;	
}

// Hypothèse : La distance du noeud courant a été remplie avant l'appel
void Labyrinthe::fill_dist(int x, int y){

    int current_dist = _dist_vect[x][y];
    
    if(   _dist_vect[x+1][y] != -1
       && current_dist + 1 < _dist_vect[x+1][y]){
	
	_dist_vect[x+1][y] = current_dist + 1;
	fill_dist(x+1, y);
    }
    if(   _dist_vect[x-1][y] != -1
       && current_dist + 1 < _dist_vect[x-1][y]){
	_dist_vect[x-1][y] = current_dist + 1;
	fill_dist(x-1, y);
    }
    if(   _dist_vect[x][y+1] != -1
       && current_dist + 1 < _dist_vect[x][y+1]){
	_dist_vect[x][y+1] = current_dist + 1;
	fill_dist(x, y+1);
    }

    if(   _dist_vect[x][y-1] != -1
       && current_dist + 1 < _dist_vect[x][y-1]){
	
	_dist_vect[x][y-1] = current_dist + 1;
	fill_dist(x, y-1);
    }
}


// 1. Calcul du vecteur de distances
void Labyrinthe::init_vector_dist(){
    // lab_width, lab_height
    
    int max_int = 1000000;
    _dist_vect = vector<vector<int>>(lab_width);
    for(int i = 0; i< lab_width; i++){
	_dist_vect[i] = vector<int>(lab_height);
    }
    
    for(int i = 0; i < lab_width; i++){
	for(int j = 0; j < lab_height; j++){
	    char val = _data[i][j];
	    if(val == EMPTY || val == GARDE || val == JOUEUR)
		_dist_vect[i][j] = max_int;
	    else
		_dist_vect[i][j] = -1;
	}
    }
    
    _dist_vect[_treasor._x][_treasor._y] = 0;
    fill_dist(_treasor._x, _treasor._y);    
}

void Labyrinthe::set_data(int x, int y, char value){
    _data[x][y] = value;
}
void Labyrinthe::hurt_joueur(){
    Chasseur * c = (Chasseur*) _guards[0];
    c->hurt();
}
void Labyrinthe::hurt_gardien_at(int x, int y){
    for(int i = 1; i < _nguards; i++){
	int guard_x = _guards[i]->_x / scale,
	    guard_y = _guards[i]->_y / scale;
	
	if(guard_x == x && guard_y == y){
	    Gardien* g = (Gardien*) _guards[i];
	    g->hurt();
	}
	    
	 
	
    }
}
    
