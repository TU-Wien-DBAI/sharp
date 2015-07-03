/*
 ============================================================================
 Name        : support.hpp
 Author      : Michael Morak
 Version     : 1.0
 Description : Error handling routines and macro definitions. Thanks to
               Martin Elshuber (e9825286@student.tuwien.ac.at) for the
               basic idea and some code.
 Date        : 2007-11-07
 ============================================================================
 */
#ifndef SHARP_SUPPORT_H_
#define SHARP_SUPPORT_H_

#include <sys/resource.h>

#ifndef EOF
// Define EOF (End Of File) as -1
#define EOF (-1)
#endif

namespace sharp
{
	/*
	 ============================================================================
	 Variable    : sProgramName
	 Description : A string that stores the program name (argv[0])
	 Date        : 2007-11-07
	 ============================================================================
	 */
	extern char sProgramName[];
	
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
	    #define PrintError(MESSAGE, CODE) _printError(__LINE__, __FILE__, (MESSAGE), (CODE))
	#else
	    #define PrintError(MESSAGE, CODE) _printError((MESSAGE), (CODE))
	#endif

	void _printError(int line, const char *file, const char *message, const char *code);
	void _printError(const char *message, const char *code);
	    
	/*
	 ============================================================================
	 Error handling macros
	 ============================================================================
	 */
	#define CHECK(CMD, COND, MSG) 	{ if((CMD) COND) PrintError(MSG, #CMD); }
	#define CHECK0(CMD, MSG)       		CHECK(CMD, == 0, MSG)
	#define CHECKNULL(CMD, MSG)    		CHECK(CMD, == NULL, MSG)
	#define CHECKNEG(CMD, MSG)     		CHECK(CMD, < 0, MSG)
	#define CHECKPOS(CMD, MSG)     		CHECK(CMD, > 0, MSG)
	#define CHECKNOT0(CMD, MSG)    		CHECK(CMD, != 0, MSG)
	#define CHECKNOTNULL(CMD, MSG) 		CHECK(CMD, != NULL, MSG)
	#define CHECKEOF(CMD, MSG)  		CHECK(CMD, == EOF, MSG)
	
} // namespace sharp
    
#endif /*SHARP_SUPPORT_H_*/
