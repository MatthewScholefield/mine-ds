#!/usr/bin/python

import sys
import os

print("Creating localisation files...\n\n")

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
	string = "const char* "+basename+"_tags[]={\"locale\""
	for tag in tags:
		string += ",\"" + tag + "\""
	string += "};\n"
	string += "const char* "+basename+"_values[]={\"" + basename + "\""
	for value in values:
		string += ",\"" + value + "\""
	string += "};\n"
	string += "int "+basename+"_num = " + str(len(tags)) + ";"

	wfile = open("source/localizations/" + basename + ".h","w")
	wfile.write(string)
	
