/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|              ....                     |
|          ........::.::::::.           |
|        .:........::.:^^^~~~:          |
|        :^^::::::^^^::^!7??7~^.        |
|       .:^~~^!77777~~7?YY?7??7^.       |
|       :.^~!??!^::::^^~!?5PY??!~.      |
|       ~!!7J~.:::^^^^~!!~~?G5J?~       |
|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |
|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |
|       .~77..    . .~^:^^^~7?:         |
|       .^!^~:::.:^!7?~^~!77J:          |
|        ^^!Y~^^^^~?YJ77??7JJ^          |
|       .^7J?~^~~^~7??7??7JY?~:         |
|        ::^^~^7?!^~~!7???J?J7~:.       |
|         ^~~!.^7YPPPP5Y?7J7777~.       |
|        ..:~..:^!JPP5YJ?!777!^.        |
| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |
|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |
|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |
|    :~75 \__ \/ _ \  _| / _` | 5?.     |
|     7~7 |___/\___/_| |_\__,_| Y5?.    |
|    .^~!~.....................P5YY7.   |
|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |
|                                       |
=---------------------------------------=
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

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 14

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		     /*  0: Error token */
    COMMENT_T,       /* 1: Comment token (single-line, starts with #) */
	NEW_LINE_T,      /* 4: New line token (\n) to end an statement */
	MNID_T,		     /*  1: Method name identifier token (start: &) */
	INL_T,		     /*  2: Integer literal token */
	STR_T,		     /*  3: String literal token */
	FLOAT_LIT_T,     /* 8: Floating-point literal token (not directly mentioned but implied by '.') */
	LPR_T,		     /*  4: Left parenthesis token */
	RPR_T,	        /*  5: Right parenthesis token */
	LBR_T,	        /*  6: Left brace token */
	RBR_T,		    /*  7: Right brace token */
	KW_T,		    /*  8: Keyword token */
	RTE_T,		    /* 10: Run-time error token */
	SEOF_T,		    /* 11: Source end-of-file token */
	 
};

/* TO_DO: Define the list of keywords */
static str tokenStrTable[NUM_TOKENS] = {
	"if",
	"elif",
	"for",
	"while",
	"class",
	"return",
	"break",
	"continue",
	"def",
	"import"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	int codeType;                  // Integer attributes accessor, if needed
	AriOperator arithmeticOperator; // Arithmetic operator attribute code
	RelOperator relationalOperator; // Relational operator attribute code
	LogOperator logicalOperator;    // Logical operator attribute code
	EofOperator seofType;           // Source-end-of-file attribute code
	int intValue;                   // Integer literal attribute (value)
	int keywordIndex;               // Keyword index in the keyword table, if used
	int contentString;              // String literal offset from the beginning of the string literal buffer (stringLiteralTable->content), if needed
	float floatValue;               // Floating-point literal attribute (value)
	Cast_char idLexeme[VID_LEN + 1];     // Variable identifier token attribute
	Cast_char errLexeme[ERR_LEN + 1];    // Error token attribute
} TokenAttribute;


/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	Cast_byte flags;			/* Flags information */
	union {
		int intValue;				/* Integer value */
		float floatValue;			/* Float value */
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
#define CHRCOL_HASH '#'  // Single line Comments start
#define CHRCOL_UNDERSCORE '_'  // undercore 
#define CHRCOL_QUOTE '\''  // Part of string delimiters
#define CHRCOL_DOUBLEQUOTE '"'  // Part of string delimiters
#define CHRCOL_COMMA ','  // Separates items in lists, function arguments
#define CHRCOL_DOT '.'  // Decimal point in floats, object method access
#define CHRCOL_LPAREN '('  // Start of function arguments, expressions
#define CHRCOL_RPAREN ')'  // End of function arguments, expressions
#define CHRCOL_MINUS '-'  // Arithmetic subtraction, unary negation
#define CHRCOL_PLUS '+'  // Arithmetic addition
#define CHRCOL_ASTERISK '*'  // Arithmetic multiplication
#define CHRCOL_SLASH '/'  // Arithmetic division
#define CHRCOL_CARET '^'  // Exponentiation (if used in your language)
#define CHRCOL_EQUAL '='  // Assignment
#define CHRCOL_EQEQ '=='  // Equality comparison
#define CHRCOL_NEQ '!='  // Inequality comparison
#define CHRCOL_NOT '!'  // Logical NOT
#define CHRCOL_OROR '||'  // Logical OR
#define CHRCOL_ANDAND '&&'  // Logical AND
#define CHRCOL_ELLIPSIS '...'  // Range operator, variadic arguments (if applicable)


/* These constants will be used on VID / MID function */
#define MNID_SUF '&'
#define COMM_SYM '#'

/* Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */

/* State transition table definition */
#define NUM_STATES		14		/* Updated to match the number of states from your automata */
#define CHAR_CLASSES	9		/* Number of character classes */

/* TO_DO: Transition table - type of states defined in separate table */
static int transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*    [A-z], [0-9],  _,    &,     (,)    (X),    ),    N,    .    */
	/*    L(0),   D(1), U(2), B1O(3), Q(4), X(5), B1C(6), #(7), P(8) */
	{     3,      8,    ESNR, ESNR,   4 ,   ESWR, ESNR,    10,  ESNR   },  // S0
	{     1,      1,     1,    1,     1 ,    1,     1,     2,  1   },  // S1
	{    FS,     FS,    FS,   FS,     FS,   FS,    FS,    FS,   FS    },  // S2
	{     3,      3,     3,   13,     5 ,    5,    4 ,    5 ,   5    },  // S3
	{    FS,     FS,    FS,   FS,     FS,   FS,    FS,    FS,   FS    },  // S4
	{    FS,     FS,    FS,   FS,     FS,   FS,    FS,    FS,   FS    },  // S5
	{     6,      6,     6,    6,      7,    6,    6 ,     6,   6    },  // S6
	{    FS,     FS,    FS,   FS,     FS,   FS,    FS,    FS,  FS    },  // S7
	{     9,      8,     9,    9,     9 ,    9,    9 ,     9,  10   },  // S8
	{    FS,     FS,    FS,   FS,     FS,   FS,    FS,    FS,  FS   },  // S9
	{  ESNR,     11,  ESNR, ESNR,   ESNR, ESNR,  ESNR,  ESNR, ESNR  },  // S10
	{    12,     11,    12,   12,     12,   12,    12,   12,   12    },  // S11
	{    FS,     FS,    FS,   FS,     FS,   FS,    FS,   FS,   FS   },  // S12
	{    13,     13,    13,   13,     ESNR, ESNR,  ESNR, ESNR,  4   }   // S13
};


/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static int stateType[NUM_STATES] = {
	NOFS, /* S0: Initial state, not accepting */
	NOFS, /* S1: Non-accepting state */
	FSNR, /* S2: Final state for Method IDs, no retract */
	FSWR, /* S3: Final state for Keywords, with retract */
	NOFS, /* S4: Non-accepting state */
	FSNR, /* S5: Final state for String Literals, no retract */
	NOFS, /* S6: Non-accepting state */
	FSNR, /* S7: Final state for Comments, no retract */
	ESNR, /* S8: Error state with no retract */
	ESWR, /* S9: Error state with retract */
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
	funcFL,     /* FL   [06] */
	NULL,		/* -    [07] */
	funcCMT,	/* COM  [08] */
	funcErr,	/* ERR1 [09] */
	funcErr		/* ERR2 [0q0] */
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
	"data",		/* KW00 */
	"code",		/* KW01 */
	"int",		/* KW02 */
	"real",		/* KW03 */
	"string",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do"		/* KW09 */
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

#endif
