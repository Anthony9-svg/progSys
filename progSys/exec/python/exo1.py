import sys


if len(sys.argv) < 3:
    print("Usage : python script.py <nombre1> <nombre2>")
    sys.exit(1)

a = int(sys.argv[1])
b = int(sys.argv[2])

print(f"{a}+{b}={a+b}")