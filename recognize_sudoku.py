import cv2
import numpy as np
import pandas as pd
import pytesseract
import os
from tensorflow.keras.models import load_model

# Configuration de Tesseract
pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files\Tesseract-OCR\tesseract.exe"

# Charger le modèle
model = load_model('models/best_digit_model.h5')

def detecter_grille(image):
    """Détection précise de la grille Sudoku"""
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.adaptiveThreshold(blur, 255, 
                                  cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                  cv2.THRESH_BINARY_INV, 11, 2)
    
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if not contours:
        return None
    
    plus_grand = max(contours, key=cv2.contourArea)
    x, y, w, h = cv2.boundingRect(plus_grand)
    grille = gray[y:y+h, x:x+w]
    return cv2.resize(grille, (450, 450))

def extraire_cellules(grille):
    """Extraction ordonnée des cellules"""
    cellules = []
    for i in range(9):
        ligne = []
        for j in range(9):
            marge = 2  # Réduire le bruit des bordures
            cellule = grille[i*50 + marge:(i+1)*50 - marge, 
                            j*50 + marge:(j+1)*50 - marge]
            ligne.append(cellule)
        cellules.append(ligne)
    return cellules

def preparer_cellule(cellule):
    """Prétraitement identique au dataset d'entraînement"""
    cellule = cv2.resize(cellule, (28, 28))
    _, cellule = cv2.threshold(cellule, 150, 255, cv2.THRESH_BINARY_INV)
    return cellule.astype('float32') / 255.0

def reconnaitre_chiffre(cellule, model):
    """Reconnaissance avec le modèle"""
    cellule = np.expand_dims(cellule, axis=-1)
    prediction = model.predict(np.array([cellule]))
    return np.argmax(prediction)

def extraire_sudoku(image_path):
    img = cv2.imread(image_path)
    if img is None:
        print("❌ Erreur : Image introuvable")
        return

    grille = detecter_grille(img)
    if grille is None:
        print("❌ Aucune grille détectée")
        return

    cellules = extraire_cellules(grille)
    grille_finale = []

    for i, ligne in enumerate(cellules):
        ligne_chiffres = []
        for j, cellule in enumerate(ligne):
            cellule_prete = preparer_cellule(cellule)
            chiffre = reconnaitre_chiffre(cellule_prete, model)
            ligne_chiffres.append(chiffre if chiffre != 0 else 0)
        grille_finale.append(ligne_chiffres)

    # Affichage et sauvegarde
    print("\n🧠 Grille extraite :")
    for ligne in grille_finale:
        print(" ".join(str(x) if x != 0 else "." for x in ligne))
    
    csv_path = os.path.join(os.path.dirname(__file__), "sudoku_extrait.csv")
    pd.DataFrame(grille_finale).to_csv(csv_path, index=False, header=False)
    print(f"\n✅ Fichier CSV sauvegardé : {csv_path}")

if __name__ == "__main__":
    chemin = input("🖼️ Chemin de l'image Sudoku : ").strip('"')
    extraire_sudoku(chemin)
