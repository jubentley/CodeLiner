

#include "CodeInspector.h"
#include "CodeLiner.h"


#include <stdio.h>
#include <string>
#include <cstdlib>
//#include <tchar.h> 
#include <windows.h>
//#include <strsafe.h>


extern bool DEBUG;

void CodeLiner::MOTD_ExtractFName(char *argv) {

	int a = 0, b = 0, c = 0;													
	while (argv[a++]) {								//while (argv[a++] != '\0') {
		if (argv[a] == '\\') {						//null terminator is false
			b = a;
		}
	}
	while (argv[++b]) {													
		argv[c++] = argv[b];						//while (argv[++b] != '\0') {//alias
	}
	argv[c] = '\0';
}

void CodeLiner::PrintMOTD(char *argv, char *version) {

	MOTD_ExtractFName(argv);

	printf("\n\n\
                ########\n\
              ###       ####                     ###\n\
         ####   ###         ##                 ##  #\n\
      ##           ###       ##             ##v%3s#\n\
    #                          ###       ##  ##  #\n\
    #            ###              ##  ##        ##\n\
     #              ###%13s##         #   ##\n\
       ##           #  #####                  #\n\
          ###     #   #     #####            #\n\
             ###   # #          ######   ###\n\
                #### ##               ####\n\
                      ################    ###\n\
                                               ##\n\
                                        By: Justin Bentley\n\n" \
	, version, argv);
}

void CodeLiner::ExcludeFile() {

	char excludefile[FILENAMELEN] = "";

	printf("Enter Name of Excluded file (eg resource.h, Matrices.cpp)" 
		",\"n\" to skip,\"DBG\" for debug/verbose mode\n");

//	int i = 0;
	while (true) {
		fseek(stdin, 0, SEEK_END); 
		scanf("%s", &excludefile);
		if (strcmp(excludefile, "n") == 0 || strcmp(excludefile, "N") == 0) { break; }
		if (strcmp(excludefile, "dbg") == 0 || strcmp(excludefile, "DBG") == 0) { DEBUG = true; continue; }
		if (ExcludedFileNum == EXCLUDEDFILEMAX) { printf("Max Excluded file reached, see programmer\n"); break; }
		
		strcpy(ExcludedFileNames[ExcludedFileNum], excludefile); ExcludedFileNum++;

	}
	printf("");

}

void CodeLiner::Directory() {
	printf("Enter Directory, eg \nE:\\A_OpenGL4x64_MAIN\\OpenGL4x64\\OpenGL4x64\\\n"
		"E:\\CODELINER\\Codeliner\\\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%[^\n]", &FileDirectory);//scan till newline
//	scanf("%s", &FileDirectory);

	printf("");
}
void CodeLiner::ProposeFileName(char FindFileData[FILENAMELEN]) {

	char NameSplitExt[FILENAMELEN];
	strcpy(NameSplitExt, FindFileData);
	for (int i = 0; i < FILENAMELEN; i++) {
		if (NameSplitExt[i] == '.') { NameSplitExt[i] = ' '; break; }
	}
	char fname[FILENAMELEN - 5], 
		ext[20] = "\0";
		//ext[20]; //can suffer from address carryover
	sscanf(NameSplitExt, "%s %s", fname, ext);
	if (!(strcmp(ext, "cpp") == 0 
		|| strcmp(ext, "c") == 0
		|| strcmp(ext, "h") == 0 )) {
		if (DEBUG) { printf("File Rejected %s\n", FindFileData); }
		return;
	}
	for (int i = 0; i < ExcludedFileNum; i++) {
		if (strcmp(ExcludedFileNames[i], FindFileData) == 0) { 
			if (DEBUG) { printf("File Excluded %s\n", FindFileData); }
			return;
		}
	}
	strcpy(AcceptedFileNames[AcceptedFileNum], FindFileData); 
	if (DEBUG) { printf("File Accepted %s\n", AcceptedFileNames[AcceptedFileNum]); }
	AcceptedFileNum++;
}

void CodeLiner::FindFiles() {

	WIN32_FIND_DATAA FindFileData;//DATA+A (ASCII)
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char FileDirectoryWild[DIRLEN];

	strcpy(FileDirectoryWild, FileDirectory);
	strcat(FileDirectoryWild, "*");

	hFind = FindFirstFileA(FileDirectoryWild, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind /*|| FindNextFileA(hFind, &FindFileData) != 0*/) {
		printf("Invalid Directory (FindFirstFileA)"); goto exit; 
	}

//	if (DEBUG) { printf("%s\n", &FindFileData.cFileName); }

	ProposeFileName(FindFileData.cFileName);


	int lk = 100;//leak protection
	while (FindNextFileA(hFind, &FindFileData) != 0) {

//		FindNextFileA(hFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind) { printf("Invalid Directory (FindNextFileA)"); goto exit; }
//		if (DEBUG) { printf("%s\n", &FindFileData.cFileName); }
		ProposeFileName(FindFileData.cFileName);
		--lk; if (!lk) { printf("(LEAK PROTECTION)\n"); goto exit; }
	}

//	fseek(stdin, 0, SEEK_END); getchar();

	exit:
	FindClose(hFind);
}
void CodeLiner::VaraibleLengthArray() {

	returnval_vla = (int*)malloc(sizeof(int) * CodeLiner::AcceptedFileNum);
	if (returnval_vla == NULL) { printf("malloc failure"); getchar(); exit(1); }

}

void CodeLiner::InspectFiles() {
	for (int i = 0; i < AcceptedFileNum; i++) {
		CodeInspector *CI0;
		CI0 = new CodeInspector(FileDirectory, AcceptedFileNames[i], returnval_vla + i);

		/*SPAWN DELETE-ON-FINISH THREADS HERE*/

		delete CI0;
	}


}



void CodeLiner::Total() {

	int totals = 0;

	for (int i = 0; i < AcceptedFileNum; i++) {
		totals += *(returnval_vla + i);
	}
	free(returnval_vla);//int array[amt of code inspectors] for return values of LOC

	printf("\nTotal lines of valid code in non-excluded .cpp & .h files: %38d"
		"\n\nAny key to exit\n", totals);


}


CodeLiner::CodeLiner(char *argv, char *version) {
	system("COLOR 06");//purple
	PrintMOTD(argv, version);
	ExcludeFile();
	Directory();
	FindFiles();//Includes ProposeFiles
	printf("\n");

	VaraibleLengthArray();

	InspectFiles();
	Total();

	
}

CodeLiner::~CodeLiner() {


}

/*
system("COLOR XX");
0 = Black       8 = Gray
1 = Blue        9 = Light Blue
2 = Green       A = Light Green
3 = Aqua        B = Light Aqua
4 = Red         C = Light Red
5 = Purple      D = Light Purple
6 = Yellow      E = Light Yellow
7 = White       F = Bright White
*/