import os
import stat

while True:
    directory = input("Repertoire: ")
    
    if directory == "stop":
        break

    if not os.path.isdir(directory):
        print("Erreur d'ouverture du repertoire")
        continue

    print(f"Repertoire: {directory}")

    for entry in os.listdir(directory):
        chemin = os.path.join(directory, entry)
        try:
            st = os.stat(chemin)
        except OSError:
            print(f"Erreur d'accès au fichier {entry}")
            print(chemin)
            continue

        if stat.S_ISREG(st.st_mode) and st.st_size > 1000:
            print(f"Fichier: {entry}, UID du proprietaire: {st.st_uid}, taille: {st.st_size}")

    print()