#ifndef PICTURESOLVER_H_INCLUDED
#define PICTURESOLVER_H_INCLUDED

#include <stdio.h>  // Pour FILE

// Définir la taille de la grille si ce n’est pas déjà fait
#ifndef GRID_SIZE
#define GRID_SIZE 9
#endif

// Fonction pour exécuter la reconnaissance de la grille Sudoku à partir d'une image
void executer_recognize_sudoku(void);

// Fonction pour charger une grille depuis un fichier CSV
int charger_grille_depuis_csv(FILE *csv_file, char grid[GRID_SIZE][GRID_SIZE]);

#endif // PICTURESOLVER_H_INCLUDED
