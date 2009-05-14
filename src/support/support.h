/*
 ============================================================================
 Name        : support.h
 Author      : Michael Morak
 Version     : 1.0
 Description : Error handling routines and macro definitions. Thanks to
               Martin Elshuber (e9825286@student.tuwien.ac.at) for the
               basic idea and some code.
 Date        : 2007-11-07
 ============================================================================
 */
#ifndef SUPPORT_H_
#define SUPPORT_H_

#ifndef EOF
// Define EOF (End Of File) as -1
#define EOF (-1)
#endif

/*
 ============================================================================
 Function    : InitializeErrorHandling
 Description : Initializes the variables needed for the error handling
               routines defined here.
 Date        : 2007-11-07
 ============================================================================
 */
void InitializeErrorHandling(const char *programName);


/*
 ============================================================================
 Function    : PrintError
 Description : Prints the given error message to the stderr stream and exits
               the program with return code EXIT_FAILURE after calling 
               the function stored in the freeResourcesFunction pointer.
 Date        : 2007-11-07
 ============================================================================
 */
#ifdef DEBUG
    void _printError(int line, char *file, char *message);
    #define PrintError(MESSAGE) _printError(__LINE__, __FILE__, (MESSAGE))
#else
    void _printError(char *message);
    #define PrintError(MESSAGE) _printError(MESSAGE)
#endif
    
/*
 ============================================================================
 Error handling macros
 ============================================================================
 */
#define CHECK(CMD, COND) 	{ if((CMD) COND) PrintError(#CMD); }
#define C0(CMD)       		CHECK(CMD, == 0)
#define CNULL(CMD)    		CHECK(CMD, == NULL)
#define CNEG(CMD)     		CHECK(CMD, < 0)
#define CPOS(CMD)     		CHECK(CMD, > 0)
#define CNOT0(CMD)    		CHECK(CMD, != 0)
#define CNOTNULL(CMD) 		CHECK(CMD, != NULL)
#define CEOF(CMD)  			CHECK(CMD, == EOF)
    
#endif /*SUPPORT_H_*/
