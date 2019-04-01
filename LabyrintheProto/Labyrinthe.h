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
 * \brief fonction construisant � la fois le terrain et la liste des textures
 * \param path     (in)  : Le chemin d'acc�s au fichier du labyrinthe
 * \param terrain  (out) : Le tableau de string repr�sentant le labyrinthe
 * \param tex_list (out) : La hashmap associant un chemin d'acc�s � un caract�re
 */
void build_map(const char* path, vector<string> &terrain, map<char, string> &tex_list);

/**
 * \brief construit les listes
 * \param terrain     (in) : Le terrain/labyrinthe
 * \param guard_list (out) : La liste des coordonn�es des gardes
 * \param wall_list  (out) : La liste des coordonn�es des murs
 * \param text_list  (out) : La liste des textures (une texture = coordonn�e, char-id de texture et un indicateur sur le sens de la texture)
 * \param box_list   (out) : La liste des boites
 * \param player_pos (out) : La position du joueur 
 * \param treasure_pos(out): La position du tr�sor
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
 * \brief fait la liste des extr�mit�s des murs partant de la coordonn�e base_coord sur le terrain.
 * \param terrain     (in) : Le labyrinthe
 * \param base_coord  (in) : La coordonn�e de d�part du mur
 * \param extr_list   (out): La liste des extr�mit�s des murs
 * \param text_list (inout): La liste des coordonn�es+caract�res+"verticalit�" des textures
 **/
void parse_wall_extr(const vector<string> &terrain,
		     const coord &base_coord,
		     list<coord> &extr_list,
		     list<tuple<coord, char, bool>> &text_list);

/**
 * \brief fusionne les extr�mit�s et la coordonn�e de base, pour cr�er un ou deux murs 
 * \param other_extr (in) : La liste des extr�mit�s
 * \param wall_lst (inout): La liste des murs
 **/
void add_to_wall_list(const coord &base_coord,
		      list<coord> other_extr,
		      list<pair<coord, coord>> &wall_lst);

/**
 * \brief D�cide si une ligne est consid�r�e comme vide (commentaires inclus), id est ne contenant que des caract�res espace (ou un commentaire)
 * \param str la cha�ne de caract�re test�e
 * \return true si la cha�ne est vide (uniquement des espaces ou des commentaires), false sinon
 */
bool is_blank(const string &str);

/**
 * \brief rend le premier caract�re non-nul de la chaine, ou 0 si la chaine est vide
 * \param la str � tester
 */
char first_char(const string &str);




class Labyrinthe : public Environnement {
private:
    char  **_data;	//!< indique si la case est libre ou occup�e.
    int   lab_width;	//!< dimensions du rectangle.
    int   lab_height;	//!<  englobant le labyrinthe.

    /**
     * \brief initialise _data[][], en fonction de lab_with et lab_height
     **/
    void init_data();

    /**
     * \brief Initialise _guards et _nguard dans la classe
     * \param guards     (in) : une liste des coordonn�es des gardes 
     * \param player_pos (in) : La coordonn�e du joueur
     * TODO : g�n�ration de gardes diff�rents
     **/
    void build_guards(list<coord> guards, const coord &player_pos);

    /**
     * \brief Initialise les murs (_nwall et _walls[]) 
     * \param wall_list (in) : la liste des murs, un mur �tant repr�sent� par
     * une paire de coordonn�es. 
     * Pour chaque paire de coordonn�es (x1 y1) (x2 y2) de la liste, on fait 
     * l'hypoth�se que 
     * La premi�re coordonn�es est "plus petite" que la seconde, id est 
     * (x1 < x2 || y1 < y2).
     **/
    void build_walls (list<pair<coord, coord>> wall_list);

    /**
     * \brief initialise les boites 
     * \param box_list (in) : liste des coordonn�es des boites
     **/
    void build_boxes (list<coord> box_list);

    /**
     * \brief initialisation du tr�sor
     * \param c la coordonn�e du tr�sor
     **/
    void build_treasure(coord c);

    /**
     * \brief initialisation des textures
     * \param text_map (in) : la structure associant un caract�re repr�sentant une texture 
     * dans le .txt du labyrinthe au nom de la texture
     * \param text_list (in): Une liste des textures, o� chaque �l�ment contient la coordonn�e 
     * de la texture, le caract�re repr�sentant la texture, ainsi que l'orientation de la texture
     * (�gale � HORIZONTAL ou VERTICAL)
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
    // retourne l'�tat (occupation) de la case (i, j).
    char data (int i, int j){
	return _data [i][j];
    }
};

#endif
