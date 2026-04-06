import sys
import subprocess

if len(sys.argv) < 2:
    print("Usage : python script.py <chemin_source>")
    sys.exit(1)

source = sys.argv[1]
subprocess.run(["cp", "-r", source, "."], check=True)