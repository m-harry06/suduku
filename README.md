Dossier de conception de l'application sudoku

        1. introduction

    1.1 Objectif du projet

L'objectif de ce projet est de develloper une application en C permettent aux utilisateurs de jouer au sudoku sur un terminal. L'application doit permettre:

    . La generation des grille de sudoku de differentes dificultés
    . La possibilité de resoudre une grille manuellement ou automatiquement
    . La validation des entrees des utilisateurs
    . L'enregistrement et le changenment de paertie en cours 
    . 

    1.2 Contexte

L'applicaton est concue pour etre utilisee par toute personne souhaitant jouer au sudoku sur son ordinateur. elle vise a offrir une experience fluide et intuitive en mode console. 

    1.3 Perimètre de l'Application

L'application comportera les fonctionalités suivantes:
    . Géneration aleatoire de grille de sudoku
    . verification de la validité des mouvements 
    . resolution automatique d'une grille
    . sauvegarde et changement de parties
    . interface en mode texte permettant une interaction fluide

    1.4 Technologie et bibliotheque utilisées 

    .Language: C
    . Bibliothèques standard: stdio.h, stdlib.h, string.h, time.h, conio.h
    . Ségestion des fichier: fopen, fprintf, fscanf
    . Sécurité: Verification des entrées utilisateur pour eviter les erreurs

        2. Architechture du projet

    2.1 module principaux

    Module          Description

1. Generation.c	    Génère une grille de sudoku avec different niveaux de difficulté

2. Resolution.c	    Implemente un solveur automatique basé sur un algorithme de bactraking

3. Verification.c	Verifie si un movement est valide dans la grille

4. Sauvegarde.c	    Permet de sauvegarder et changer une partie en cours

5. Interface.c	    Fournit une interface utilisateur en mode terminal

6. Main.c	        Point d’entré de l’application

7. Auth.c	        Permet de gerer l’authentification des utilisateurs

8. Menu.c	        Permet de gerer tous les menus relatifs au jeu

9. Cryptage.c	    Permet de hacher les mots de passe des utlisateurs dans le fichier d’enregistrement


        3. Repatition des responsabilité

    membres	            responsabilité

1.  Bamou	            Devellopement du module  generation.c

2.  Grac	            Implementation du solver dans Resolution.c

3.  Mbengmo	            Verification des mouvements et des entres de l’utilisateurverification.c 

4.  Nzonta & Tessemo	Gestion des fichier avec sauvegarde.c

5.  Obono & Ndoukeu	    Hachage des données de l’utilisateurs cryptage.c


        4. Sécurité et bonnes pratique

    4.1 sécurisation des données

    . Validation des entrees utilisateur pour eviter les erreurs 
    . Gestion des fichiers sécurisée pour sauvegarde et chargement

    4.2 Gestion des erreurs

    . Vérification des fichiers avant lecture
    . Gestion des erreurs
    . Logs des erreurs pour faciliter le debogage

    4.3 Tests et validation 

    . tests unitaires sur chaque module
    . Verification des erreurs pour faciliter le debogage

    4.3 Test et Validation

    . Tests unitaire sur chaque module
    . Verification des cas limites (grille invalide, entrees incorrecte)
    . Simulation de scénarios d'utilisation

        5. Conclision
