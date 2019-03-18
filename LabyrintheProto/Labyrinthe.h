#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"
#include <utility>
#include <list>
#include <iostream>
#include <string>
#include <vector>

struct coord{
    int x;
    int y;
};

/**
 * \brief Charge un fichier texte vers un vector de string, 
 *  o� chaque ligne correspond � une entr�e du tableu 
 * \param path le chemin vers le fichier texte
 * \return Un tableau de chaque ligne contenue dans le fichier en entr�e
 * (todo)
 **/
std::vector<std::string> make_str_vector(char* path);

/**
 * \brief � partir d'une coordonn�e, rend la liste des coordonn�es des extr�mit�s des murs
 * qui vont vers le bas ou vers la gauche
 * \param base_coord la coordonn�e duquel on part
 * \param laby le labyrinthe, sous forme de vecteur de cha�nes de caract�res
 * \return la liste (qui peut �tre nulle) des extr�mit�s des murs partant de coord, et qui vont 
 * vers le bas ou la gauche
 */
std::list<coord> mk_wall_list(const coord &base_coord, const std::vector<std::string> &laby);

/**
 * \brief D�cide si une ligne est consid�r�e comme vide (commentaires inclus), id est ne contenant que des caract�res espace (ou un commentaire)
 * \param str la cha�ne de caract�re test�e
 * \return true si la cha�ne est vide (uniquement des espaces ou des commentaires), false sinon
 */
bool is_blank(const std::string &str);


class Labyrinthe : public Environnement {
private:
	char	**_data;	// indique si la case est libre ou occup�e.
	int   lab_width;	// dimensions du rectangle.
	int   lab_height;	// englobant le labyrinthe.

public:
	Labyrinthe();
	Labyrinthe (char*);
	int width () { return lab_width;}	// retourne la largeur du labyrinthe.
	int height () { return lab_height;}	// retourne la longueur du labyrinthe.
	// retourne l'�tat (occupation) de la case (i, j).
	char data (int i, int j)
	{
		return _data [i][j];
	}
};

#endif
