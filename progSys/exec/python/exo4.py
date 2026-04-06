import os
import subprocess
import sys

while True:
    pid = os.fork()

    if pid == 0:
        # Fils
        input_dir = input('Repertoire (Entrez "stop" pour quitter): ')

        if input_dir == "stop":
            os._exit(3)  # équivalent exit(3) en C
        else:
            # copie du répertoire courant vers le répertoire donné
            # équivalent à `cp -r . <input_dir>`
            subprocess.run(["cp", "-r", ".", input_dir])
            os._exit(0)  # fin du fils
    else:
        # Père
        pid_waited, status = os.wait()
        if os.WIFEXITED(status) and os.WEXITSTATUS(status) == 3:
            sys.exit(0)