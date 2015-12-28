#!/usr/bin/python
# coding: UTF-8

import sys
import os

print("Creating localisation files...\n\n")

numFiles=0
fileNames = []

for arg in sys.argv[1:]:
	

	
	print("Reading file \"" + arg + "\"")
	f = open("source/localizations/" + arg,'r')
	tags = []
	values = []
	basename = arg.split(".")[0]
	for line in f:
		list = line.replace("\n","").split("=")
		if (len(list)==2):
			tags.append(list[0])
			values.append(list[1])
	string = "#pragma once\nconst char* "+basename+"_tags[]={\"locale\""
	for tag in tags:
		string += ",\"" + tag + "\""
	string += "};\n"
	string += "const char* "+basename+"_values[]={\"" + basename + "\""
	for value in values:
		string += ",\"" + value + "\""
	string += "};\n"
	string += "int "+basename+"_num = " + str(len(tags)) + ";"

	wfile = open("source/localizations/" + basename + ".h","wb")

	#For creating the languages.h file

	numFiles += 1
	fileNames.append(basename)

	if (sys.version_info[0]<3):
		string = string.decode("UTF-8")

	ostring = string
	string = ""
	for char in ostring:
		#German ß should be replaced with ss
		if (char==u"ß"):
			string += "ss"
		#Esperanto ^ letters can be replaced with x
		# ĝ -> gx ĉ -> cx etc
		elif (char==u"ĝ"):
			string += "gx"
		elif (char==u"ŝ"):
			string += "sx"
		elif (char==u"ĉ"):
			string += "cx"
		else:
			string += char
			

	wfile.write(string.encode("ISO-8859-15"))
	wfile.close()
	

languagesFile = open("source/localizations/languages.h","w")
languagesFile.write("#ifdef LOCALE_CPP\n#pragma once\n")
for name in fileNames:
	languagesFile.write("#include \""+name	+".h\"\n")
languagesFile.write("const int numLanguages = "+str(numFiles)+";\n")
languagesFile.write("const char** languagesTags[] = {")
comma = False
for name in fileNames:
	if (comma):
		languagesFile.write(",")
	languagesFile.write(name+"_tags")
	comma = True
languagesFile.write("};\n")

languagesFile.write("const char** languagesValues[] = {")
comma = False
for name in fileNames:
	if (comma):
		languagesFile.write(",")
	languagesFile.write(name+"_values")
	comma = True
languagesFile.write("};\n")

languagesFile.write("const int languagesNums[] = {")
comma = False
for name in fileNames:
	if (comma):
		languagesFile.write(",")
	languagesFile.write(name+"_num")
	comma = True
languagesFile.write("};\n#endif\n")

languagesFile.close()



