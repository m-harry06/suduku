#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"
#include "menu.h"

#define MAX_LINE 1024
#define DELIMITER ","
#define FILENAME "utilisateurs.csv"

// Fonction pour nettoyer les chaînes
void clean_string(char *str) {
    if (str == NULL) return;
    
    // Supprimer les retours chariot et sauts de ligne
    str[strcspn(str, "\r\n")] = 0;
    
    // Supprimer les espaces en début et fin
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

int authentification(const char *nom_utilisateur, const char *password) {
    if (nom_utilisateur == NULL || password == NULL) {
        fprintf(stderr, "Erreur: Paramètres invalides\n");
        return 0;
    }

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier utilisateurs");
        return 0;
    }

    char line[MAX_LINE];
    int auth_result = 0;
    char cleaned_user[MAX_LINE];
    char cleaned_pass[MAX_LINE];

    // Nettoyer les entrées
    strncpy(cleaned_user, nom_utilisateur, MAX_LINE-1);
    strncpy(cleaned_pass, password, MAX_LINE-1);
    cleaned_user[MAX_LINE-1] = '\0';
    cleaned_pass[MAX_LINE-1] = '\0';
    clean_string(cleaned_user);
    clean_string(cleaned_pass);

    while (fgets(line, sizeof(line), file)) {
        char *tokens[6]; // Nous avons besoin jusqu'à la colonne 5 (index 4)
        int token_count = 0;
        char *token = strtok(line, DELIMITER);
        
        // Découper la ligne en tokens
        while (token != NULL && token_count < 6) {
            tokens[token_count++] = token;
            token = strtok(NULL, DELIMITER);
        }

        // Vérifier que nous avons bien les colonnes nécessaires
        if (token_count >= 5) { // Au moins 5 colonnes (0-4)
            char file_user[MAX_LINE];
            char file_pass[MAX_LINE];
            
            // Colonne 4 (index 3) pour le pseudo
            // Colonne 5 (index 4) pour le mot de passe
            if (token_count > 3) strncpy(file_user, tokens[3], MAX_LINE-1);
            else continue;
            
            if (token_count > 4) strncpy(file_pass, tokens[4], MAX_LINE-1);
            else continue;
            
            file_user[MAX_LINE-1] = '\0';
            file_pass[MAX_LINE-1] = '\0';
            
            clean_string(file_user);
            clean_string(file_pass);

            if (strcmp(file_user, cleaned_user) == 0 && strcmp(file_pass, cleaned_pass) == 0) {
                auth_result = 1;
                break;
            }
        }
    }

    fclose(file);
    
    if (!auth_result) {
        fprintf(stderr, "Authentification échouée pour %s\n", cleaned_user);
    }
    
    return auth_result;
}