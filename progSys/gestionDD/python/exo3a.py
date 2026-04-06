import array


n = int(input("Nombre d'elements : "))
tab = array.array('i') 
for i in range(n):
    val = int(input(f"tab[{i}] = "))
    tab.append(val)
with open("tableau.bin", "wb") as f:
    tab.tofile(f)