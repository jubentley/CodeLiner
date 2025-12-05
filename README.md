# CodeLiner

Very old, made it during the holidays at uni, is and designed for only C in C++, made in VS15 if not earlier, comes with no guarantees but it is solid.

Uses Win32API (90's C windows API) to scour a directory for .cpp and .h files and a nifty algorithm to detect valid lines-of-code with a broad emphasis on counting multi-statement lines in those files (found at /Codeliner/CodeInspector.cpp).

I installed the infrastructure for threading but never implemented.

## C/C++ Filter

```C++
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
```
