/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Hamza El Sousi
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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
int line;								/* Current line number of the source code */
extern int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern int stateType[NUM_STATES];
extern str keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */           ///////*ok*/////////

int startScanner(BufferPointer psc_buf) {
	if (psc_buf == NULL) {
		fprintf(stderr, "Error: Null pointer passed to startScanner\n");
		return EXIT_FAILURE;
	}

	// Initialize histogram
	for (int i = 0; i < NUM_TOKENS; i++)
		scData.scanHistogram[i] = 0;

	// Basic scanner initialization
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;

	return EXIT_SUCCESS; // 0 indicates success
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(Cast_void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	Cast_char c;	/* input symbol */
	int state = 0;		/* initial state of the FSM */
	int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	int lexLength;		/* token length */
	int i;				/* counter */
	/*
	Cast_char newc;			// new char
	*/

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

		/* Cases for spaces */
		case ' ':
		case '\t':
			// Adjusted comment to reflect the actual token being generated
			// Spaces and tabs typically don't directly generate INDENT tokens unless at the start of a line
			// and contributing to a change in indentation level. However, sticking to your constraints:
			currentToken.code = INDENT_T; // Consider adjusting this logic to more accurately reflect indentation handling
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
			
		case '\n':
			line++;  // Increment line counter correctly for newlines
			// NEWLINE_T is correct for representing the end of a line/statement in many contexts
			currentToken.code = NEWLINE_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		/* Cases for symbols */

		
		/* Cases for symbols */

		case '#':
			currentToken.code = CMT_T; // Comment token
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case '(':
			currentToken.code = LPR_T; // Left Parenthesis token
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case ')':
			currentToken.code = RPR_T; // Right Parenthesis token
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case '{':
			currentToken.code = LBR_T; // Left Brace token
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case '}':
			currentToken.code = RBR_T; // Right Brace token
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		
		
		case '.':
			currentToken.code = PERIOD_T; // Period token, often used for object attributes or float literals
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		
		 
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
			}
			if (stateType[state] == FSWR) {
				readerRetract(sourceBuffer); // Retract if the state requires it
			}
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart; // Calculate the lexeme length

			// Create a buffer for the lexeme
			lexemeBuffer = readerCreate(lexLength + 1, 0, MODE_FIXED); // Adding 1 for the terminator
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Cannot create buffer\n");
				exit(EXIT_FAILURE); // Use a standardized exit code
			}

			readerRestore(sourceBuffer); // Restore buffer to the start of the lexeme
			for (int i = 0; i < lexLength; i++) {
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer)); // Copy lexeme
			}
			readerAddChar(lexemeBuffer, '\0'); // Assuming READER_TERMINATOR is '\0'

			// Create the final token using the function corresponding to the final state
			currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
			readerFree(lexemeBuffer); // Assuming a function to free the buffer
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */        /*  DONE */

int nextState(int state, Cast_char c) {
	int col;
	int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */


	   /* Adjust the logic to return the next column in TT */
	   /*    [A-z],[0-9],    _,    B10=(,   Q(',    B1C=),    X, N, P */

int nextClass(char c) {
	int val = -1;

	if (isalpha(c)) { // [A-z]
		val = 0;
	}
	else if (isdigit(c)) { // [0-9]
		val = 1;
	}
	else {
		switch (c) {
		case CHRCOL5: // '_'
			val = 2;
			break;
		case CHRCOL2: // '('
			val = 3;
			break;
		case CHRCOL4: // '\''
			val = 4;
			break;
		case CHRCOL3: // ')'
			val = 5;
			break;
		case CHARSEOF0:
		case CHARSEOF255:
			val = 5; // End of File; assuming the same column as ')'
			break;
		case CHRCOL6: // '#'
			val = 7; // Assuming column 7 for comments
			break;
		case CHRCOL13: // '.'
			val = 7; // Assuming column 7 for periods
			break;
		case CHRCOL7: // '['
		case CHRCOL8: // ']'
		case CHRCOL9: // '{'
		case CHRCOL10: // '}'
		case CHRCOL11: // ':'
		case CHRCOL12: // ','
			val = 8; // Assuming other characters are in column 8
			break;
		default:
			val = 8; // All other characters
		}
	}
	return val;
}



/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
                                                           /* H(^N)*N*/

Token funcCMT(str lexeme) {
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

 /*
************************************************************
* Acceptance State Function IL
*    Function responsible for identifying IL (integer literals).
* - It is necessary to respect the limit (e.g., the range of 'short' in C).
* - In the case of lexemes representing larger values, an error should be returned.
* - Only the first ERR_LEN characters are accepted, and if necessary,
*   additional three dots (...) should be appended.
************************************************************
*/

/* TO_DO: Adjust the function for IL */                         /*D+*/
Token funcIL(str lexeme) {
	Token currentToken = { 0 };
	long tlong; // Using long to accommodate larger-than-short values
	char* endptr; // Pointer to character following the parsed integer

	// strtol allows to check if we consumed the entire string and also performs range checking
	tlong = strtol(lexeme, &endptr, 10);

	// Check if we consumed the entire string and didn't go out of range
	if (*endptr == '\0' && lexeme[0] != '\0') {
		if (tlong <= SHRT_MAX && tlong >= SHRT_MIN) {
			currentToken.code = IN_T; // Token code for integer literals
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (short)tlong;
		}
		else {
			// Handle integer overflow by capturing only the first ERR_LEN characters
			strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN);
			currentToken.attribute.errLexeme[ERR_LEN - 1] = '\0'; // Ensuring null-termination
			currentToken.code = ERR_T; // Token code for errors
		}
	}
	else {
		// If the string was not consumed completely, then it's an error (e.g., "123abc")
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN);
		currentToken.attribute.errLexeme[ERR_LEN - 1] = '\0'; // Ensuring null-termination
		currentToken.code = ERR_T; // Token code for errors
	}
	return currentToken;
}


/*____________________________________________________________________________*/
/*
 ************************************************************
 * Acceptance State Function ID
 *    Function responsible for identifying method names and keywords.
 *    This function checks if the lexeme represents a method (with a specific suffix) or a keyword.
 *    Keywords are checked after removing the potential method name suffix.
 *    If the lexeme is neither a method nor a keyword, it's processed as an error (assuming all valid identifiers are keywords or method names).
 ***********************************************************
 */
Token funcID(str lexeme) {
	Token currentToken = { 0 }; // Initialize the current token with default values.
	size_t length = strlen(lexeme); // Get the length of the lexeme.

	// Check if the lexeme potentially represents a method name with the specific suffix.
	if (length > 0 && lexeme[length - 1] == MNID_SUF) {
		// Temporarily remove the method name suffix to check if the remaining part matches a keyword.
		lexeme[length - 1] = '\0';
		currentToken = funcKEY(lexeme); // Check if the lexeme matches any keywords.
		lexeme[length - 1] = MNID_SUF; // Restore the method name suffix.

		// If the lexeme doesn't match a keyword, it's treated as a method name.
		if (currentToken.code != KW_T) {
			currentToken.code = MNID_T; // Set the token type to method name identifier.
			// Ensure the lexeme is not longer than VID_LEN
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
			currentToken.attribute.idLexeme[VID_LEN] = '\0'; // Ensure the lexeme is null-terminated.
		}
		// If it was a keyword, funcKEY has already set the correct token attributes.
	}
	else {
		// If the lexeme doesn't end with the method name suffix, treat it as a potential keyword.
		currentToken = funcKEY(lexeme);

		// If funcKEY couldn't match it to a keyword, treat it as an error.
		if (currentToken.code != KW_T) {
			currentToken.code = ERR_T; // Assuming ERR_T is the token code for errors.
			strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN);
			currentToken.attribute.errLexeme[ERR_LEN] = '\0'; // Ensure the error lexeme is null-terminated.
		}
	}

	// Update the histogram for method names and errors.
	if (currentToken.code == MNID_T || currentToken.code == ERR_T) {
		scData.scanHistogram[currentToken.code]++;
	}

	return currentToken;
}






/*___________________________________________________________________________________*/

/*
 ***********************************************************
 * Acceptance State Function FL
 * Function responsible to identify FL (floating-point literals).
 * Pattern: D+PD* | D*PD+
 * D+PD* - one or more digits, followed by a period, followed by zero or more digits
 * D*PD+ - zero or more digits, followed by a period, followed by one or more digits
 ***********************************************************
 */
Token funcFL(str lexeme) {
	Token currentToken = { 0 };  // Initialize token.
	currentToken.code = FL_T;  // Assuming FL_T is the token code for floating literals.

	char* endptr;
	double value = strtod(lexeme, &endptr);

	// Check if the lexeme is a proper float literal after conversion
	// Ensure the whole lexeme was a valid float literal and not just a part of it
	if (*endptr == '\0' && endptr != lexeme) {
		currentToken.attribute.floatValue = value;  // Store the converted float value
	}
	else {
		// If the lexeme is not a valid float literal, handle it as an error
		currentToken.code = ERR_T;  // Assuming ERR_T is the token code for errors.
		// Capture only the first ERR_LEN characters of the lexeme to report the error
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 1);
		currentToken.attribute.errLexeme[ERR_LEN - 1] = '\0'; // Ensure null-termination
		if (strlen(lexeme) > ERR_LEN - 1) {
			// If the lexeme is longer than ERR_LEN - 1, append "..." to indicate truncation
			strcat(currentToken.attribute.errLexeme, "...");
		}
	}

	return currentToken;  // Return the token with its set attributes
}


/*________________________________________________________________________________________*/

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */              /* Q(H|N|L|D|P)*Q */

Token funcSL(str lexeme) {
	Token currentToken = { 0 };
	int len = (int)strlen(lexeme);

	// Assume the lexeme includes the quotation marks, so start and end inside them
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable); // Get current write position

	for (int i = 1; i < len - 1; i++) { // Skip the initial and final quotation marks
		if (lexeme[i] == '\n') {
			line++; // Increment line counter for each newline character within the string
		}
		if (!readerAddChar(stringLiteralTable, lexeme[i])) { // Attempt to add each character
			// On failure to add a character, set the token to a run-time error
			currentToken.code = RTE_T; // Assuming RTE_T is the token code for runtime errors
			scData.scanHistogram[currentToken.code]++;
			strncpy(currentToken.attribute.errLexeme, "Run Time Error:", ERR_LEN);
			currentToken.attribute.errLexeme[ERR_LEN] = '\0'; // Ensure null-termination
			errorNumber = RTE_CODE; // Assuming RTE_CODE represents the specific error number for this scenario
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, '\0')) { // Ensure separation of lexemes with \0
		// Handle potential error for adding separator
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strncpy(currentToken.attribute.errLexeme, "Run Time Error:", ERR_LEN);
		currentToken.attribute.errLexeme[ERR_LEN] = '\0'; // Ensure null-termination
		errorNumber = RTE_CODE;
		return currentToken;
	}

	// On successful addition of the lexeme (excluding the quotation marks) to the table
	currentToken.code = STRL_T; // Assuming STRL_T is the token code for string literals
	scData.scanHistogram[currentToken.code]++;

	return currentToken; // Return the token with its set attributes
}


/*____________________________________________________________________________________________*/

/*
************************************************************
* This function checks if one specific lexeme is a keyword.
* - Tip: Remember to use the keywordTable to check the keywords.
***********************************************************
*/
/* TO_DO: Adjust the function for Keywords */

Token funcKEY(str lexeme) {
	Token currentToken = { 0 };
	int kwindex = -1;

	// Search the keyword table for a matching lexeme
	for (int j = 0; j < KWT_SIZE; j++) {
		if (strcmp(lexeme, keywordTable[j]) == 0) {
			kwindex = j;
			break; // Found a matching keyword
		}
	}

	if (kwindex != -1) {
		// Lexeme matches a keyword
		currentToken.code = KW_T; // Assuming KW_T is the token code for keywords
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.keywordIndex = kwindex; // Store the index of the keyword
	}
	else {
		// Lexeme did not match any keyword
		currentToken = funcErr(lexeme); // Handle as an error or an identifier
	}

	return currentToken;
}




/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(str lexeme) {
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*_______________________________________________________________________________________________*/

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

void printToken(Token t) {
	extern str keywordTable[]; // Assuming this links to your keyword table

	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s\n", t.attribute.errLexeme);
		if (errorNumber) {
			printf("Run-time error number: %d\n", errorNumber);
			exit(errorNumber);
		}
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\n");
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STRL_T:
		// Assuming readerGetContent function returns a pointer to the string in the String Literal Table at the given offset
		printf("STRL_T\t\t\"%s\"\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case FL_T:
		printf("FL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case LPR_T:
		printf("LPR_T\t\t'('\n");
		break;
	case RPR_T:
		printf("RPR_T\t\t')'\n");
		break;
	case LBR_T:
		printf("LBR_T\t\t'{'\n");
		break;
	case RBR_T:
		printf("RBR_T\t\t'}'\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.keywordIndex]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case INDENT_T:
		printf("INDENT_T\n");
		break;
	case NEWLINE_T:
		printf("NEWLINE_T\n");
		break;
	case PERIOD_T:
		printf("PERIOD_T\t\t'.'\n");
		break;
	default:
		printf("Scanner error: Invalid token code: %d\n", t.code);
	}
}






/*___________________________________________________________________________________________________*/
/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
Cast_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/


