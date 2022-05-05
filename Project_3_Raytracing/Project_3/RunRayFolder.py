import os
import subprocess


# path to the folder with all the ray files.
path = './Media/my_scenes/animation'

# for every ray file in the folder run the os command and wait untill it is finished.
for root, dirs, files in os.walk(path, topdown=False):
    for name in files:
        if name[-4:] == '.ray':
        	cpath = os.path.join(root, name)
        	cname = name[:-4]
        	system_call = "Project_3.exe " + cpath[8:] + " y " + cname + "\n"
        	print(system_call)
        	os.system("start /wait cmd /c "+ system_call)