# the script to convert folder with CCG format instances
# the folder need to have *.col files and single file M.in
# M.in is to have single line with M integer variable
# the script runs ccg2dimacs converter for N in 3..M 

import os
import subprocess
import argparse

SUBPROCESS_STRING = "python3 coloring_script.py '{files_mask:s}' {N:d} -folder N{N:d}_DIMACS"


parser = argparse.ArgumentParser()
parser.add_argument("folder", help = "folder with M.in file and CCG-format instances")
args = parser.parse_args()

m_filename = os.path.join(args.folder, "M.in")
m_file = open(m_filename, "r")
M = int(m_file.read())
m_file.close()

mask = os.path.join(args.folder, "*.col")

for i in range(3, M + 1):
    try:
        command = SUBPROCESS_STRING.format(**{"files_mask":mask, "N":i})
        retcode = subprocess.call(command, shell=True)
        if retcode < 0:
            print("An error occured ", -retcode, file=sys.stderr)
        else:
            print("N=" + str(i) + " DONE")
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)
