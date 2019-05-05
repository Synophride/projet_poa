#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <list>
#include <iostream>
#include <string>
#include <vector>
#include <ios>
#include <map>
#include <fstream>
#include <unistd.h>

#include "Environnement.h"
using namespace std; 

const bool HORIZONTAL = false; //!< booléen permettant d'indiquer le sens des affiches
const bool VERTICAL   =  true; //!< booléen permettant d'indiquer le sens des affiches

const char WALL = 1;
const char BOX = 2;
const char GARDE  = 3;
const char JOUEUR = 4;
const char TREASURE = 5;

/// Structure représentant une coordonnée du labyrinthe 
struct coord{
    int x;
    int y;
};

/**
 * \brief fonction construisant à la fois le terrain et la liste des textures
 * \param path     (in)  : Le chemin d'accès au fichier du labyrinthe
 * \param terrain  (out) : Le tableau de string représentant le labyrinthe
 * \param tex_list (out) : La hashmap associant un chemin d'accès à un caractère
 */
void build_map(const char* path, vector<string> &terrain, map<char, string> &tex_list);

/**
 * \brief construit les listes
 * \param terrain     (in) : Le terrain/labyrinthe
 * \param guard_list (out) : La liste des coordonnées des gardes
 * \param wall_list  (out) : La liste des coordonnées des murs
 * \param text_list  (out) : La liste des textures (une texture = coordonnée, char-id de texture et un indicateur sur le sens de la texture)
 * \param box_list   (out) : La liste des boites
 * \param player_pos (out) : La position du joueur 
 * \param treasure_pos(out): La position du trésor
 **/
void parse_map(const vector<string> &terrain, 
	       list<coord> &guard_list,
	       list<pair<coord, coord>> &wall_list,
	       list<tuple<coord, char, bool >> &text_list,
	       list<coord> &box_list,
	       coord &player_pos,
	       coord &treasure_pos,
	       int &height,
	       int &width);

/**
 * \brief fait la liste des extrémités des murs partant de la coordonnée base_coord sur le terrain.
 * \param terrain     (in) : Le labyrinthe
 * \param base_coord  (in) : La coordonnée de départ du mur
 * \param extr_list   (out): La liste des extrémités des murs
 * \param text_list (inout): La liste des coordonnées+caractères+"verticalité" des textures
 **/
void parse_wall_extr(const vector<string> &terrain,
		     const coord &base_coord,
		     list<coord> &extr_list,
		     list<tuple<coord, char, bool>> &text_list);

/**
 * \brief fusionne les extrémités et la coordonnée de base, pour créer un ou deux murs
 * \param other_extr (in) : La liste des extrémités
 * \param wall_lst (inout): La liste des murs
 **/
void add_to_wall_list(const coord &base_coord,
		      list<coord> other_extr,
		      list<pair<coord, coord>> &wall_lst);

/**
 * \brief Décide si une ligne est considérée comme vide (commentaires inclus), id est ne contenant que des caractères espace (ou un commentaire)
 * \param str la chaîne de caractère testée
 * \return true si la chaîne est vide (uniquement des espaces ou des commentaires), false sinon
 */
bool is_blank(const string &str);

/**
 * \brief rend le premier caractère non-nul de la chaine, ou 0 si la chaine est vide
 * \param la str à tester
 */
char first_char(const string &str);

class Labyrinthe : public Environnement {

 private:
    char  **_data;	//!< indique si la case est libre ou occupée.
    int   lab_width;	//!< Dimension de l'axe 'x' du labyrinthe. 
    int   lab_height;	//!< Dimension de l'axe 'y' du labyrinthe.
    int   _nb_alive;    //!< Indique le nombre 
    
    vector<vector<int>> _dist_vect; //!< Vecteur contenant les distances entre chaque case et le trésor
    vector<vector<int>> _estimated_dist_player; //!< Vecteur contenant les distances estimées par les gardiens au joueur
    
    /**
     * \brief initialise _data[][], en fonction de lab_with et lab_height
     **/
    void init_data();

    /**
     * \brief remplit la matrice de distance au trésor, récursivement, 
     * en partant de la case de coordonnées (x, y) qu'on sait déjà remplie
     * 
     */
    void fill_dist(int x, int y, vector<vector<int>> &t);
    
    /**
     * \brief Initialise _guards et _nguard dans la classe
     * \param guards     (in) : une liste des coordonnées des gardes 
     * \param player_pos (in) : La coordonnée du joueur
     * 
     **/
    void build_guards(list<coord> guards, const coord &player_pos);

    /**
     * \brief Initialise les murs (_nwall et _walls[]) 
     * \param wall_list (in) : la liste des murs, un mur étant représenté par
     * une paire de coordonnées. 
     * Pour chaque paire de coordonnées (x1 y1) (x2 y2) de la liste, on fait 
     * l'hypothèse que 
     * La première coordonnées est "plus petite" que la seconde, id est 
     * (x1 < x2 || y1 < y2).
     **/
    void build_walls (list<pair<coord, coord>> wall_list);

    /**
     * \brief initialise les boites 
     * \param box_list (in) : liste des coordonnées des boites
     **/
    void build_boxes (list<coord> box_list);

    /**
     * \brief initialisation du trésor
     * \param c la coordonnée du trésor
     **/
    void build_treasure(coord c);

    /**
     * \brief initialisation des textures
     * \param text_map (in) : la structure associant un caractère représentant une texture 
     * dans le .txt du labyrinthe au nom de la texture
     * \param text_list (in): Une liste des textures, où chaque élément contient la coordonnée 
     * de la texture, le caractère représentant la texture, ainsi que l'orientation de la texture
     * (égale à HORIZONTAL ou VERTICAL)
     **/
    void build_text(map<char, string> text_map, list<tuple<coord, char, bool>> text_list);

    /**
     * \brief initialise le vecteur des distances. Doit être appellé à la fin du constructeur
     **/
    void init_vector_dist();

    /**
     * \brief initialise le vecteur des distances estimées vers le joueur
     **/
    void init_vector_playerdist();
    
    bool spotted = false; //!< Indique si le joueur a été vu par les gardiens
    int* _box_pv; //!< tableau indiquant le nombre de pv restant à chaque boite

    
    void echange_boites(int x);
    
 public:
    Labyrinthe();
    Labyrinthe (char*);

    /**
     * \brief Indique aux gardiens que le joueur a été 
     * détecté (= vu par un gardien ou ayant tiré une boule de feu)
     * , et met la matrice des distances au joueur
     * estimée par les gardiens à jour
     * \param x la coord x à laquelle le joueur a été détécté
     * \param y la coord y à laquelle le joueur a été vu
     
     **/
    void spot(int x, int y){
	spotted = true;
	maj_player_dist(x, y);
    }

    /**
     * \brief indique que la boite située aux coordonnées en paramètre a été touchée
     * par une boule de feu
     * \param x la coord en x de la boite touchée
     * \param y la coord en y de la boite touchée
     **/
    void hurt_box_at(int x, int y);

    /**
     * \brief indique si le joueur a été vu dans le labyrinthe durant la partie
     * \returns true si le joueur a été vu, false sinon
     **/
    bool is_spotted(){return spotted;}
    
    
    /**
     * \brief  retourne la largeur du labyrinthe. 
     */
    int width () { return lab_width;}

    /**
     * \brief retourne la longueur du labyrinthe. */
    int height () {return lab_height;}

    /**
     * \brief Informe sur l'état d'occupation de la case (i, j).
     * \returns WALL si la case est un mur,
     * BOX si la case est occupée par une boîte, 
     * GARDE si la case est occupée par un garde, 
     * JOUEUR si la case est occupée par le joueur, 
     * TREASURE si la case est occupée par le trésor
     **/
    char data (int i, int j){
	return _data [i][j];
    }

    /**
     * \brief indique au labyrinthe qu'un des gardes est mort
     **/
    void iamdying(){_nb_alive--;}

    /**
     * \brief Indique le nombre de gardiens encore en vie dans le labyrinthe 
     * \returns le nombre de gardes encore en vie.
     **/
    int nb_alive(){
	return _nb_alive;
    }
    
    /**
     * ·\brief indique que une instance de Fireball du joueur a explosé à l'endroit où était placé un (ou plusieurs) des gardiens. 
     * Va donc "Blesser" le gardien via la méthode Gardien::hurt()
     * \param x la composante dans l'axe des abcisses de la case dans laquelle la boule de feu explose
     * \parap y la composante dans l'axe des ordonnées de la case dans laquelle la boule de feu explose
**/
    void hurt_gardien_at(int x, int y);

    /**
     * \brief indique au labyrinthe que le joueur a été touché par une boule de 
     * feu provenant d'un des gardiens 
     *
     * Va donc appeller la méthode Chasseur::hurt()
     **/
    void hurt_joueur();

    /**
     * \brief change la valeur d'une case de la matrice _data, par exemple lors du déplacement d'un gardien
     * \param x 
     * \param y
     * \param value La nouvelle valeur de la case _data[x][y]. 
     * En théorie, compris entre EMPTY et TREASURE, mais dans les faits ce sera EMPTY, JOUEUR, ou GARDE, étant donné que seules ces entités se déplacent)
     **/
    void set_data(int x, int y, char value);

    /**
     * \brief rend la distance de la case courante 
     * \param x la coordonnée dans l'axe des abcisses
     * \param y la coordonnée dans l'axe des ordonnées
     * \returns -1 si la case est un mur, 
     * 1 000 000 s'il n'existe pas de chemin entre le point de coordonnées (x,y),
     * La distance en nombre de cases entre le point (x,y) et le trésor dans les autres cas
     **/
    int dist_of_treasure(int x, int y){
	return _dist_vect[x][y];
    }

    /**
     * \brief rend la distance estimée par les gardiens
     * entre la case de coord (x, y)
     * et le chasseur.
     * \param x
     * \param y
     * \returns La distance estimée par les gardiens entre le point donné en paramètre et 
     * la valeur réelle.
     **/
    int dist_of_player(int x, int y){
	return _estimated_dist_player[x][y];
    }

    /**
    * \brief met à jour les estimations de distances au joueur, en partant du principe que le 
    * joueur est aux coordonnéees (x, y)
    **/
    void maj_player_dist(int x, int y);
    
};

#endif
