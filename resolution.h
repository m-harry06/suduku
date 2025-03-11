#indef RESOLUTION_H_INCLUDED
#define RESOLUTION_H_INCLUDED

#define N 9

bool est_sauve(int grille[N][N], int ligne, int colonne, int num);
bool resoudre_sudoku(int grille[N][N]);
void afficher_grille(int grille[N][N]);

#endef 
