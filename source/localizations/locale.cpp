#include <string>
#define LOCALE_CPP
#include "generated/languages.h"



const char** loadedLanguageTags;
const char** loadedLanguageValues;
int 	loadedLanguageNum;

const char* locale(const char* tagname)
{
	std::string tag(tagname);

	for (int i = 0; i <= loadedLanguageNum; ++i)
	{
		if (tag.compare(loadedLanguageTags[i])==0)
			return loadedLanguageValues[i];
	}
	return tagname;
}

void setLanguage(int ID)
{
	loadedLanguageTags = languagesTags[ID];
	loadedLanguageValues = languagesValues[ID];
	loadedLanguageNum = languagesNums[ID];
}

int getNumLanguages()
{
	return numLanguages;
}
