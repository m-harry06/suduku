#include <stdio.h>
#include <stdbool.h>

#define N 9  // Taille de la grille de Sudoku (9x9)

// Fonction pour vérifier si un chiffre peut être placé à une position donnée
bool est_sauve(int grille[N][N], int ligne, int colonne, int num) {
    // Vérifier la ligne
    for (int i = 0; i < N; i++) {
        if (grille[ligne][i] == num) {
            return false;  // Le chiffre existe déjà dans la ligne
        }
    }

    // Vérifier la colonne
    for (int i = 0; i < N; i++) {
        if (grille[i][colonne] == num) {
            return false;  // Le chiffre existe déjà dans la colonne
        }
    }

    // Vérifier le carré 3x3
    int debut_ligne = ligne - ligne % 3;
    int debut_colonne = colonne - colonne % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[i + debut_ligne][j + debut_colonne] == num) {
                return false;  // Le chiffre existe déjà dans le carré 3x3
            }
        }
    }

    return true;  // Le chiffre peut être placé en toute sécurité
}

// Fonction principale pour résoudre le Sudoku
bool resoudre_sudoku(int grille[N][N]) {
    int ligne, colonne;

    // Trouver une case vide (représentée par 0)
    bool trouve_vide = false;
    for (ligne = 0; ligne < N; ligne++) {
        for (colonne = 0; colonne < N; colonne++) {
            if (grille[ligne][colonne] == 0) {
                trouve_vide = true;
                break;
            }
        }
        if (trouve_vide) {
            break;
        }
    }

    // Si toutes les cases sont remplies, Sudoku résolu
    if (!trouve_vide) {
        return true;
    }

    // Essayer les chiffres de 1 à 9 pour cette case vide
    for (int num = 1; num <= N; num++) {
        // Vérifier si le chiffre peut être placé
        if (est_sauve(grille, ligne, colonne, num)) {
            // Placer le chiffre
            grille[ligne][colonne] = num;

            // Appeler récursivement la fonction pour résoudre le reste du Sudoku
            if (resoudre_sudoku(grille)) {
                return true;  // Si ça marche, le Sudoku est résolu
            }

            // Si ça ne marche pas, revenir en arrière (réinitialiser la case)
            grille[ligne][colonne] = 0;
        }
    }

    // Si aucun chiffre valide ne peut être placé, retourner faux
    return false;
}

// Fonction pour afficher la grille de Sudoku
void afficher_grille(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) {
                printf(". ");  // Afficher un point pour une case vide
            } else {
                printf("%d ", grille[i][j]);
            }
        }
        printf("\n");
    }
}

// Programme principal
int main() {
    // Exemple de grille de Sudoku partiellement remplie
    int grille[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    // Afficher la grille avant de la résoudre
    printf("Grille avant résolution :\n");
    afficher_grille(grille);

    // Résoudre le Sudoku
    if (resoudre_sudoku(grille)) {
        printf("\nGrille résolue :\n");
        afficher_grille(grille);
    } else {
        printf("\nAucune solution trouvée.\n");
    }

    return 0;
}
