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
    inputs = input_files[0:int(len(input_files) / 2)]
    outputs = input_files[int(len(input_files) / 2):int(len(input_files))]

    # determine mac or windows, set as var for debug
    os_name = platform.system()
    if (os_name == "Windows"):
        command = "verifier.exe "
    else:
        command = "./verifier "

    # add files to commands
    project_path = os.path.realpath(os.path.dirname(__file__))
    for i in range(0, len(inputs)):
        command += "data/" + inputs[i] + " " + "data/" + outputs[i] + " "

    # Compile source
    if (os_name == "Windows"):
        subprocess.run("g++ -g src/verifier.cpp -o verifier.exe")
    else:
        subprocess.run("g++ -g src/verifier.cpp -o verifier")

    # Run command
    subprocess.run(command)
