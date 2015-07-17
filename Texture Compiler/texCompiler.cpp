#include <iostream> 
#include <fstream>
#include <vector>
#include <unistd.h> /* chdir */
#include <stdlib.h> /* system */
#include <sys/stat.h> /* struct stat */
#include <stdio.h> /* remove */
using namespace std;

bool doesFileExists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char *argv[])
{
	if (chdir(argv[1]) == -1)
	{
		printf("Usage:\n"
				"texCompiler [FOLDER]\n");
		return 0;
	}

	if (!doesFileExists("blocks.png") || !doesFileExists("mobs.png") || !doesFileExists("UI.png"))
	{
		printf("Error, the folder should contain the following:\n"
				" - blocks.png\n"
				" - mobs.png\n"
				" - UI.png\n");
		return 0;
	}

	system("grit blocks.png -ftbin -gB8 -gt -th 16 -gT FF00FF -m! -fh!");
	system("grit mobs.png -ftbin -gB8 -gt -Mh4 -Mw2 -gT FF00FF -m! -fh!");
	system("grit UI.png -ftbin -gB8 -gt -gT FF00FF -m! -fh!");

	vector<string> names;
	vector<ifstream> files;
	names.emplace_back("blocks.img.bin");
	names.emplace_back("blocks.pal.bin");
	names.emplace_back("mobs.img.bin");
	names.emplace_back("mobs.pal.bin");
	names.emplace_back("UI.img.bin");
	names.emplace_back("UI.pal.bin");

	string outName(argv[1]);
	outName += ".tex";
	ofstream output(outName.c_str(), ios::binary);
	for (std::vector<string>::iterator i = names.begin(); i != names.end(); ++i)
	{
		files.emplace_back(i->c_str(), ios::binary);
		output << files.back().rdbuf();
		files.back().close();
		remove(i->c_str());
	}
	output.close();
	chdir("..");
	string newPath = outName;
	outName=string(argv[1])+"/"+outName;
	rename(outName.c_str(), newPath.c_str());
	return 0;
}
