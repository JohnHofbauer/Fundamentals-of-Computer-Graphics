import os
import subprocess
import math
distance = 4
counter = 1
for thedaD in range(0, 360):
	path = "./Media/my_scenes/animation/ray"+ str(counter) +".ray"
	f = open(path, "w")
	counter+=1

	

	f.write("Background {\n")
	f.write("\tcolor 0.05 0.05 0.05\n")
	f.write("\tambientLight 0.2 0.2 0.2 \n")
	f.write("}\n")
	f.write("Camera {\n")
	f.write("\teye 0 5 3\n")
	f.write("\tlookAt 15 0 0\n")
	f.write("\tup 0 0 1\n")
	f.write("\tfovy 45\n")
	f.write("}\n")
	f.write("Lights {\n")
	f.write("\tLight {\n")
	f.write("\tposition 11 0 10\n")
	f.write("\tcolor 1 1 1\n")
	f.write("\t}\n")
	f.write("\tLight {\n")
	f.write("\tposition 30 0 0\n")
	f.write("\tcolor .3 .3 .3\n")
	f.write("\t}\n")
	f.write("}\n")
	f.write("Materials {\n")
	f.write("\tMaterial {\n")
	f.write("\t\ttextureFilename NULL\n")
	f.write("\t\tdiffuseColor 0.5 0 0\n")
	f.write("\t\tspecularColor 0.3 0.3 0.3\n")
	f.write("\t\treflectiveColor 0.3 0.3 0.3\n")
	f.write("\t\tshininess 50\n")
	f.write("\t\ttransparentColor 0 0 0 \n")
	f.write("\t\tindexOfRefraction 0.6\n")
	f.write("\t}\n")
	f.write("\tMaterial {\n")
	f.write("\t\ttextureFilename NULL\n")
	f.write("\t\tdiffuseColor 0.5 0.5 0.5\n")
	f.write("\t\tspecularColor 0.3 0.3 0.3\n")
	f.write("\t\treflectiveColor 0.5 0.5 0.5\n")
	f.write("\t\tshininess 50\n")
	f.write("\t\ttransparentColor 0 0 0 \n")
	f.write("\t\tindexOfRefraction 0.6\n")
	f.write("\t}\n")
	f.write("\tMaterial {\n")
	f.write("\t\ttextureFilename NULL\n")
	f.write("\t\tdiffuseColor 0 0 0.5\n")
	f.write("\t\tspecularColor 0.8 0.8 0.8\n")
	f.write("\t\treflectiveColor 0.3 0.3 0.3\n")
	f.write("\t\tshininess 50\n")
	f.write("\t\ttransparentColor 0 0 0 \n")
	f.write("\t\tindexOfRefraction 0.6\n")
	f.write("\t}\n")
	f.write("}\n")

	
	theda = math.radians(thedaD)
	x = math.cos(theda) * distance
	y = math.sin(theda) * distance
	x = str(x)
	y = str(y)

	f.write("Group {\n")
	f.write("\tSphere {\n")
	f.write("\t\tmaterialIndex 1\n")
	f.write("\t\tcenter 15 "+ x +" "+ y +"\n")
	f.write("\t\tradius 2\n")
	f.write("\t}\n")

	theda = math.radians(thedaD+90)
	x = math.cos(theda) * distance
	y = math.sin(theda) * distance
	x = str(x)
	y = str(y)
	f.write("\tSphere {\n")
	f.write("\t\tmaterialIndex 1\n")
	f.write("\t\tcenter 15 "+ x +" "+ y +"\n")
	f.write("\t\tradius 2\n")
	f.write("\t}\n")

	theda = math.radians(thedaD+180)
	x = math.cos(theda) * distance
	y = math.sin(theda) * distance
	x = str(x)
	y = str(y)
	f.write("\tSphere {\n")
	f.write("\t\tmaterialIndex 1\n")
	f.write("\t\tcenter 15 "+ x +" "+ y +"\n")
	f.write("\t\tradius 2\n")
	f.write("\t}\n")

	theda = math.radians(thedaD+270)
	x = math.cos(theda) * distance
	y = math.sin(theda) * distance
	x = str(x)
	y = str(y)
	f.write("\tSphere {\n")
	f.write("\t\tmaterialIndex 1\n")
	f.write("\t\tcenter 15 "+ x +" "+ y +"\n")
	f.write("\t\tradius 2\n")
	f.write("\t}\n")
	f.write("}\n")

	f.close()