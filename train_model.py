import os
import numpy as np
import cv2
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense, Dropout
from tensorflow.keras.utils import to_categorical
from sklearn.model_selection import train_test_split
from tensorflow.keras.callbacks import ModelCheckpoint
import matplotlib.pyplot as plt

def load_data(dataset_path="digits_dataset"):
    X, y = [], []
    for label in os.listdir(dataset_path):
        label_dir = os.path.join(dataset_path, label)
        if not os.path.isdir(label_dir):
            continue
        for file in os.listdir(label_dir):
            img_path = os.path.join(label_dir, file)
            try:
                img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
                if img is None:
                    print(f"Image non valide : {img_path}")
                    continue
                img = cv2.resize(img, (28, 28)) / 255.0
                X.append(img)
                y.append(int(label))
            except Exception as e:
                print(f"Erreur lors du chargement de l'image {img_path}: {e}")
                continue
    X = np.expand_dims(np.array(X), -1)
    y = to_categorical(np.array(y), num_classes=10)
    return train_test_split(X, y, test_size=0.2)

def build_model():
    model = Sequential([
        Conv2D(32, (3,3), activation='relu', input_shape=(28,28,1)),
        MaxPooling2D(2,2),
        Dropout(0.2),
        Conv2D(64, (3,3), activation='relu'),
        MaxPooling2D(2,2),
        Dropout(0.2),
        Flatten(),
        Dense(128, activation='relu'),
        Dropout(0.5),
        Dense(10, activation='softmax')
    ])
    model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
    return model

if __name__ == "__main__":
    X_train, X_test, y_train, y_test = load_data()
    model = build_model()

    os.makedirs("models", exist_ok=True)
    checkpoint = ModelCheckpoint("models/best_digit_model.h5", save_best_only=True, monitor="val_accuracy", mode="max")
    history = model.fit(X_train, y_train, epochs=10, validation_data=(X_test, y_test), callbacks=[checkpoint])

    # Visualiser la courbe d'apprentissage
    plt.plot(history.history['accuracy'], label='Précision entraînement')
    plt.plot(history.history['val_accuracy'], label='Précision validation')
    plt.xlabel("Époques")
    plt.ylabel("Précision")
    plt.legend()
    plt.grid(True)
    plt.show()

    print("✅ Modèle sauvegardé dans models/best_digit_model.h5")
