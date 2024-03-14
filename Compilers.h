/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2024
* Author: Hamza El Sousi
* Professors: Paulo Sousa
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
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* Language name */
#define STR_LANGNAME	"4Cast"

/* Logical constants - adapt for your language */
#define True  1
#define False 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/


/* TO_DO: Define your typedefs */                    // THE TASK HERE IS TO MAKE SURE THAT YOUR CODE ARE SIMILARE TO PYTHON 
                                                    // REMOVED : typdef int    int, 
                                                   //  REMOVED : typedef float   float;
typedef char			Cast_char;
typedef char*           str;
typedef void			Cast_void;

typedef unsigned char	Cast_boln;
typedef unsigned char	Cast_byte;

typedef long			Cast_long;
typedef double			Cast_double;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'r',
	PGM_SCANNER = 's',
	PGM_PARSER	= 'p'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
int mainReader(int argc, str* argv);
int mainScanner(int argc, str* argv);

/* 
TO_DO: Include later mainScaner (A22) and mainParser (A32)
*/
Cast_void printLogo();

#endif
