#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "generer.h"
#include "menu.h"
#include "affichageCentrer.h"
#include "main.h"

#define GRID_SIZE 9

/**
 * Charge une grille Sudoku depuis un fichier CSV dans un tableau de char
 * Remplace les '0' par des espaces ' ' pour l'affichage
 * @param csv_file Fichier CSV déjà ouvert en lecture
 * @param grid Tableau 9x9 de char pour stocker la grille
 * @return 1 si succès, 0 si erreur de format
 */
int charger_grille_depuis_csv(FILE *csv_file, char grid[GRID_SIZE][GRID_SIZE]) {
    char line[256];
    int row = 0;

    while (fgets(line, sizeof(line), csv_file) && row < GRID_SIZE) {
        // Nettoyer la ligne
        line[strcspn(line, "\r\n")] = '\0';
        
        char *token = strtok(line, ",");
        int col = 0;

        while (token && col < GRID_SIZE) {
            // Validation basique
            if (strlen(token) != 1 || !isdigit(token[0])) {
                fprintf(stderr, "Erreur: Caractère invalide en [%d,%d]\n", row, col);
                return 0;
            }

            // Stockage avec remplacement des '0' par ' '
            grid[row][col] = (token[0] == '0') ? ' ' : token[0];
            
            // Vérification valeur 1-9 ou espace
            if (grid[row][col] != ' ' && (grid[row][col] < '1' || grid[row][col] > '9')) {
                fprintf(stderr, "Erreur: Valeur '%c' invalide en [%d,%d]\n", 
                       grid[row][col], row, col);
                return 0;
            }

            token = strtok(NULL, ",");
            col++;
        }

        if (col != GRID_SIZE) {
            fprintf(stderr, "Erreur: Ligne %d incomplète (%d/%d valeurs)\n", 
                   row+1, col, GRID_SIZE);
            return 0;
        }
        row++;
    }

    if (row != GRID_SIZE) {
        fprintf(stderr, "Erreur: Fichier incomplet (%d/%d lignes)\n", 
               row, GRID_SIZE);
        return 0;
    }

    return 1;
}

void executer_recognize_sudoku() {
    effacerEcran();
    afficherLigneSeparation();
    afficherCentre("*** Solveur Sudoku par Image ***");
    afficherLigneSeparation();

    // Demander le chemin de l'image
    char image_path[256];
    afficherCentre("Entrez le chemin complet de l'image (ou glissez-déposez le fichier):");
    
    // Lire le chemin avec fgets pour gérer les espaces
    fflush(stdin);  // Vider le buffer d'entrée
    if (fgets(image_path, sizeof(image_path), stdin) == NULL) {
        afficherCentre("Erreur de lecture du chemin.");
        system("pause");
        main();
        return;
    }
    
    // Supprimer le saut de ligne final
    image_path[strcspn(image_path, "\n")] = 0;

    // Vérifier si le fichier existe
    FILE *file_check = fopen(image_path, "r");
    if (file_check == NULL) {
        afficherCentre("Erreur: Fichier introuvable ou inaccessible.");
        system("pause");
        main();
        return;
    }
    fclose(file_check);

    // Construire la commande Python
    char command[512];
    snprintf(command, sizeof(command), "python recognize_sudoku.py \"%s\"", image_path);

    // Exécuter le script Python avec l'image
    afficherCentre("Analyse de l'image en cours...");
    int python_result = system(command);

    // Gestion des erreurs
    if (python_result != 0) {
        afficherCentre("Erreur lors de l'execution du script Python.");
        afficherCentre("Verifiez que:");
        afficherCentre("- Python est installe (python --version)");
        afficherCentre("- Toutes les bibliotheques sont installees (opencv, numpy, etc.)");
        afficherCentre("- Le chemin de l'image est valide");
        system("pause");
        return;
    }

    // Lire et traiter le résultat CSV
    FILE *csv_file = fopen("sudoku_extrait.csv", "r");
    if (!csv_file) {
        afficherCentre("Erreur: Le script n'a pas genere de fichier CSV.");
        system("pause");
        return;
    }

    // Charger la grille depuis le CSV
    char grid[GRID_SIZE][GRID_SIZE];
    if (charger_grille_depuis_csv(csv_file, grid) == 1) {
        fclose(csv_file);
        afficherCentre("Grille chargee avec succes!");
        system("pause");
        jouer_sudoku(grid);
        system("pause");
        main();  // Lancer le jeu avec la grille détectée
    } else {
        fclose(csv_file);
        afficherCentre("Erreur: Format du CSV invalide.");
        system("pause");
        main();
    }
}

