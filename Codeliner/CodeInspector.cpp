#include "CodeInspector.h"

#include <stdio.h>
#include <string.h>

//extern unsigned int LineTotals;

//#define DEBUG TRUE

extern bool DEBUG;

CodeInspector::CodeInspector(
	char FileDirectory[DIRLEN], 
	char AcceptedFileNames[FILENAMELEN],
	int *returnval)
{
	strcpy(FileDirectory_CI, FileDirectory);
	strcat(FileDirectory_CI, AcceptedFileNames);
	//printf("Full File Name %s\n", FileDirectory_CI);

	FILE *infile;
	char line[LINELEN];
/*counts lines of code not commented out, will count multiple commands on one line, looks for ';' or '{' (excludes for
loops), or a line with valid ASCII*/
	infile = fopen(FileDirectory_CI, "r");
	bool slashstarflag = false, fourloopflag = false/*avoid the word "f.o.r"*/, whitespace = true;
	short linesofcode = 0;
	while (fgets(line, LINELEN, infile) > 0) {
		whitespace = true; fourloopflag = false; linesofcode = 0;
		for (int i = 0; i < LINELEN; i++) {
			if (line[i] == '\0') { break; }//end-of-line
			if (fourloopflag) { if (line[i] == ')') { fourloopflag = false; } else { continue; }}//cycle >> end '4' loop
			if (line[i] == '/') {	//		// or /* or */
				if (line[i + 1] == '/') { break; }	//commented (//) rightwards from here, end-of-line
				if (line[i + 1] == '*') { slashstarflag = true; }	//set /*
				if (i > 0) { if (line[i - 1] == '*') { slashstarflag = false; }}}	//set */
			if (!slashstarflag) {	//		out of /* X */
				if (line[i] == 'f') { if (line[i + 1] == 'o' && line[i + 2] == 'r') { fourloopflag = true; } }//detect/mask 4 loop
				if (line[i] == 0x3B/*;*/) { linesofcode++; }	// ';', int 59, !detect itself :)
				if (whitespace
					&& line[i] > 96 //97 a
					&& line[i] < 123) { //122 z					
					whitespace = false;}
				if (line[i] == 0x7B/*}*/ && !whitespace) { linesofcode++; }	// '}', int 123
			}
		}
		if (linesofcode == 0) { if (!whitespace) { linesofcode = 1; } } //check for !; ( while (fgets) { )
		LinesOfCode += linesofcode;
		//if (DEBUG) { printf(" Counted Line %s LOC %d\n", line, linesofcode); }
		if (DEBUG) { printf(" LOC %d %s", linesofcode, line); }
	}
	int strpad = (88 - strlen(FileDirectory_CI)); 
	while (--strpad > 0) { strcat(FileDirectory_CI, " "); }
	//while (--strpad) { strcat(FileDirectory_CI, " "); }

	//first string needs max value, infact get into this habbit, (negative function)
	printf("%s  %8d\n", FileDirectory_CI, LinesOfCode);

	*returnval = LinesOfCode;//threadsafe

	fclose(infile);
}

CodeInspector::~CodeInspector()
{
}
/*
only fault; 
if (NameSplitExt[i] == '.') { NameSplitExt[i] = ' '; break; }
also, bool slashstarflag = false, fourloopflag = false, whitespace = true;
should be three lines
could look for if, while, for, switch, =? but too dicky

1 line, if (NameSplitExt[i] == '.')
2 line, NameSplitExt[i] = ' '; break;
2 line, if (NameSplitExt[i] == '.') { NameSplitExt[i] = ' '; break; }

*/





//if (whitespace //skip check if already OK
//	&& line[i] != ' '	//interchangable w/ ascii a>>Z
//	&& line[i] != '}'
//	&& line[i] != '{'
//	&& line[i] != '*'
//	&& line[i] != '/'
//	&& line[i] != '\n'	//pot not needed, check me??
//	&& !slashstarflag	//check for /* x */
//	&& line[i] != '\t') { whitespace = false; }