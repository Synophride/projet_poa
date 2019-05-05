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
using namespace std; // � virer


const bool HORIZONTAL = false; //!< bool�en permettant d'indiquer le sens des affiches
const bool VERTICAL   =  true; //!< bool�en permettant d'indiquer le sens des affiches


const char WALL = 1;
const char BOX = 2;
const char GARDE  = 3;
const char JOUEUR = 4;
const char TREASURE = 5;

/// Structure repr�sentant une coordonn�e du labyrinthe 
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
    int   lab_width;	//!< Dimension de l'axe 'x' du labyrinthe. 
    int   lab_height;	//!< Dimension de l'axe 'y' du labyrinthe.
    int   _nb_alive;    //!< Indique le nombre 

    
    vector<vector<int>> _dist_vect; //!< Vecteur contenant les distances entre chaque case et le tr�sor
    vector<vector<int>> _estimated_dist_player; //!< Vecteur contenant les distances estim�es par les gardiens au joueur
    
    /**
     * \brief initialise _data[][], en fonction de lab_with et lab_height
     **/
    void init_data();

    /**
     * \brief remplit la matrice de distance au tr�sor, r�cursivement, 
     * en partant de la case de coordonn�es (x, y) qu'on sait d�j� remplie
     * 
     */
    void fill_dist(int x, int y, vector<vector<int>> &t);
    
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
     **/
    void build_text(map<char, string> text_map, list<tuple<coord, char, bool>> text_list);

    /**
     * \brief initialise le vecteur des distances. Doit �tre appell� � la fin du constructeur
     **/
    void init_vector_dist();

    /**
     * \brief initialise le vecteur des distances estim�es vers le joueur
     **/
    void init_vector_playerdist();
    
    bool spotted = false;
    int* _box_pv;
    void echange_boites(int x);
 public:
    Labyrinthe();
    Labyrinthe (char*);
    
    void spot(int x, int y){
	spotted = true;
	maj_player_dist(x, y);
    }

    void hurt_box_at(int x, int y);
    bool is_spotted(){return spotted;}
    
    
    /// \brief  retourne la largeur du labyrinthe.
    int width () { return lab_width;}

    /// \brief retourne la longueur du labyrinthe.
    int height () {return lab_height;}

    /**
     * \brief Informe sur l'�tat d'occupation de la case (i, j).
     * \returns WALL si la case est un mur,
     * BOX si la case est occup�e par une bo�te, 
     * GARDE si la case est occup�e par un garde, 
     * JOUEUR si la case est occup�e par le joueur, 
     * TREASURE si la case est occup�e par le tr�sor
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
     * �\brief indique que une instance de Fireball du joueur a explos� � l'endroit o� �tait plac� un (ou plusieurs) des gardiens. 
     * Va donc "Blesser" le gardien via la m�thode Gardien::hurt()
     * \param x la composante dans l'axe des abcisses de la case dans laquelle la boule de feu explose
     * \parap y la composante dans l'axe des ordonn�es de la case dans laquelle la boule de feu explose
**/
    void hurt_gardien_at(int x, int y);

    /**
     * \brief indique au labyrinthe que le joueur a �t� touch� par une boule de 
     * feu provenant d'un des gardiens 
     *
     * Va donc appeller la m�thode Chasseur::hurt()
     **/
    void hurt_joueur();

    /**
     * \brief change la valeur d'une case de la matrice _data, par exemple lors du d�placement d'un gardien
     * \param x 
     * \param y
     * \param value La nouvelle valeur de la case _data[x][y]. 
     * En th�orie, compris entre EMPTY et TREASURE, mais dans les faits ce sera EMPTY, JOUEUR, ou GARDE, �tant donn� que seules ces entit�s se d�placent)
     **/
    void set_data(int x, int y, char value);

    /**
     * \brief rend la distance de la case courante 
     * \param x la coordonn�e dans l'axe des abcisses
     * \param y la coordonn�e dans l'axe des ordonn�es
     * \returns -1 si la case est un mur, 
     * 1 000 000 s'il n'existe pas de chemin entre le point de coordonn�es (x,y),
     * La distance en nombre de cases entre le point (x,y) et le tr�sor dans les autres cas
     **/
    int dist_of_treasure(int x, int y){
	return _dist_vect[x][y];
    }

    /**
     * \brief rend la distance estim�e par les gardiens
     * entre la case de coord (x, y)
     * et le chasseur.
     * \param x
     * \param y
     * \returns La distance estim�e par les gardiens entre le point donn� en param�tre et 
     * la valeur r�elle.
     **/
    int dist_of_player(int x, int y){
	return _estimated_dist_player[x][y];
    }

    /**
    * \brief met � jour les estimations de distances au joueur, en partant du principe que le 
    * joueur est aux coordonn�ees (x, y)
    **/
    void maj_player_dist(int x, int y);
    
};

#endif
