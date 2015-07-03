/*
 ============================================================================
 Name        : support.cpp
 Author      : Michael Morak
 Version     : 1.0
 Description : Error handling routines.
 Date        : 2007-11-07
 ============================================================================
 */
#include "errorhandling.hpp"

#include <config.h>

#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

#define MAX_LENGTH 4096

namespace 
{
	/*
	 ============================================================================
	 Variable    : g_programName
	 Description : A string that stores the program name (argv[0])
	 Date        : 2007-11-07
	 ============================================================================
	 */
	char g_programName[MAX_LENGTH];
}

namespace sharp
{
	/*
	 ============================================================================
	 Function    : InitializeErrorHandling
	 Description : Initializes the variables needed for the error handling
	               routines defined here.
	 Date        : 2007-11-07
	 Globals     : g_programName
	 ============================================================================
	 */
	void InitializeErrorHandling(const char programName[])
	{
	    if(programName == NULL)
	    {
	        strncpy(g_programName, "Error", MAX_LENGTH);
	    }
	    else
	    {
	        strncpy(g_programName, programName, MAX_LENGTH);
	        g_programName[MAX_LENGTH - 1] = '\0';
	    }
	}
	
	/*
	 ============================================================================
	 Function    : PrintError
	 Description : Prints the given error message to the stderr stream and exits
	               the program with return code EXIT_FAILURE after calling 
	               the function stored in the freeResourcesFunction pointer.
	 Date        : 2007-11-07
	 Globals     : g_programName
	 ============================================================================
	 */
#ifdef DEBUG
	void _printError(int line, const char *file, const char *message, const char *code)
	{
		using std::endl;
		using std::cerr;	

	    if(errno == 0)
	    	cerr << g_programName << ": " << file << ":" << line 
			<< ": ERROR - " << message << " (code: " << code << ")" << endl;
	    else
	    	cerr << g_programName << ": " << file << ":" << line 
			<< ": ERROR - " << message << " (code: " << code << ")" 
			<< " - " << strerror(errno) << endl;
	    exit(EXIT_FAILURE);
	}
#endif // DEBUG

	void _printError(const char *message, const char *code)
	{
		using std::endl;
		using std::cerr;	

	    if(errno == 0)
	    	cerr << g_programName << ": ERROR - " << message << " (code: " << code << ")" << endl;
	    else
	    	cerr << g_programName << ": ERROR - " << message << " (code: " << code << ")" 
			<< " - " << strerror(errno) << endl; 
	    exit(EXIT_FAILURE);
	}

} // namespace sharp
