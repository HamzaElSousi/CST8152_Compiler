/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2024
* Author: TO_DO
* Professors: Paulo Sousa
* Student Names: Hamza El Sousi && Mohammad Alashi
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (W24)  |
=---------------------------------------=

     d8888   .d8888b.                    888
    d8P888  d88P  Y88b                   888
   d8P 888  888    888                   888
  d8P  888  888         8888b.  .d8888b  888888
 d88   888  888            "88b 88K      888
 8888888888 888    888 .d888888 "Y8888b. 888
       888  Y88b  d88P 888  888      X88 Y88b.
       888   "Y8888P"  "Y888888  88888P'  "Y888
 */


/*
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/


#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

int main(int argc, char** argv) {
	int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	Cast_char option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;
	/* TO_DO: Include later SCANNER (A22) and PARSER (A32) */
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver S23
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

/* TO_DO: Update your logo with your own language message */

Cast_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=--------------------------------------------------=\n",
		"\t|  COMPILERS - ALGONQUIN COLLEGE (W24)		   |\n",
		"\t=--------------------------------------------------=\n",
		"\t|       d8888   .d8888b.                   888     |\n",
		"\t|     d8P888  d88P  Y88b                   888     |\n",
		"\t|    d8P 888  888    888                   888     |\n",
		"\t|   d8P  888  888         8888b.  .d8888b  88888   |\n",
		"\t|  d88   888  888            '88b 88K      888     |\n",
		"\t|  8888888888 888    888 .d888888 'Y8888b. 888	   |\n",
		"\t|         888  Y88b  d88P 888  888     X88 Y88b.   |\n",
		"\t|         888   'Y8888P'  'Y888888 88888P'  'Y888  |\n",
		"\t=--------------------------------------------------=\n"
	);
}
