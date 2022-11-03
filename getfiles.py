import pathlib
import os
import platform
import subprocess
from os import listdir
from os.path import isfile, join

if __name__ == '__main__':

    # Get list of input files and their outputs
    project_directory = os.path.realpath(os.path.dirname(__file__))
    mypath = pathlib.Path(__file__).parent.resolve() / 'data'
    input_files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    input_files.sort()
    inputs = []
    str = ""
    for f in input_files:
        if "OUTPUT" not in f:
            inputs.append(f)
            str += "data/" + f + "\n "
    print(str)