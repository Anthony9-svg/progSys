import os
import time
import random

random.seed()

time1 = random.randint(1, 10)
time2 = random.randint(1, 10)

pid1 = os.fork()

if pid1 == 0:
    time.sleep(time1)
    print(f"Je suis le fils 1 qui a duré {time1} secondes")
else:
    pid2 = os.fork()
    
    if pid2 == 0:

        time.sleep(time2)
        print(f"Je suis le fils 2 qui a duré {time2} secondes")
    else:
        if time2 > time1:
            os.waitpid(pid2, 0)
            os.waitpid(pid1, 0)
        else:
            os.waitpid(pid1, 0)
            os.waitpid(pid2, 0)