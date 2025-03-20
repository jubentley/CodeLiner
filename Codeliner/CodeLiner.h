#ifndef CL_def
#define CL_def 

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DIRLEN 256 
#define FILENAMELEN 128
#define EXCLUDEDFILEMAX 50
#define ACCEPTEDFILEMAX 200



//#define DEBUG_FILESFOUND TRUE
#define DEBUG_FILESFOUND FALSE

//[^\d|[|^\s|^/ |\^](\w){1,} regular expression from aiden

class CodeLiner {
private:

	int ExcludedFileNum = 0;
	char ExcludedFileNames[EXCLUDEDFILEMAX][FILENAMELEN];
	int AcceptedFileNum = 0;
	char AcceptedFileNames[ACCEPTEDFILEMAX][FILENAMELEN];
	int LinesOfCode[ACCEPTEDFILEMAX];
	char FileDirectory[DIRLEN];//Unicode Defenition, See the scanf

	unsigned int LineTotals = 0;
	int *returnval_vla;



	void MOTD_ExtractFName(char * argv);
	void PrintMOTD(char * argv, char * version);
	void ExcludeFile();
	void Directory();
	void ProposeFileName(char FindFileData[FILENAMELEN]);
	void FindFiles();
	void VaraibleLengthArray();
	void InspectFiles();
	void Total();

public:


	CodeLiner(char * argv, char * version);
	~CodeLiner();
};
#endif
