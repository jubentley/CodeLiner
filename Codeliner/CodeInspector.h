#ifndef CI_def
#define CI_def 

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define DIRLEN 156 
#define FILENAMELEN 128
#define LINELEN 500

//#define DEBUG TRUE
//#define DEBUG FALSE

//#define EXCLUDEDFILEMAX 50
//#define ACCEPTEDFILEMAX 200
//#define DEBUG_FILESFOUND TRUE
//#define DEBUG_FILESFOUND FALSE


class CodeInspector {
private:

public:

	char bb = 'f';

	int LinesOfCode = 0;

	char FileDirectory_CI[DIRLEN];

	CodeInspector(char FileDirectory[DIRLEN], char AcceptedFileNames[FILENAMELEN], int * returnval);

	~CodeInspector();
};
#endif
