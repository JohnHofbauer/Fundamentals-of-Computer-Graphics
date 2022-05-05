from random import random, randrange
from math import floor

f = open("x.sp", "w")



print("0 0 0")
f.write("0 0 0\n")
print("5 0 0")
f.write("5 0 0\n")

distance = 10
numberOfPoints = 4
height = 4
globalstart = [5, 0, 0]
for x in range(1, numberOfPoints):
    #magnatude = 0
    for i in range(0, 3):
        if i ==1:
            if randrange(0,2) == 1:
                globalstart[i] += randrange(floor(height/2),height)
            else:
                globalstart[i] += randrange(-height,-floor(height/2))
        else:
            if randrange(0,1) == 1:
                globalstart[i] += randrange(floor(distance/2),distance)
            else:
                globalstart[i] += randrange(-distance,-floor(distance/2))
        #magnatude += globalstart[i]**2
        
    

    # Normalize
    #for i in range(0, 3):
    #    globalstart[i] = floor(globalstart[i]/(magnatude/100))
    
    

    print(globalstart[0],globalstart[1],globalstart[2])
    f.write(str(str(globalstart[0])+" "+str(globalstart[1])+" "+str(globalstart[2])+"\n"))
    
for x in range(1, 100):
    magnatude = 0
    for i in range(0, 3):
        if globalstart[i] >= 10:
            globalstart[i] += randrange(-distance, 6)
        if globalstart[i] < 10 and globalstart[i] > 0:
            globalstart[i] = 0
        if globalstart[i] <= -10:
            globalstart[i] += randrange(6, distance)
        if globalstart[i] > -10 and globalstart[i] < 0:
            globalstart[i] = 0
        magnatude += globalstart[i]**2
    print(globalstart[0],globalstart[1],globalstart[2])
    f.write(str(str(globalstart[0])+" "+str(globalstart[1])+" "+str(globalstart[2])+"\n"))
    if globalstart[0] == 0 and globalstart[1] == 0 and globalstart[2] == 0:
        f.close()
        break
        

file = open("x.sp","r")
Counter = 0
  
Content = file.read()
CoList = Content.split("\n")
  
for i in CoList:
    if i:
        Counter += 1
          

file.close()
print(Counter)


with open("x.sp", "r") as f:
    contents = f.readlines()

contents.insert(0, str(Counter)+"\n")

with open("x.sp", "w") as f:
    contents = "".join(contents)
    f.write(contents)
f.close()
