import array
import os

taille = os.stat("tableau.bin").st_size
tab = array.array('i')
with open("tableau.bin", "rb") as f:
    tab.fromfile(f, taille // tab.itemsize) 
print("Contenu du tableau :")
for val in tab:
    print(val, end=' ')
print()