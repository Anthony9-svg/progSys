import os
import sys
import subprocess

while True:
    pid = os.fork()

    if pid == 0:
        fichier = input('Fichier (Entrez "stop" pour quitter): ')

        if fichier == "stop":
            os._exit(3) 
        else:      
            subprocess.run(["emacs", fichier])
            os._exit(0)
    else:
        pid_waited, status = os.wait()
        if os.WIFEXITED(status) and os.WEXITSTATUS(status) == 3:
            sys.exit(0)