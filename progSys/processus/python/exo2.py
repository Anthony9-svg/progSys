import os

pid = os.fork()

if pid == 0:
    print(f"Je suis le fils 1 avec le PID {os.getpid()}")
else:
    pid2 = os.fork()

    if pid2 == 0:
        print(f"Je suis le fils 2 avec le PID {os.getpid()}")
    else:
        os.wait()
        print("Je suis le père")