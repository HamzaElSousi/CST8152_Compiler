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

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */
#define Symbole_TOKENS 10

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 16

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (_) */
	IN_T,		/*  2: Integer literal token */
	STRL_T,		/*  3: String literal token */
	FL_T,       /*  4: Float  litreal toekn    */
	LPR_T,	    /*  4: Left parenthesis token */
	RPR_T,	    /*  5: Right parenthesis token */
	LBR_T,	    /*  6: Left brace token */
	RBR_T,	    /*  7: Right brace token */
	NEWLINE_T,  /* 9: New line token */
	KW_T,		/*  8: Keyword token */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	INDENT_T,   /* 7: Indentation increase token */
	

	

};

/* TO_DO: Define the list of keywords */
static str tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	 "FL_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"NEWLINE_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"INDENT_T",
	
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators {
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_FLOOR_DIV, OP_MOD, OP_EXP
} AriOperator;

typedef enum RelationalOperators {
	OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE
} RelOperator;

typedef enum LogicalOperators {
	OP_AND, OP_OR, OP_NOT
} LogOperator;

typedef enum BitwiseOperators {
	OP_BITWISE_AND, OP_BITWISE_OR, OP_BITWISE_XOR, OP_BITWISE_NOT, OP_LSHIFT, OP_RSHIFT
} BitwiseOperator;

typedef enum AssignmentOperators {
	OP_ASSIGN, OP_ADD_ASSIGN, OP_SUB_ASSIGN, OP_MUL_ASSIGN, OP_DIV_ASSIGN, OP_FLOOR_DIV_ASSIGN,
	OP_MOD_ASSIGN, OP_EXP_ASSIGN, OP_BITWISE_AND_ASSIGN, OP_BITWISE_OR_ASSIGN,
	OP_BITWISE_XOR_ASSIGN, OP_LSHIFT_ASSIGN, OP_RSHIFT_ASSIGN
} AssignOperator;

typedef enum MembershipOperators {
	OP_IN, OP_NOT_IN
} MembershipOperator;

typedef enum IdentityOperators {
	OP_IS, OP_IS_NOT
} IdentityOperator;

typedef enum SourceEndOfFile {
	SEOF_0, SEOF_255
} EofOperator;


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
		Cast_double floatValue;			/* Float value */
		str stringContent;		/* String value */
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
#define CHRCOL2 '('
#define CHRCOL3 ')'
#define CHRCOL4 '\''
#define CHRCOL5 '_'
#define CHRCOL6 '#'

/* These constants will be used on VID / MID function */
#define MNID_SUF '_'
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		10
#define CHAR_CLASSES	8

/* TO_DO: Transition table - type of states defined in separate table */
static int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */
	{     1, ESNR, ESNR, ESNR,    4, ESWR,	  6, ESNR},	// S0: NOAS
	{     1,    1,    1,    2,	  3,    3,   3,    3},	// S1: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S2: ASNR (MVID)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S3: ASWR (KEY)
	{     4,    4,    4,    4,    5, ESWR,	  4,    4},	// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S5: ASNR (SL)
	{     6,    6,    6,    6,    6, ESWR,	  7,    6},	// S6: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S7: ASNR (COM)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S8: ASNR (ES)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS}  // S9: ASWR (ER)

};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static int stateType[NUM_STATES] = {
	NOFS, /* S0: Start or other non-final states */
	NOFS, /* S1: ... */
	FSNR, /* S2: Final State, Method names (MID) - No retract */
	FSNR, /* S3: Final State, Keywords (KEY) - No retract */
	NOFS, /* S4: ... */
	FSNR, /* S5: Final State, String literals (SL) - No retract */
	NOFS, /* S6: ... */
	FSNR, /* S7: Final State, Comments (COM) - No retract */
	FSNR, /* S8: Final State, Error state 1 - No retract */
	FSWR  /* S9: Final State, Error state 2 - With retract */
	// Continue with the same logic for all states in your FSM
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
Cast_void			printScannerData(ScannerData scData);
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
Token funcID	(str lexeme);
Token funcFL    (str lexeme);
Token funcCMT   (str lexeme);
Token funcKEY	(str lexeme);
Token funcErr	(str lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */

#define KWT_SIZE 16

static str keywordTable[KWT_SIZE] = {
	"def",      // KW00 - Defines a function
	"class",    // KW01 - Defines a class
	"if",       // KW02 - Conditional statement
	"else",     // KW03 - Alternative branch in conditional statements
	"elif",     // KW04 - Else if branch in conditional statements
	"for",      // KW05 - For loop
	"while",    // KW06 - While loop
	"return",   // KW07 - Returns a value from a function
	"import",   // KW08 - Imports a module
	"in",       // KW09 - Membership test operations
	"break",    // KW10 - Breaks out of the current loop
	"continue", // KW11 - Continues to the next iteration of the loop
	"pass",     // KW12 - Null operation, nothing happens when it executes
	"True",     // KW13 - Boolean True
	"False",    // KW14 - Boolean False
	"None",     // KW15 - Represents the absence of a value or a null value
};




/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */;
typedef struct languageAttributes {
	Cast_char indentationCharType;
	int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
int numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
