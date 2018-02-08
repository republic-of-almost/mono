#ifndef TYPES_INCLUDED_07B0AC05_DA3F_4387_B327_CB79998954A7
#define TYPES_INCLUDED_07B0AC05_DA3F_4387_B327_CB79998954A7


#include <ctime>
#include <stdlib.h>
#include <vector>
#include <string>


struct GUID
{
	char str[40];

	GUID()
	{
		static int i = 0;
		i++;

		srand(clock() + i);

		int t = 0;
		char *szTemp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
		char *szHex = "0123456789ABCDEF-";
		int nLen = strlen(szTemp);

		for (t = 0; t<nLen + 1; t++)
		{
			int r = rand() % 16;
			char c = ' ';

			switch (szTemp[t])
			{
			case 'x': { c = szHex[r]; } break;
			case 'y': { c = szHex[r & 0x03 | 0x08]; } break;
			case '-': { c = '-'; } break;
			case '4': { c = '4'; } break;
			}

			str[t] = (t < nLen) ? c : 0x00;
		}
	}
};


struct Config
{
  std::string name;
  int symbols;
  int optim_level;
  int arch;
};


enum
{
  SED_FILE_TYPE_TEXT,
  SED_FILE_TYPE_HEADER,
  SED_FILE_TYPE_SOURCE,
};


struct File
{
  std::string name;
  std::string path;
  int type;
};


struct Project
{
  std::string name;
  std::string path;
  
  std::vector<File> files;

  std::vector<std::string> inc_dirs;
  std::vector<std::string> lib_dirs;
  int lang;
  int kind;

  GUID guid;
};


struct Solution
{
  std::string name;
  std::string path;

  std::vector<Project> projects;
  std::vector<Config> configs;

  struct GUID guid;
};


#endif /* inc guard */
