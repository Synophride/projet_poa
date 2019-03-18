#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include <utility>
#include <list>
#include <iostream>
#include <string>
#include <vector>
#include <ios>
#include <map>
#include <fstream>
#include "Environnement.h"

struct coord{
    int x;
    int y;
};

/**
 * \brief Charge un fichier texte vers un vector de string, 
 *  où chaque ligne correspond à une entrée du tableu 
 * \param path le chemin vers le fichier texte
 * \return Un tableau de chaque ligne contenue dans le fichier en entrée
 * (todo)
 **/
std::vector<std::string> make_str_vector(char* path);

/**
 * \brief à partir d'une coordonnée, rend la liste des coordonnées des extrémités des murs
 * qui vont vers le bas ou vers la gauche
 * \param base_coord la coordonnée duquel on part
 * \param laby le labyrinthe, sous forme de vecteur de chaînes de caractères
 * \return la liste (qui peut être nulle) des extrémités des murs partant de coord, et qui vont 
 * vers le bas ou la gauche
 */
std::list<coord> mk_wall_list(const coord &base_coord, const std::vector<std::string> &laby);

/**
 * \brief Décide si une ligne est considérée comme vide (commentaires inclus), id est ne contenant que des caractères espace (ou un commentaire)
 * \param str la chaîne de caractère testée
 * \return true si la chaîne est vide (uniquement des espaces ou des commentaires), false sinon
 */
bool is_blank(const std::string &str);


class Labyrinthe : public Environnement {
private:
	char	**_data;	// indique si la case est libre ou occupée.
	int   lab_width;	// dimensions du rectangle.
	int   lab_height;	// englobant le labyrinthe.

public:
	Labyrinthe();
	Labyrinthe (char*);
	int width () { return lab_width;}	// retourne la largeur du labyrinthe.
	int height () { return lab_height;}	// retourne la longueur du labyrinthe.
	// retourne l'état (occupation) de la case (i, j).
	char data (int i, int j)
	{
		return _data [i][j];
	}
};

#endif
