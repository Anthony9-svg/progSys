import os

for i in range(5):
    pid = os.fork()

    if pid == 0:
        print(f"Je suis le fils numero {i+1}, pid={os.getpid()}")
        break
    else:
        os.wait()
        print("Je suis le pere")