#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include "resolution.h"
#include "generer.h" 

#define N 9

// Prototypes des fonctions
void initialiser_grille(int grille[N][N]);
int est_valide(int grille[N][N], int ligne, int colonne, int num);
void generer_grille(int grille[N][N]);
void afficher_grille(int grille[N][N]);
int est_complete(int grille[N][N]);
void clear_input_buffer();
bool est_sauve(int grille[N][N], int ligne, int colonne, int num);

// Fonction pour vérifier si un nombre peut être placé en sécurité



void initialiser_grille(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grille[i][j] = 0;
        }
    }
}

int est_valide(int grille[N][N], int ligne, int colonne, int num) {
    return est_sauve(grille, ligne, colonne, num);
}

void generer_grille(int grille[N][N]) {
    srand(time(0));

    for (int ligne = 0; ligne < N; ligne++) {
        for (int colonne = 0; colonne < N; colonne++) {
            if (rand() % 3 == 0) {
                grille[ligne][colonne] = 0;
            } else {
                int essais = 0;
                int num;
                do {
                    num = rand() % 9 + 1;
                    essais++;
                    if (essais > 20) break;
                } while (!est_valide(grille, ligne, colonne, num));

                grille[ligne][colonne] = (essais <= 20) ? num : 0;
            }
        }
    }
}

void afficher_grille(int grille[N][N]) {
    printf("\n\n      1   2   3    4   5   6    7   8   9\n");
    printf("    +---+---+---+ +---+---+---+ +---+---+---+\n");
    
    for (int i = 0; i < N; i++) {
        printf("%d   |", i+1);
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) {
                printf("   |");
            } else {
                printf(" %d |", grille[i][j]);
            }
            if (j == 2 || j == 5) printf(" |");
        }
        printf("\n");
        if (i == 2 || i == 5) {
            printf("    ++===+===+===++===+===+===++===+===+===++\n");
        } else {
            printf("    +---+---+---+ +---+---+---+ +---+---+---+\n");
        }
    }
    printf("\n");
}

int est_complete(int grille[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille[i][j] == 0) return 0;
        }
    }
    return 1;
}

void clear_input_buffer() {
    while (getchar() != '\n');
}

void jouer_sudoku(int grille[N][N]) {
    int ligne, colonne, num;
    char action;
    
    while (!est_complete(grille)) {
        afficher_grille(grille);
        
        printf("Menu:\n");
        printf("R - Remplir une case\n");
        printf("S - Resoudre le Sudoku\n");
        printf("Q - Quitter\n");
        printf("Votre choix : ");
        scanf(" %c", &action);
        action = toupper(action);
        
        switch(action) {
            case 'R':
                printf("\nEntrez la ligne (1-9), colonne (1-9) et nombre (1-9) : ");
                if (scanf("%d %d %d", &ligne, &colonne, &num) != 3) {
                    printf("Saisie invalide.\n");
                    clear_input_buffer();
                    continue;
                }
                
                if (ligne < 1 || ligne > 9 || colonne < 1 || colonne > 9 || num < 1 || num > 9) {
                    printf("Valeurs invalides. Doit etre entre 1 et 9.\n");
                    continue;
                }
                
                if (grille[ligne-1][colonne-1] != 0) {
                    printf("Case deja remplie. Voulez-vous la modifier? (O/N) : ");
                    char choix;
                    scanf(" %c", &choix);
                    if (toupper(choix) != 'O') continue;
                }
                
                if (est_valide(grille, ligne-1, colonne-1, num)) {
                    grille[ligne-1][colonne-1] = num;
                    printf("Nombre place avec succes!\n");
                } else {
                    printf("Ce nombre n'est pas valide ici!\n");
                }
                break;
                
            case 'S':
                printf("\nRésolution du Sudoku en cours...\n");
                if (resoudre_sudoku_entree(grille)) {
                    printf("Sudoku résolu avec succes!\n");
                    afficher_grille(grille);
                    return;
                } else {
                    printf("Aucune solution trouvee pour cette grille.\n");
                }
                break;
                
            case 'Q':
                printf("\nPartie terminee. Voici votre grille finale :\n");
                return;
                
            default:
                printf("Option invalide.\n");
        }
    }
    
    printf("\nFélicitations! Vous avez complete le Sudoku!\n");
}

int niveau_facile() {
    int grille[N][N];
    
    initialiser_grille(grille);
    generer_grille(grille);
    
    printf("Bienvenue dans le jeu de Sudoku!\n");
    printf("Remplissez les cases vides (affichées comme ' ') avec des nombres de 1 à 9.\n");
    
    jouer_sudoku(grille);
    
    afficher_grille(grille);
    
    return 0;
}