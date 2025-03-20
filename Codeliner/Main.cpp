
#include <stdio.h>

#include "CodeLiner.h"

CodeLiner *CL;

#define VERSION "1.4"

bool DEBUG = false;
//bool DEBUG = true; (command in program)

int main(int argc, char **argv) {


	CL = new CodeLiner(argv[0], VERSION);
	delete CL;

	/*pause*/ 
	fseek(stdin, 0, SEEK_END); 
	char c = scanf("%c", &c);


	exit(0);





}
