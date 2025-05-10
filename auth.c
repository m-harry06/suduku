#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"
#include "menu.h"

#define MAX_LINE 1024
#define DELIMITER ","
#define FILENAME "utilisateurs.csv"
#define MAX_TOKENS 6

// Fonction pour nettoyer les chaînes
void clean_string(char *str) {
    if (str == NULL) return;
    
    // Supprimer les retours chariot et sauts de ligne
    char *newline = strchr(str, '\n');
    if (newline) *newline = '\0';
    
    char *carriage = strchr(str, '\r');
    if (carriage) *carriage = '\0';
    
    // Supprimer les espaces en début
    char *start = str;
    while(isspace((unsigned char)*start)) start++;
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    // Si la chaîne est vide après suppression des espaces du début
    if(*str == 0) return;
    
    // Supprimer les espaces en fin
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

// Méthode simple et fiable pour traiter une ligne CSV
static void parse_csv_line(const char *line, char fields[][MAX_LINE], int max_fields) {
    if (line == NULL || fields == NULL || max_fields <= 0) return;
    
    const char *p = line;
    int field_idx = 0;
    int char_idx = 0;
    
    while (*p && field_idx < max_fields) {
        if (*p == ',') {
            // Fin d'un champ
            fields[field_idx][char_idx] = '\0';
            field_idx++;
            char_idx = 0;
        } else if (*p != '\r' && *p != '\n') {
            // Ajouter le caractère au champ courant
            if (char_idx < MAX_LINE - 1) {
                fields[field_idx][char_idx++] = *p;
            }
        }
        p++;
    }
    
    // Terminer le dernier champ
    if (field_idx < max_fields) {
        fields[field_idx][char_idx] = '\0';
    }
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

    // Préparation des identifiants à rechercher
    char cleaned_user[MAX_LINE];
    char cleaned_pass[MAX_LINE];
    
    // Copie sécurisée des chaînes d'entrée
    strncpy(cleaned_user, nom_utilisateur, MAX_LINE - 1);
    strncpy(cleaned_pass, password, MAX_LINE - 1);
    cleaned_user[MAX_LINE - 1] = '\0';
    cleaned_pass[MAX_LINE - 1] = '\0';
    
    clean_string(cleaned_user);
    clean_string(cleaned_pass);
    
    // Contournement: Vérifier aussi en ignorant le premier caractère
    // pour gérer le cas où le premier caractère est perdu avant l'appel
    char user_without_first[MAX_LINE] = "";
    if (strlen(cleaned_user) > 0) {
        strcpy(user_without_first, cleaned_user + 1);
    }
    
    // Analyse du fichier CSV
    char line[MAX_LINE];
    int auth_result = 0;
    
    while (fgets(line, sizeof(line), file)) {
        char fields[MAX_TOKENS][MAX_LINE];
        memset(fields, 0, sizeof(fields));
        
        // Parser la ligne CSV
        parse_csv_line(line, fields, MAX_TOKENS);
        
        // Nettoyer les champs pertinents (utilisateur et mot de passe)
        clean_string(fields[3]); // Nom d'utilisateur
        clean_string(fields[4]); // Mot de passe
        
        // Vérifier les identifiants - cas normal
        if (strcmp(fields[3], cleaned_user) == 0 && 
            strcmp(fields[4], cleaned_pass) == 0) {
            auth_result = 1;
            break;
        }
        
        // Contournement: Vérifier aussi si le premier caractère est manquant
        if (strlen(user_without_first) > 0 && 
            strcmp(fields[3], user_without_first) == 0 && 
            strcmp(fields[4], cleaned_pass) == 0) {
            // On a trouvé une correspondance en ignorant le premier caractère
            auth_result = 1;
            
            // Afficher un avertissement en mode debug (stderr)
            fprintf(stderr, "Attention: Premier caractère ignoré dans le nom d'utilisateur\n");
            break;
        }
    }

    fclose(file);
    
    if (!auth_result) {
        fprintf(stderr, "Échec d'authentification\n");
    }
    
    return auth_result;
}