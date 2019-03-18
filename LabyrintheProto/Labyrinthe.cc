#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#include <sstream>


Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

Environnement* Environnement::init (char* filename){
    return new Labyrinthe (filename);
}

/**
 * EXEMPLE de labyrinthe.
 */
Labyrinthe::Labyrinthe (){
    // taille du labyrinthe.
    lab_height = 80;
    lab_width = 25;
    
    // les murs: 4 dans cet EXEMPLE!
    int n = 0;
    
    _walls = new Wall [4];
    
    // le premier.
    _walls [n]._x1 = 0;
    _walls [n]._y1 = 0; 

    _walls [n]._x2 = 0;
    _walls [n]._y2 = lab_height-1;

    _walls [n]._ntex = 0;
    ++n;

    // le deuxieme.
    _walls [n]._x1 = 0; _walls [n]._y1 = lab_height-1;
    _walls [n]._x2 = lab_width-1; _walls [n]._y2 = lab_height-1;
    _walls [n]._ntex = 0;
    ++n;

    // le troisieme.
    _walls [n]._x1 = lab_width-1; _walls [n]._y1 = lab_height-1;
    _walls [n]._x2 = lab_width-1; _walls [n]._y2 = 0;
    _walls [n]._ntex = 0;
    ++n;

    // le dernier.
    _walls [n]._x1 = lab_width-1; _walls [n]._y1 = 0;
    _walls [n]._x2 = 0; _walls [n]._y2 = 0;
    _walls [n]._ntex = 0;
    ++n;
    _nwall = n;

    // une affiche.
    // (attention: pour des raisons de rapport d'aspect, les affiches doivent faire 2 de long)
    n = 0;
    _picts = new Wall [2];
    // la première (texture par défaut).
    _picts [n]._ntex = 0;
    _picts [n]._x1 = 0; _picts [n]._y1 = 4;
    _picts [n]._x2 = 0; _picts [n]._y2 = 6;
    ++n;

    // la deuxième a une texture différente.
    char	tmp [128];
    sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");

    _picts [n]._ntex = wall_texture (tmp);
    _picts [n]._x1 = 0; _picts [n]._y1 = 8;
    _picts [n]._x2 = 0; _picts [n]._y2 = 10;
    ++n;
    _npicts = n;

    // 3 caisses.
    _boxes = new Box [3];

    n = 0;
    // la première.
    _boxes [n]._x = 12; _boxes [n]._y = 70; _boxes [n]._ntex = 0;
    ++n;
    // la deuxieme.
    _boxes [n]._x = 5; _boxes [n]._y = 10; _boxes [n]._ntex = 0;
    ++n;
    // la dernière.
    _boxes [n]._x = 22; _boxes [n]._y = 65; _boxes [n]._ntex = 0;
    ++n;
    _nboxes = n;

    // création du tableau d'occupation du sol.
    _data = new char* [lab_width];
    for (int i = 0; i < lab_width; ++i)
	_data [i] = new char [lab_height];
    // initialisation du tableau d'occupation du sol.
    for (int i = 0; i < lab_width; ++i)
	for (int j = 0; j < lab_height; ++j) {
	    // murs sur les bords.
	    if (i == 0 || i == lab_width-1 || j == 0 || j == lab_height-1)
		_data [i][j] = 1;
	    else
		// rien dedans.
		_data [i][j] = EMPTY;
	}

    // indiquer qu'on ne marche pas sur une caisse.
    _data [_boxes [0]._x][_boxes [0]._y] = 1;
    _data [_boxes [1]._x][_boxes [1]._y] = 1;
    _data [_boxes [2]._x][_boxes [2]._y] = 1;

    // coordonnées du trésor.
    _treasor._x = 10;
    _treasor._y = 10;
    // indiquer qu'on ne marche pas dessus.
    _data [_treasor._x][_treasor._y] = 1;

    // le chasseur et les 4 gardiens.
    _nguards = 1+4;
    _guards = new Mover* [_nguards];
    _guards [0] = new Chasseur (this);
    _guards [1] = new Gardien (this, "drfreak");
    _guards [2] = new Gardien (this, "Marvin");
    _guards [2] -> _x = 90.;
    _guards [2] -> _y = 70.;

    _guards [3] = new Gardien (this, "Potator");
    _guards [3] -> _x = 60.;
    _guards [3] -> _y = 10.;
	
    _guards [4] = new Gardien (this, "garde"); _guards [4] -> _x = 130.; _guards [4] -> _y = 100.;

    // indiquer qu'on ne marche pas sur les gardiens.
    _data [(int)(_guards [1] -> _x / scale)][(int)(_guards [1] -> _y / scale)] = 1;
    _data [(int)(_guards [2] -> _x / scale)][(int)(_guards [2] -> _y / scale)] = 1;
    _data [(int)(_guards [3] -> _x / scale)][(int)(_guards [3] -> _y / scale)] = 1;
    _data [(int)(_guards [4] -> _x / scale)][(int)(_guards [4] -> _y / scale)] = 1;
}


// string, vector, fstream
std::vector<std::string> make_str_vect(char* path){
    std::ifstream fd;
    std::string line;
    // "while pas fin du fichier"
    fd.open(path);
    fd >> line;
    
    std::vector<std::string> ret = std::vector<std::string>();
    
    while(std::getline(fd, line))
	{
    std::stringstream   linestream(line);
    std::string         data;
    
    std::getline(linestream, data, '\n');
    ret.push_back(data);

	
	}
    fd.close();
    return ret;
    throw "pas implémenté";
}


std::list<coord> mk_wall_list(const coord &base_coord,
			      const std::vector<std::string> &laby){
    int x = base_coord.x,
	y = base_coord.y;
    std::list<coord> ret = {};
    std::string line = laby[x];
    
    // 1. On regarde vers la droite du point en paramètre
    for(int j = x+1; j < line.length(); j++){
	if(line[j] == '+'){
	  coord c;
	    c.x = j;
	    c.y = y;
	    ret.push_front(c);
	    break;
	} else if( !( line[j] > 'a' && line[j] < 'z'  ||  line[j] == '-')){ 
	    // cas où le mur ne continue pas. Fin de la boucle
	    break;
	}
    }

    // 2. On regarde en dessous du point en paramètre
    for(int i = x+1; i<laby.size(); i++){
	if(laby[i].length() <= x) // cas a priori inexistant
	    break;
	
	if(laby[i][x] == '+'){ // Fin du mur
	    coord c;
	    c.x = x;
	    c.y = i;
	    ret.push_front(c);
	    break;
	} else if( !( (laby[i][x] > 'a' && laby[i][x] < 'z')  ||  laby[i][x] == '|')){ 
	    // cas où le mur ne continue pas.
	    break;
	}
    }
    return ret;
}

bool is_blank(const std::string &str){
    for(int i = 0; i < str.length(); i++){
	char c = str[i];
	if(c == '#')
	    return true;
	else if( c != ' ' && c != '\t' && c != '\n')
	    return false;
    }
    return true;
}


Labyrinthe::Labyrinthe(char* filename){
    std::string line;    
    std::vector<std::string> laby_lines;
    std::map<char, std::string> affiches; // associe à chaque caractère le chemin d'accès à l'affiche
    std::list<coord> guard_list = {}; 
    std::list<coord> box_list = {};
    std::list<std::pair<coord, coord>> wall_list = {}; // Coordonnées des murs
    coord player_pos;
    coord treasure_pos;
    lab_height = 0;
    lab_width = 0;
  
    // chargement du fichier
    laby_lines = make_str_vect(filename);
    bool is_laby = false;
    
    /* Construction de :
       lab_height, lab_width
       Liste des G
       Liste des B 
       Map (char -> LinkedList coord) (liste des affiches)
    */
    for(int i=0; i<laby_lines.size(); i++){
	std::string line = laby_lines[i];
	if(line[0] == '+') // Todo : changer ce truc pour "si le premier kr de la ligne est +"
	    is_laby = true;
	
	// Todo : calculer la width correctement
	if(is_laby){
	    if(!is_blank(line))
		lab_height++;
	    
	    for(int j = 0; j<line.length(); j++){
		char c = line[j];
		if (c == '#')
		    break;
		coord co;
		
		switch(c){
		case '+': // Début de mur      
		{
		    co.y = i;
		    co.x = j;
		    std::list<coord> coord_list = mk_wall_list(co, laby_lines);
		    while(!coord_list.empty()){
			coord wall_end = coord_list.front();
			coord_list.pop_front();
			wall_list.push_front(std::make_pair(co, wall_end));
		
		    }
			break;
		}
		case 'G':
		{
		    co.y = i;
		    co.x = j;
		    guard_list.push_front(co);
		    break;
		}
		case 'X':
		{
		    co.y = i;
		    co.x = j;
		    box_list.push_front(co);
		    break;
		}   
		case 'C':
		{
		    player_pos.y = j;
		    player_pos.x = i;
		    break;
		}
		case 'T':
		{
		    treasure_pos.y = j;
		    treasure_pos.x = i;
		    break;
		}
		default:
		{
		    break;
		}
		} // switch
	    } // for (krs)
	} else if(is_blank(laby_lines[i])){
	    continue;
	} else {
	    // TODO : parsing de la ligne de la forme [<char> \t <str>]. 
	} // else
    } // for (lignes)
    
    // 2 : on a toutes les listes. On construit maintenant le labyrinthe
    // Construction des murs
    _walls = new Wall[wall_list.size()];
    int cpt = 0;
    while(! wall_list.empty()){
	std::pair<coord, coord> paire_coord = wall_list.front();
	wall_list.pop_front();
	coord c1 = std::get<0>(paire_coord),
	    c2 = std::get<1>(paire_coord);
	_walls[cpt]._x1 = c1.x;
	_walls[cpt]._y1 = c1.y;
	_walls[cpt]._x2 = c2.x;
	_walls[cpt]._y2 = c2.y;
	_walls[cpt]._ntex = 0;
	cpt++;
    }

    // Dépôt des boîtes
    _boxes = new Box[box_list.size()];
    cpt = 0;
    while(!box_list.empty()){
	coord c = box_list.front();
	box_list.pop_front();
	_boxes[cpt]._x = c.x;
	_boxes[cpt]._y = c.y;
	_boxes[cpt]._ntex = 0;
	cpt++;
    }

    // Gardiens
    _guards = new Mover*[guard_list.size() +1];
    _guards[0] = new Chasseur(this);
    _guards[0]->_x = player_pos.x;
    _guards[0]->_y = player_pos.y;
    
    cpt = 1;
    while(!guard_list.empty()){
	coord c = guard_list.front();
	guard_list.pop_front();
	_guards[cpt] = new Gardien(this, "Potator"); // Todo : génération aléatoire de gardien
	_guards[cpt]->_x = c.x;
	_guards[cpt]->_y = c.y;
    }

    // TODO : affiches, réglage des tableaux d'occupation du sol
    // Occupation des murs
}
