import os

num = 1

for i in range(5):
    pid = os.fork()
    if pid == 0:
        print(f"Je suis le fils {num}")
        break
    else:
        num += 1