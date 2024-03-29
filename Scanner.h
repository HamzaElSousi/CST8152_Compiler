/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Hamza El Sousi & Mohammed Alashi 
* Professors: Paulo Sousa
************************************************************
=---------------------------------------------=
|      COMPILERS - ALGONQUIN COLLEGE (W24)    |
=---------------------------------------------=


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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif


#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 16

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	FLT_T,		/*   float literal token*/
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	EOS_T,		/*  end of statement marked by end of line*/
	KW_T,		/*  8: Keyword token */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	LOG_OP_T,   /* Logical operators : &&, || , ! */
	REL_OP_T,	/* Relational operators: ==, !=, >, <, >=, <= */
	ASSIGN_OP_T,/* Assignment operators: =, +=, -=, */
	ARTH_T, // Arithmetic operations token
	//LOG_T,  // Logical operations token
	//NUM_TOKENS
};


/* TO_DO: Define the list of keywords */
static str tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"FLT_T"
	"STR_T",
	"LPR_T",
	"RPR_T",
	"EOS_T",
	"KW_T",	
	"RTE_T",
	"SEOF_T",
	"ARTH_T",
	"LOG_OP_T",
	"CMT_T",
	"LOG_OP_T",   /* Logical operators : &&, || , ! */
	"REL_OP_T",	  /* Relational operators: ==, !=, >, <, >=, <= */
	"ASSIGN_OP_T" /* Assignment operators: =, +=, -=, */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	int intValue;				/* integer literal attribute (value) */
	int keywordIndex;			/* keyword index in the keyword table */
	int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	Cast_double floatValue;				/* floating-point literal attribute (value) */
	Cast_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	Cast_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	Cast_byte flags;			/* Flags information */
	union {
		int intValue;				/* Integer value */
		float floatValue;			/* Float value */
		str* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '#'
#define CHRCOL4 '\"'
#define CHRCOL5 '.'
#define CHRCOL6 '('
#define CHRCOL7 '\n'

/* These constants will be used on VID / MID function */
#define MNID_SUF '('
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	16		/* Error state with no retract */
#define ESWR	17		/* Error state with retract */
#define FS		18		/* Illegal state */

/* Define accepting states types */
#define NOFS	19		/* not accepting state */
#define FSNR	20		/* accepting state with no retract */
#define FSWR	21		/* accepting state with retract */

 /* TO_DO: State transition table definition */
#define NUM_STATES		18
#define CHAR_CLASSES	10



/* TO_DO: Transition table - type of states defined in separate table */
static int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,    #,   \",    .,    (,    \n	   EOF		Other
	   L(0), D(1), U(2), H(3), Q(4), P(5), B1X(6) N(7)     EOF		O		*/	
	{     1,	8, ESNR,   14,    6,   12,	ESNR, ESNR,	 FSNR,		ESNR},	    // S0: start
	{     1,    2,    2, FSWR, FSWR,    3,    4,     3,	    2,		   3},		// S1: start with a letter
	{     2,    2,    2, FSWR, FSWR, FSWR,	  4,     3,	    3, 	       3},		// S2: variable name or function name
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,		FSNR},		// S3: variable name final state
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,		FSNR},		// S4: function name final state
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,	 	FSNR},		// S5: keyword final state
	{     6,    6,    6,    6,    7,    6,	  6,     6,   ESNR,		   6},		// S6: looping to fill string
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,		FSNR},		// S7: string final state
	{    10,    8,   10,   10,   10,    9,	 10,    10,	   10,		  10},		// S8: got a digit, either int or float
	{    11,	9,	 11,   11,   11,   11,   11,    11,	   11,		  11},		// S9: got a period, float starting with a number
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,		FSNR},		// S10: Integer final state
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,		FSNR},		// S11: Float final state
	{  ESNR,   12, ESNR, ESNR, ESNR, ESNR, ESNR,    11,  ESNR,		ESNR},		// S12: float that starts with a .
	{	 11,   13,   11,   11,   11,   11,   11,    11,	 FSNR,		  11},		// S13: looping for float that starts with a .
	{	 14,   14,   14,   14,   14,   14,   14,    15,	 FSNR,		  14},		// S14: looping for Comment
	{  FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR,  FSNR,	 FSNR,	    FSNR},		// S15: Comment end state
    
};


/* TO_DO: Define list of acceptable states */
static int stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	NOFS, /* 02*/
	FSNR, /* 03 Variable name */
	FSNR, /* 04 function name */
	FSNR, /* 05 keyword */
	NOFS, /* 06*/
	FSNR, /* 07 String */
	NOFS, /* 08 */
	NOFS, /* 09 */
	FSNR, /* 10 Integer */
	FSNR, /* 11 Float */
	NOFS, /* 12 */
	NOFS, /* 13 */
	NOFS, /* 14*/
	FSNR, /* 15 Comment */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
int			startScanner(BufferPointer psc_buf);
static int	nextClass(Cast_char c);					/* character class function */
static int	nextState(int, Cast_char);		/* state machine function */
void			printScannerData(ScannerData scData);
Token				tokenizer(Cast_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(str lexeme);


/* Declare accepting states functions */
Token funcSL	(str lexeme);
Token funcIL	(str lexeme);
Token funcVAR   (str lexeme);
Token funcFLT	(str lexeme);
Token funcID	(str lexeme);
Token funcCMT   (str lexeme);
Token funcKEY	(str lexeme);
Token funcErr	(str lexeme);
Token funcLOG_OP (str lexeme);


/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES ] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	NULL,		/* -    [02] */
	funcVAR,	/* -    [03] */
	funcID,		/* MNID	[04] */
	funcKEY,	/* KEY  [05] */
	NULL,		/* -    [06] */
	funcSL,		/* SL   [07] */
	NULL,		/* -    [08] */
	NULL,		/* -    [09] */
	funcIL,		/* -    [10] */
	funcFLT,	/* -    [11] */
	NULL,		/* -    [12] */
	NULL,		/* -    [13] */
	NULL,		/* -    [14] */
	funcCMT,	/* COM  [15] */
	funcErr,
	funcLOG_OP // Assuming this is the correct state for a logical operator

};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* TO_DO: Define the list of keywords */
static str keywordTable[KWT_SIZE] = {
	"if",		/* KW00 */
	"elif",		/* KW01 */
	"for",		/* KW02 */
	"while",	/* KW03 */
	"class",	/* KW04 */
	"return",	/* KW05 */
	"break",	/* KW06 */
	"continue",	/* KW07 */
	"def",		/* KW08 */
	"import"	/* KW09 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	Cast_char indentationCharType;
	int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
int numScannerErrors;

/* Scanner data */
ScannerData scData;


#endif // SCANNER_H_