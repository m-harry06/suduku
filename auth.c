#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"

#define MAX_LINE 1024
#define DELIMITER ","
#define FILENAME "utilisateurs.csv"

int authentification(const char *nom_utilisateur,const char *password){

    FILE *file = fopen(FILENAME, "r");
    if(file == NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        return 0;
    }

    char line[MAX_LINE];
    while(fgets(line, sizeof(line), file )){
        char *token;
        char *user = NULL, *pass = NULL;
        int i = 0;
        token = strtok(line, DELIMITER);
        while (token != NULL){
            if(i == 3){
                user = token;
            }else if (i == 4){
                pass = token;
                break;
            }
            i++;
            token = strtok(NULL, DELIMITER);
        }
        if(user && pass){
            user[strcspn(user, "\n")] = 0;
            pass[strcspn(pass, "\n")] = 0;

            if(strcmp(user, nom_utilisateur)== 0 && strcmp(pass, password)==0){
                fclose(file);
                return 1;
            }
        }
        
    }

    fclose(file);
    return 0;
}