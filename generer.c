#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

#include "generer.h"
#include "resolution.h"
#include "afficher.h"
#include "sauvegarde.h"
#include "main.h" // Pour la variable globale username
#include "menu.h"

#define GRID_SIZE 9
#define MAX_ATTEMPTS 1000

extern int theme_blanc; // Pour accéder à la variable de thème déclarée dans afficher.c

// Fonction pour afficher correctement le texte du menu avec les bonnes couleurs
void afficher_texte_menu(const char *texte)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (theme_blanc)
    {
        // Mode clair: fond blanc (F), texte noir (0)
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | 0);
    }
    else
    {
        // Mode sombre: fond noir (0), texte blanc (7)
        SetConsoleTextAttribute(hConsole, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    printf("%s", texte);

    // Restaurer les couleurs par défaut du mode actuel
    if (theme_blanc)
    {
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | 0);
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
}

// Affiche une notification avec les couleurs du thème actuel
void afficher_notification(const char *message)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (theme_blanc)
    {
        // Mode clair: message en noir sur fond blanc
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | 0);
    }
    else
    {
        // Mode sombre: message en blanc sur fond noir
        SetConsoleTextAttribute(hConsole, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    printf("\n%s\n", message);
    Sleep(1500);
}

void initialiser_grille(char grille[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grille[i][j] = ' ';
        }
    }
}

void generer_grille_valide(char grille[GRID_SIZE][GRID_SIZE])
{
    initialiser_grille(grille);
    srand((unsigned int)time(NULL));

    for (int bloc = 0; bloc < 3; bloc++)
    {
        char nombres[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
        for (int i = 8; i > 0; i--)
        {
            int j = rand() % (i + 1);
            char temp = nombres[i];
            nombres[i] = nombres[j];
            nombres[j] = temp;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                grille[bloc * 3 + i][bloc * 3 + j] = nombres[i * 3 + j];
            }
        }
    }

    if (!resoudre_sudoku_entree(grille))
    {
        fprintf(stderr, "Erreur: Résolution échouée\n");
        exit(EXIT_FAILURE);
    }
}

void copier_grille(char source[GRID_SIZE][GRID_SIZE], char destination[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            destination[i][j] = source[i][j];
        }
    }
}

bool est_complete(char grille[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grille[i][j] == ' ')
                return false;
        }
    }
    return true;
}

int creer_niveau(int nombres_visibles)
{
    char grille[GRID_SIZE][GRID_SIZE];
    char grille_joueur[GRID_SIZE][GRID_SIZE];
    bool is_user_input[GRID_SIZE][GRID_SIZE] = {false};

    generer_grille_valide(grille);
    copier_grille(grille, grille_joueur);

    int cases_a_supprimer = GRID_SIZE * GRID_SIZE - nombres_visibles;
    int attempts = 0;

    while (cases_a_supprimer > 0 && attempts++ < MAX_ATTEMPTS)
    {
        int ligne = rand() % GRID_SIZE;
        int colonne = rand() % GRID_SIZE;

        if (grille_joueur[ligne][colonne] != ' ')
        {
            char temp = grille_joueur[ligne][colonne];
            grille_joueur[ligne][colonne] = ' ';

            char grille_test[GRID_SIZE][GRID_SIZE];
            copier_grille(grille_joueur, grille_test);

            if (resoudre_sudoku_entree(grille_test))
            {
                cases_a_supprimer--;
            }
            else
            {
                grille_joueur[ligne][colonne] = temp;
            }
        }
    }

    if (attempts >= MAX_ATTEMPTS)
    {
        fprintf(stderr, "Erreur: Impossible de créer le niveau\n");
        return -1;
    }

    jouer_sudoku(grille_joueur);
    return 0;
}

int niveau_facile() { return creer_niveau(45 + rand() % 6); }
int niveau_moyen() { return creer_niveau(30); }
int niveau_difficile() { return creer_niveau(17); }

void jouer_sudoku(char grille[GRID_SIZE][GRID_SIZE])
{
    int ligne = 0, colonne = 0; // Position initiale du curseur
    int action;
    bool is_user_input[GRID_SIZE][GRID_SIZE] = {false};
    bool game_over = false;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Trouver la première case vide pour positionner le curseur initialement
    bool found_empty = false;
    for (int i = 0; i < GRID_SIZE && !found_empty; i++)
    {
        for (int j = 0; j < GRID_SIZE && !found_empty; j++)
        {
            if (grille[i][j] == ' ')
            {
                ligne = i;
                colonne = j;
                found_empty = true;
            }
        }
    }

    // Marquer les cases qui sont fixes (ne peuvent pas être modifiées)
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grille[i][j] != ' ')
            {
                is_user_input[i][j] = false; // Les cases déjà remplies sont les cases fixes
            }
        }
    }

    while (!game_over)
    {
        // Appliquer le thème et afficher la grille
        appliquer_theme_terminal();
        afficher_grille(grille, is_user_input, ligne, colonne);

        // Afficher le menu avec les couleurs correctes pour le thème actuel
        if (theme_blanc)
        {
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY | 0);
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }

        printf("\nMenu:\n");
        printf("Flèches directionnelles - Déplacer le curseur\n");
        printf("Entrer un chiffre (1-9) - Remplir la case\n");
        printf("S - Résoudre le Sudoku\n");
        printf("R - Supprimer un nombre (si entré par l'utilisateur)\n");
        printf("G - Sauvegarder la partie\n");
        printf("T - Changer de thème (clair/sombre)\n");
        printf("Q - Quitter\n");
        printf("Votre choix : ");

        action = _getch(); // Obtenir l'entrée utilisateur

        switch (action)
        {
        case -32:
        case 224:
            action = _getch(); // Lire la touche directionnelle
            switch (action)
            {
            case 72: // Flèche haut
                if (ligne > 0)
                    ligne--;
                break;
            case 80: // Flèche bas
                if (ligne < GRID_SIZE - 1)
                    ligne++;
                break;
            case 75: // Flèche gauche
                if (colonne > 0)
                    colonne--;
                break;
            case 77: // Flèche droite
                if (colonne < GRID_SIZE - 1)
                    colonne++;
                break;
            }
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': // Entrer un nombre
            if (grille[ligne][colonne] != ' ' && !is_user_input[ligne][colonne])
            {
                // Case fixe, ne peut pas être modifiée
                afficher_notification("Cette case ne peut pas être modifiée!");
            }
            else
            {
                if (est_valide(grille, ligne, colonne, action))
                {
                    grille[ligne][colonne] = action;
                    is_user_input[ligne][colonne] = true;

                    // Vérifier si la grille est complète après ce coup
                    if (est_complete(grille))
                    {
                        appliquer_theme_terminal();
                        afficher_grille(grille, is_user_input, -1, -1);
                        afficher_notification("Félicitations! Vous avez résolu le Sudoku!");
                        system("pause");
                        game_over = true;
                    }
                }
                else
                {
                    afficher_notification("Valeur invalide pour cette case!");
                }
            }
            break;

        case 'R':
        case 'r': // Supprimer un nombre
            if (is_user_input[ligne][colonne] && grille[ligne][colonne] != ' ')
            {
                grille[ligne][colonne] = ' ';
                is_user_input[ligne][colonne] = false;
            }
            else if (!is_user_input[ligne][colonne] && grille[ligne][colonne] != ' ')
            {
                afficher_notification("Impossible de supprimer un nombre fixe!");
            }
            else
            {
                afficher_notification("Cette case est déjà vide!");
            }
            break;

        case 'S':
        case 's': // Résoudre le Sudoku
            if (resoudre_sudoku_entree(grille))
            {
                appliquer_theme_terminal();
                afficher_grille(grille, is_user_input, -1, -1);
                afficher_notification("Grille résolue avec succès!");
                system("pause");
                game_over = true;
            }
            else
            {
                afficher_notification("Impossible de résoudre la grille. Vérifiez vos entrées.");
            }
            break;

        case 'G':
        case 'g': // Sauvegarder la partie
            sauvegarderSudoku(grille, username);
            afficher_notification("Grille sauvegardée avec succès!");
            break;

        case 'T':
        case 't': // Changer de thème
            theme_blanc = !theme_blanc;
            appliquer_theme_terminal();
            break;

        case 'Q':
        case 'q': // Quitter le jeu
            game_over = true;
            break;

        default:
            afficher_notification("Option invalide!");
            break;
        }
    }
}