Pour répondre à votre demande, voici un programme en langage C qui génère une grille de Sudoku avec un niveau de difficulté ajustable de 1 à 100. Les colonnes vides sont représentées par des points d'interrogation ('?').

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 9

// Fonction pour vérifier si un nombre peut être placé dans une case spécifique
bool estValide(int grille[N][N], int ligne, int colonne, int nombre) {
    for (int x = 0; x < N; x++) {
        if (grille[ligne][x] == nombre || grille[x][colonne] == nombre)
            return false;
    }
    int debutLigne = ligne - ligne % 3, debutColonne = colonne - colonne % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[i + debutLigne][j + debutColonne] == nombre)
                return false;
        }
    }
    return true;
}

// Fonction pour résoudre la grille de Sudoku en utilisant le backtracking
bool resoudreGrille(int grille[N][N]) {
    int ligne = -1, colonne = -1;
    bool estVide = true;
    for (int i = 0; i < N && estVide; i++) {
        for (int j = 0; j < N && estVide; j++) {
            if (grille[i][j] == 0) {
                ligne = i;
                colonne = j;
                estVide = false;
            }
        }
    }
    if (estVide)
        return true;
    for (int nombre = 1; nombre <= 9; nombre++) {
        if (estValide(grille, ligne, colonne, nombre)) {
            grille[ligne][colonne] = nombre;
            if (resoudreGrille(grille))
                return true;
            grille[ligne][colonne] = 0;
        }
    }
    return false;
}

// Fonction pour imprimer la grille de Sudoku avec des colonnes vides représentées par '?'
void imprimerGrille(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0)
                printf(" ? ");
            else
                printf(" %d ", grille[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour remplir la diagonale de blocs 3x3
void remplirDiagonale(int grille[N][N]) {
    for (int i = 0; i < N; i += 3) {
        int num;
        bool present[10] = {false};
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                do {
                    num = rand() % 9 + 1;
                } while (present[num]);
                present[num] = true;
                grille[i + j][i + k] = num;
            }
        }
    }
}

// Fonction pour supprimer des cases de la grille en fonction du niveau de difficulté
void supprimerCases(int grille[N][N], int niveau) {
    int nbCasesASupprimer = (niveau * 81) / 100; // Calcul du nombre de cases à supprimer
    int count = nbCasesASupprimer;
    while (count != 0) {
        int cellId = rand() % 81;
        int i = cellId / 9;
        int j = cellId % 9;
        if (grille[i][j] != 0) {
            grille[i][j] = 0;
            count--;
        }
    }
}

// Fonction pour générer une grille de Sudoku complète
void genererGrilleComplete(int grille[N][N]) {
    remplirDiagonale(grille);
    resoudreGrille(grille);
}

// Fonction principale
int main() {
    srand(time(0));
    int grille[N][N] = {0};
    genererGrilleComplete(grille);
    printf("Grille complete :\n");
    imprimerGrille(grille);

    int niveau;
    printf("Choisissez le niveau de difficulte (1-100) : ");
    scanf("%d", &niveau);
    if (niveau < 1 || niveau > 100) {
        printf("Niveau invalide. Veuillez choisir un niveau entre 1 et 100.\n");
        return 1;
    }

    supprimerCases(grille, niveau);
    printf("\nGrille avec cases supprimees (niveau %d) :\n", niveau);
    imprimerGrille(grille);

    return 0;
  }
