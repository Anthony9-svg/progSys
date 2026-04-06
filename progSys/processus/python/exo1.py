import os

pid = os.fork()

if pid == 0:
    print("Je suis le fils")
else:
    os.waitpid(pid, 0)
    print("Je suis le pere")