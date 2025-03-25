#ifndef GENERER_H
#define GENERER_H

#include <stdbool.h>

#define N 9

// Fonctions de génération et manipulation de grille
void initialiser_grille(int grille[N][N]);
bool est_valide(int grille[N][N], int ligne, int colonne, int num);
void generer_grille_valide(int grille[N][N]);
void afficher_grille(int grille[N][N]);
bool est_complete(int grille[N][N]);

// Fonction de jeu
void jouer_sudoku(int grille[N][N]);

// Niveaux de difficulté
int niveau_facile();

#endif