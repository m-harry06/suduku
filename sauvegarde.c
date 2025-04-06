#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "affichageCentrer.h"
#include "menu.h"


void sauvegarderSudoku(char grille[9][9], const char *nomUtilisateur) {
    // Ouvrir le fichier en mode ajout (a+) pour pouvoir lire et écrire
    FILE *fichier = fopen("sauvegarde.csv", "a+");
    if (fichier == NULL) {
        afficherCentre("Erreur: Impossible d'ouvrir le fichier de sauvegarde");
        return;
    }

    // Générer un ID unique (par exemple, timestamp)
    int id = getLastId("sauvegarde.csv");
    
    // Écrire l'ID et le nom d'utilisateur
    fprintf(fichier, "%d,%s", id, nomUtilisateur);
    
    // Écrire toutes les cases de la grille séparées par des virgules
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Remplacer les espaces vides par un caractère spécifique (0 ou .)
            char caseActuelle = (grille[i][j] == ' ') ? '0' : grille[i][j];
            fprintf(fichier, ",%c", caseActuelle);
        }
    }
    
    // Terminer la ligne
    fprintf(fichier, "\n");
    fclose(fichier);
    
    // Message de confirmation
    char message[150];
    snprintf(message, sizeof(message), 
            "Sauvegarde réussie pour %s (ID: %d)\nFichier: sauvegarde.csv", 
            nomUtilisateur, id);
    afficherCentre(message);
}

void chargerSudoku(char grille[9][9], const char *nomUtilisateur) {
    // Vérification des paramètres
    if (nomUtilisateur == NULL || strlen(nomUtilisateur) == 0) {
        afficherCentre("Erreur: Nom d'utilisateur invalide");
        return;
    }

    FILE *fichier = fopen("sauvegarde.csv", "r");
    if (fichier == NULL) {
        afficherCentre("Erreur: Fichier de sauvegarde introuvable");
        return;
    }

    char ligne[1024]; // Buffer pour une ligne complète
    bool trouve = false;
    char message[100];

    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Supprimer le saut de ligne
        ligne[strcspn(ligne, "\n")] = '\0';
        
        // Trouver la position du nom d'utilisateur
        char *ptr = strstr(ligne, nomUtilisateur);
        if (ptr != NULL) {
            // Vérifier que c'est bien le nom d'utilisateur et pas une partie du CSV
            if (ptr > ligne && *(ptr - 1) == ',' && (ptr[strlen(nomUtilisateur)] == ',' || ptr[strlen(nomUtilisateur)] == '\0')) {
                trouve = true;
                break;
            }
        }
    }

    if (!trouve) {
        snprintf(message, sizeof(message), "Aucune sauvegarde trouvée pour %s", nomUtilisateur);
        afficherCentre(message);
        fclose(fichier);
        return;
    }

    // Traitement de la ligne trouvée
    char *token = strtok(ligne, ",");
    int index = 0;
    
    // Sauter l'ID et le nom d'utilisateur (2 premiers champs)
    if (token) token = strtok(NULL, ","); // ID
    if (token) token = strtok(NULL, ","); // Nom utilisateur
    
    // Remplir la grille
    for (int i = 0; i < 9 && token != NULL; i++) {
        for (int j = 0; j < 9 && token != NULL; j++) {
            // Remplacer '0' par ' ' et garder les autres valeurs
            grille[i][j] = (token[0] == '0') ? ' ' : token[0];
            token = strtok(NULL, ",");
            index++;
        }
    }

    // Vérification complète
    if (index != 81) {
        snprintf(message, sizeof(message), 
                "Erreur: Format de sauvegarde invalide pour %s", nomUtilisateur);
        afficherCentre(message);
        initialiser_grille(grille); // Réinitialiser la grille en cas d'erreur
    } else {
        snprintf(message, sizeof(message), 
                "Grille chargée avec succès pour %s", nomUtilisateur);
        afficherCentre(message);
    }

    fclose(fichier);
}