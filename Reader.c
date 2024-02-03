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

BufferPointer readerCreate(int size, int increment, char mode) {
    if (size <= 0 || increment < 0 || (mode != 'f' && mode != 'a' && mode != 'm')) {
        // Invalid parameters
        return NULL;
    }

    BufferPointer readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
    if (!readerPointer) {
        // Memory allocation failed
        return NULL;
    }

    readerPointer->content = (char*)malloc(size * sizeof(char));
    if (!readerPointer->content) {
        // Memory allocation for buffer content failed
        free(readerPointer); // Clean up previously allocated memory
        return NULL;
    }

    // Initialize buffer properties
    readerPointer->size = size;
    readerPointer->increment = increment;
    readerPointer->mode = mode;
    readerPointer->flags = BUFFER_EMPTY; // Assume BUFFER_EMPTY is a flag indicating the buffer is empty

    // The created flag must be signalized as EMPTY
    // Cleaning the content is already handled by setting the first character to READER_TERMINATOR
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
	if (!readerPointer || !readerPointer->content) return NULL; // Defensive programming

	// Check if buffer is full
	if (readerPointer->flags & FLAG_FUL) return NULL;

	// When buffer is about to be full and needs resizing
	if (readerPointer->position.wrte >= readerPointer->size - 1) {
		int newSize = 0;
		switch (readerPointer->mode) {
		case MODE_FIXED: // Cannot expand
			readerPointer->flags |= FLAG_FUL; // Set buffer as full
			return NULL;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			break;
		default: // Unsupported mode
			return NULL;
		}

		char* newContent = (char*)realloc(readerPointer->content, newSize);
		if (!newContent) {
			readerPointer->flags |= FLAG_ERROR; // Set error flag
			return NULL;
		}

		// Successfully reallocated
		readerPointer->content = newContent;
		readerPointer->size = newSize;
		readerPointer->flags &= ~FLAG_FUL; // Clear full flag
		readerPointer->flags |= FLAG_REL; // Set reallocation flag
	}

	// Add character to buffer
	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->flags &= ~FLAG_EMP; // Buffer is not empty

	// Check if buffer is now full after adding
	if (readerPointer->position.wrte == readerPointer->size) {
		readerPointer->flags |= FLAG_FUL;
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
	if (!readerPointer) return 0; // Error, return false

	readerPointer->position.wrte = 0;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	readerPointer->flags = FLAG_EMP; // Set buffer as empty

	return 1; // Success, return true
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
	if (!readerPointer) return 0; // Error, return false

	if (readerPointer->content) {
		free(readerPointer->content);
	}
	free(readerPointer);

	return 1; // Success, return true
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
	/* TO_DO: Defensive programming */
	/* TO_DO: Check flag if buffer is FUL */
	return False;
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
	/* TO_DO: Defensive programming */
	/* TO_DO: Check flag if buffer is EMP */
	return False;
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
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte) return 0; // Error, return false

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
	Cast_char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	c = readerGetChar(readerPointer);
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
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
	Cast_char c;
	/* TO_DO: Defensive programming */
	c = (Cast_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	/* TO_DO: Defensive programming */
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
	if (!readerPointer) return 0; // Error, return false

	readerPointer->position.read = 0;
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
	if (!readerPointer || readerPointer->position.read <= 0) return 0; // Error, return false

	readerPointer->position.read--;
	return True; // Success, return true
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
	if (!readerPointer || readerPointer->position.mark < 0 ||
		readerPointer->position.mark > readerPointer->position.wrte) return False; // Error, return false

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
	if (!readerPointer || readerPointer->position.read >= readerPointer->position.wrte) {
		// Either the pointer is invalid, or we've reached or surpassed the write position (end of content)
		return CHARSEOF; // Indicate end of content or error
	}

	// Return the character at the current read position and increment the read position
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
	if (!readerPointer || pos < 0 || pos >= readerPointer->position.wrte) {
		// Invalid pointer, position out of bounds
		return NULL;
	}

	// Return a pointer to the specified position in the buffer
	return readerPointer->content + pos;;
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
	/* TO_DO: Defensive programming */
	/* TO_DO: Return read */
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
	/* TO_DO: Defensive programming */
	/* TO_DO: Return wrte */
	return 0;
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
	if (!readerPointer) {
		// If the readerPointer is NULL, indicating that the buffer is not initialized,
		// return an error code or a value that clearly indicates an invalid position.
		return -1; // Using -1 as a sentinel value for error or invalid state
	}

	// Return the current mark position from the buffer's position structure.
	return readerPointer->position.mark;
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
	if (!readerPointer) {
		// If the buffer pointer is NULL, indicate an error or invalid state.
		return -1; // Using -1 as a sentinel value for error.
	}

	// Return the current size of the buffer.
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
	if (!readerPointer) {
		// If the buffer pointer is NULL, indicate an error or invalid state.
		return -1; // Using -1 as a sentinel value for error.
	}

	// Return the increment factor of the buffer.
	return readerPointer->increment;
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
	if (!readerPointer) {
		// If the buffer pointer is NULL, indicate an error or invalid state.
		return -1; // Using -1 as a sentinel value for error, assuming mode is char and -1 is distinguishable.
	}

	// Return the operational mode of the buffer.
	// Casting to int to align with the function's return type; adjust if necessary.
	return (int)readerPointer->mode;
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
	if (!readerPointer) {
		// If the buffer pointer is NULL, it's not possible to return meaningful flags.
		// Returning 0 as a default value, indicating no flags are set.
		// Adjust based on how you wish to handle this error case.
		return 0;
	}

	// Return the current flags of the buffer.
	return readerPointer->flags;
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
	if (!readerPointer) {
		printf("Buffer is NULL.\n");
		return;
	}

	printf("Buffer Statistics:\n");
	printf("Size: %d\n", readerPointer->size);
	printf("Write Position: %d\n", readerPointer->position.wrte);
	printf("Read Position: %d\n", readerPointer->position.read);
	printf("Mark Position: %d\n", readerPointer->position.mark);
	printf("Flags: %s%s%s%s\n",
		(readerPointer->flags & FLAG_FUL) ? "FULL " : "",
		(readerPointer->flags & FLAG_EMP) ? "EMPTY " : "",
		(readerPointer->flags & FLAG_REL) ? "REALLOCATED " : "",
		(readerPointer->flags & FLAG_ERROR) ? "ERROR " : "");
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
	if (!readerPointer) {
		// Consider the inability to access the buffer itself as an error condition
		return -1; // Or another sentinel value indicating the error
	}

	return readerPointer->numReaderErrors;
}
