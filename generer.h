#ifndef GENERER_H_INCLUDED
#define GENERER_H_INCLUDED

#include <stdbool.h>

#define N 9

void initialiser_grille(int grille[N][N]);
int est_valide(int grille[N][N], int ligne, int colonne, int num);
void generer_grille(int grille[N][N]);
void afficher_grille(int grille[N][N]);
int est_complete(int grille[N][N]);
void clear_input_buffer();
bool est_sauve(int grille[N][N], int ligne, int colonne, int num);
void jouer_sudoku(int grille[N][N]);
int niveau_facile();

#endif // GENERER_H