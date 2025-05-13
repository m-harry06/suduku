import cv2
import numpy as np
import os

def preprocess_and_save_digits(image_path, output_dir='digits_dataset'):
    # Créer les sous-dossiers 0-9
    for i in range(10):
        os.makedirs(os.path.join(output_dir, str(i)), exist_ok=True)

    # Lire et prétraiter l'image
    img = cv2.imread(image_path)
    if img is None:
        print(f"❌ Image introuvable : {image_path}")
        return

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.adaptiveThreshold(blur, 255,
                                   cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                   cv2.THRESH_BINARY_INV, 11, 2)

    # Extraire la plus grande zone (la grille)
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if not contours:
        print("❌ Aucune grille détectée.")
        return

    largest = max(contours, key=cv2.contourArea)
    x, y, w, h = cv2.boundingRect(largest)
    grid = gray[y:y+h, x:x+w]
    grid = cv2.resize(grid, (450, 450))

    print("🧠 Tape un chiffre entre 0-9 pour chaque cellule.")
    print("   🔘 Appuie sur '0' si la cellule est vide ou contient un '-'.")
    print("   ⏭️ Appuie sur espace pour ignorer la cellule.")
    print("   🛑 Appuie sur 'q' pour quitter.")

    for i in range(9):
        for j in range(9):
            cell = grid[i*50:(i+1)*50, j*50:(j+1)*50]
            cell = cv2.resize(cell, (28, 28))
            _, cell_bin = cv2.threshold(cell, 150, 255, cv2.THRESH_BINARY_INV)

            cv2.imshow("Cellule", cell_bin)
            key = cv2.waitKey(0) & 0xFF  # Lire la touche

            if key == ord('q'):
                print("🛑 Fin manuelle.")
                cv2.destroyAllWindows()
                return

            elif key == ord(' '):
                print("⏭️ Cellule ignorée.")
                continue

            elif chr(key) in '0123456789':
                digit = chr(key)
                filename = f"{i}_{j}_{np.random.randint(10000)}.png"
                out_path = os.path.join(output_dir, digit, filename)
                cv2.imwrite(out_path, cell_bin)
                print(f"✅ Sauvé chiffre {digit} à {out_path}")
            else:
                print("⏭️ Touche invalide. Cellule ignorée.")

    cv2.destroyAllWindows()
    print("\n✅ Extraction terminée.")

if __name__ == "__main__":
    chemin = input("🖼️ Entrez le chemin de l'image du Sudoku : ").strip('"')
    preprocess_and_save_digits(chemin)
