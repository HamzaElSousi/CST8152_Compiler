/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2024
* Author: Hamza El Sousi && Mohammad Alashi
* Professors: Paulo Sousa
* Student Names: Hamza El Sousi && Mohammad Alashi
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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(int size, int increment, int mode) {
	BufferPointer readerPointer;
	/* Defensive programming */

	if (mode != 'f' && mode != 'a' && mode != 'm') {
		printf("Invalid mode entered in readerCreate\n");
		return NULL;  // Invalid parameters
	}

	if (size <= 0)
	{
		size = READER_DEFAULT_SIZE;
	}
	if (increment <= 0)
	{
		increment = READER_DEFAULT_INCREMENT;
	}

	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer)
	{
		printf("calloc of readerPointer in readerCreate failed\n");
		return NULL;
	}

	readerPointer->content = (str)malloc(size);
	if (!readerPointer->content) {
		free(readerPointer);
		printf("calloc of readerPointer->content in readerCreate failed\n");
		return NULL;
	}

	/* Defensive programming */
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}

	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;

	/* Initialize flags */
	readerPointer->flags = READER_DEFAULT_FLAG;  // Assuming default flag is 0

	/* Cleaning the content */
	if (readerPointer->content)
		readerPointer->content[0] = READER_TERMINATOR;

	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, char ch) {
	str tempReader = NULL;
	int newSize = 0;
	/* TO_DO done: Defensive programming */
	if (!readerPointer) {
		printf("Invalid readerPointer in readerAddChar\n");
		return NULL;  // Invalid reader
	}

	if ((ch < 0) || (ch > 128))//invalid char
	{
		printf("Invalid ch in readerAddChar\n");
		return NULL;
	}


	if (readerPointer->position.wrte * (int)sizeof(char) < readerPointer->size) {
		/* Buffer is NOT full */
	}
	else {
		/* Reset Full flag */
		readerPointer->flags &= ~READER_FULL_FLAG;

		switch (readerPointer->mode) {
		case MODE_FIXED:
			printf("Full buffer in fixed mode in readerAddChar\n");
			return NULL;
		case MODE_ADDIT:
			/* Adjust new size */
			newSize = readerPointer->size + readerPointer->increment;
			/* TO_DO: Defensive programming */

			if ((newSize < 0) || (newSize > READER_MAX_SIZE) || (newSize <= readerPointer->size))
			{
				printf("Invalid newSize in additive mode of readerAddChar\n");
				return NULL;
			}
			break;
		case MODE_MULTI:
			/* Adjust new size */
			newSize = readerPointer->size * readerPointer->increment;
			/* TO_DO: Defensive programming */
			if ((newSize < 0) || (newSize > READER_MAX_SIZE) || (newSize <= readerPointer->size))
			{
				printf("Invalid newSize in multi mode of readerAddChar\n");
				return NULL;
			}
			break;
		default:
			printf("default case in switch case of readerAddChar\n");
			return NULL;
		}

		/* New reader allocation */
		tempReader = (str)realloc(readerPointer->content, newSize * sizeof(char));
		/* TO_DO done: Defensive programming */
		if (!tempReader) {
			printf("memory allocation for newSize failed in readerAddChar\n");
			return NULL;  // Memory allocation failed
		}

		/* Check Relocation */
		if (tempReader != readerPointer->content) {
			readerPointer->flags |= READER_RELOC_FLAG;  // Set relocation flag
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}

	/* Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;

	/* Updates histogram */
	if (ch >= 0 && ch < NCHAR) {
		readerPointer->histogram[ch]++;
	}

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		printf("Invalid readerPointer in readerClear\n");
		/* TO_DO done: Handle NULL pointer */
		return False;
	}

	/* TO_DO done: Adjust flags original */
	readerPointer->position.wrte = readerPointer->position.mark = readerPointer->position.read = 0;

	return True;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerFree(BufferPointer const readerPointer) {
	/* TO_DO done: Defensive programming */
	if (readerPointer == NULL) {
		printf("Invalid readerPointer in readerFree\n");
		/* TO_DO done: Handle NULL pointer */
		return False;
	}

	/* TO_DO done: Free pointers */
	free(readerPointer->content);
	free(readerPointer);

	return True;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO done: Defensive programming */
	if (readerPointer == NULL) {
		printf("Invalid readerPointer in readerIsFull\n");
		/* TO_DO done: Handle NULL pointer */
		return False;
	}

	/* TO_DO done: Check flag if buffer is FUL */
	return (readerPointer->flags & READER_FULL_FLAG) != 0;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO done: Defensive programming */
	if (readerPointer == NULL) {
		printf("Invalid readerPointer in readerIsEmpty\n");
		/* TO_DO done: Handle NULL pointer */
		return True;  /* Assuming a NULL pointer is considered an empty buffer */
	}

	/* TO_DO done: Check flag if buffer is EMP */
	return (readerPointer->flags & READER_EMPTY_FLAG) != 0;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerSetMark(BufferPointer const readerPointer, int mark) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Adjust mark */
	readerPointer->position.mark = mark;
	return True;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerPrint(BufferPointer const readerPointer) {
	int cont = 0;
	char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	while (cont < readerPointer->position.wrte) {
		/* TO_DO: Check flag if buffer EOB has achieved */
		if (readerPointer->position.read < 0 || readerPointer->position.read >= readerPointer->position.wrte) {
			/* TO_DO: Handle invalid positions */
			printf("Invalid char in readerPrint at position %d\n", cont);
			return READER_ERROR;
		}

		c = readerGetChar(readerPointer);
		cont++;
		printf("%c", c);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	int size = 0;
	char c;

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		printf("Invalid readerPointer in readerLoad\n");
		/* TO_DO done: Handle NULL pointers */
		return READER_ERROR;
	}

	if (fileDescriptor == NULL)
	{
		printf("Invalid fileDescriptor in readerLoad\n");
		return READER_ERROR;
	}
	c = (char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			/* TO_DO done: Handle readerAddChar error */
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}

	/* TO_DO: Defensive programming */
	if (ferror(fileDescriptor)) {
		/* TO_DO done: Handle file read error */
		return READER_ERROR;
	}

	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerRecover(BufferPointer const readerPointer) {
	/* TO_DO done: Defensive programming */
	if (readerPointer == NULL) {
		printf("Invalid readerPointer in readerRecover\n");
		/* TO_DO done: Handle NULL pointer */
		return False;
	}

	/* TO_DO: Check boundary conditions */
	if (readerPointer->position.read < 0 || readerPointer->position.read > readerPointer->position.wrte) {
		printf("Invalid readerPointer->position.read in readerRecover\n");
		/* TO_DO done: Handle invalid positions */
		return False;
	}

	/* TO_DO done: Recover positions */
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	return True;
}

/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerRetract\n");
		return False;
	}
	/* TO_DO done: Defensive programming */
	/* TO_DO: Retract (return 1 pos read) */
	if (readerPointer->position.read > 0)
	{
		readerPointer->position.read--;
		return True;

	}
	else//read is at 0 or negative and cannot retract
	{
		printf("Cannot retract read in readerRetract. Read is at: %d\n", readerPointer->position.read);
		return False;
	}

}

/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_boln readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerRestore\n");
		return False;
	}
	/* TO_DO done: Defensive programming */
	/* TO_DO: Restore positions (read/mark) */
	readerPointer->position.read = readerPointer->position.mark;
	return True;
}

/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_char readerGetChar(BufferPointer const readerPointer) {
	if(!readerPointer)
	{
		printf("Invalid readerPointer in readerGetChar\n");
		return CHARSEOF;//This was defined in Reader.h as -1 (Andrew)
	}
	/* TO_DO done: Defensive programming */
	/* TO_DO: Check condition to read/wrte */
	/* TO_DO: Set EOB flag */
	/* TO_DO: Reset EOB flag */
	if (readerPointer->position.wrte>0)
		return readerPointer->content[readerPointer->position.read++];
	return READER_TERMINATOR;
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
str readerGetContent(BufferPointer const readerPointer, int pos) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetContent\n");
		return NULL;
	}
	if ((pos < 0) || (pos > readerPointer->position.wrte))
	{
		printf("Invalid pos in readerGetContent\n");
		return NULL;
	}
	/* TO_DO done: Defensive programming */
	/* TO_DO done: Return content (string) */
	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetPosRead\n");
		return CHARSEOF;
	}
	/* TO_DO done: Defensive programming */
	/* TO_DO done: Return read */
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetPosWrte\n");
		return CHARSEOF;
	}/* TO_DO done: Defensive programming */
	/* TO_DO done: Return wrte */
	return readerPointer->position.wrte;

}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetPosMark\n");
		return CHARSEOF;
	}
	/* TO_DO done: Defensive programming */
	/* TO_DO done: Return mark */
	return readerPointer->position.mark;

	return 0;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetSize\n");
		return CHARSEOF;
	}/* TO_DO done: Defensive programming */
	/* TO_DO done: Return size */
	return readerPointer->size;

}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetInc(BufferPointer const readerPointer) {

	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetInc\n");
		return CHARSEOF;
	}/* TO_DO done: Defensive programming */
	/* TO_DO done: Return increment */
	return readerPointer->increment;

	return 0;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetMode(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetMode\n");
		return CHARSEOF;
	}/* TO_DO done: Defensive programming */
	/* TO_DO done: Return mode */
	return readerPointer->mode;

	return 0;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_byte readerGetFlags(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerGetFlags\n");
		return CHARSEOF;
	}/* TO_DO done: Defensive programming */

	/* TO_DO done: Return flags */
	return readerPointer->flags;
	return 0;
}

/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
Cast_void readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerPrintStat\n");
	}
	else
	{
		for (int i = 0; i < NCHAR; i++)
		{
			if (readerPointer->histogram[i] > 0)
			{
				printf("Statistics[%c]=%d\n", i, readerPointer->histogram[i]);
			}
		}
	}
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer)
	{
		printf("Invalid readerPointer in readerNumErrors\n");
		return CHARSEOF;
	}
	/* TO_DO done: Defensive programming */
	printf("Number of errors in the buffer: %d", readerPointer->numReaderErrors);
	/* TO_DO done: Returns the number of errors */
	return 0;
}