#ifndef GENERER_H_INCLUDED
#define GENERER_H_INCLUDED

#include <stdbool.h>

#define GRID_SIZE 9

// Fonctions de gÃ©nÃ©ration et manipulation de grille
void initialiser_grille(char grille[GRID_SIZE][GRID_SIZE]);
bool est_valide(char grille[GRID_SIZE][GRID_SIZE], int ligne, int colonne, char num); // Fixed parameter type
void generer_grille_valide(char grille[GRID_SIZE][GRID_SIZE]);
//void afficher_grille(char grille[GRID_SIZE][GRID_SIZE]);
bool est_complete(char grille[GRID_SIZE][GRID_SIZE]);

// Fonction de jeu
void jouer_sudoku(char grille[GRID_SIZE][GRID_SIZE]);

// Niveaux de difficultÃ©
int niveau_facile();
int niveau_moyen();
int niveau_difficile();

#endif
