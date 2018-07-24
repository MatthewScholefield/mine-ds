#!/usr/bin/python3
# coding: UTF-8

import sys
from os import makedirs

import os
from argparse import ArgumentParser


def process(filename, out_folder):
    print("Reading file \"" + filename + "\"")
    f = open(filename, 'r')
    tags = []
    values = []
    basename = os.path.basename(os.path.splitext(filename)[0])
    for line in f:
        list = line.replace("\n", "").split("=")
        if (len(list) == 2):
            tags.append(list[0])
            values.append(list[1])
    string = "#pragma once\nconst char* " + basename + "_tags[]={\"locale\""
    for tag in tags:
        string += ",\"" + tag + "\""
    string += "};\n"
    string += "const char* " + basename + "_values[]={\"" + basename + "\""
    for value in values:
        string += ",\"" + value + "\""
    string += "};\n"
    string += "int " + basename + "_num = " + str(len(tags)) + ";"

    out_file = os.path.join(out_folder, basename + ".h")
    wfile = open(out_file, "wb")

    # For creating the languages.h file

    ostring = string
    string = ""
    for char in ostring:
        # German ß should be replaced with ss
        if (char == u"ß"):
            string += "ss"
        # Esperanto ^ letters can be replaced with x
        # ĝ -> gx ĉ -> cx etc
        elif (char == u"ĝ"):
            string += u"â"
        elif (char == u"ŝ"):
            string += u"á"
        elif (char == u"ĉ"):
            string += u"à"
        elif (char == u"ĵ"):
            string += u"ã"
        elif (char == u"ŭ"):
            string += u"å"
        else:
            string += char

    wfile.write(string.encode("ISO-8859-15"))
    wfile.close()
    return basename


root = os.path.dirname(os.path.realpath(__file__))

print('ARGS:', sys.argv, file=sys.stderr)

parser = ArgumentParser()
parser.add_argument('filenames', nargs='*')
parser.add_argument('-o', '--out-folder', default='')
args = parser.parse_args()
fileNames = [process(filename, args.out_folder) for filename in args.filenames]
makedirs(root + "/source/localizations/generated/", exist_ok=True)
languagesFile = open(root + "/source/localizations/generated/languages.h", "w")
languagesFile.write("#ifdef LOCALE_CPP\n#pragma once\n")
for name in fileNames:
    languagesFile.write("#include \"" + name + ".h\"\n")
languagesFile.write("const int numLanguages = " + str(len(fileNames)) + ";\n")
languagesFile.write("const char** languagesTags[] = {")
comma = False
for name in fileNames:
    if (comma):
        languagesFile.write(",")
    languagesFile.write(name + "_tags")
    comma = True
languagesFile.write("};\n")

languagesFile.write("const char** languagesValues[] = {")
comma = False
for name in fileNames:
    if (comma):
        languagesFile.write(",")
    languagesFile.write(name + "_values")
    comma = True
languagesFile.write("};\n")

languagesFile.write("const int languagesNums[] = {")
comma = False
for name in fileNames:
    if (comma):
        languagesFile.write(",")
    languagesFile.write(name + "_num")
    comma = True
languagesFile.write("};\n#endif\n")

languagesFile.close()
