#ifndef GENERER_H
#define GENERER_H

#define GRID_SIZE 9

void initialiser_grille(char grille[GRID_SIZE][GRID_SIZE]);
void generer_grille_valide(char grille[GRID_SIZE][GRID_SIZE]);
void copier_grille(char source[GRID_SIZE][GRID_SIZE], char destination[GRID_SIZE][GRID_SIZE]);
void jouer_sudoku(char grille[GRID_SIZE][GRID_SIZE]);
int niveau_facile();
int niveau_moyen();
int niveau_difficile();

#endif