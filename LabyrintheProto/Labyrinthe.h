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
#include "Environnement.h"
using namespace std;

const bool HORIZONTAL = false;
const bool VERTICAL   =  true;

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
    int   lab_width;	//!< dimensions du rectangle.
    int   lab_height;	//!<  englobant le labyrinthe.

    /**
     * \brief initialise _data[][], en fonction de lab_with et lab_height
     **/
    void init_data();

    /**
     * \brief Initialise _guards et _nguard dans la classe
     * \param guards     (in) : une liste des coordonnées des gardes 
     * \param player_pos (in) : La coordonnée du joueur
     * TODO : génération de gardes différents
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
     * 
     **/
    void build_text(map<char, string> text_map, list<tuple<coord, char, bool>> text_list);
    
public:
    Labyrinthe();
    Labyrinthe (char*);

    /// \brief  retourne la largeur du labyrinthe.
    int width () { return lab_width;}

    // \brief retourne la longueur du labyrinthe.
    int height () { return lab_height;}	
    // retourne l'état (occupation) de la case (i, j).
    char data (int i, int j){
	return _data [i][j];
    }
};

#endif
