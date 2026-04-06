import subprocess
arg = input("Donnez le nom du fichier: ")
subprocess.run(["nano", arg])