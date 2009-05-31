/*
 ============================================================================
 Name        : support.c
 Author      : Michael Morak
 Version     : 1.0
 Description : Error handling routines.
 Date        : 2007-11-07
 ============================================================================
 */
#include <errno.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

#include "support.h"

#define MAX_LENGTH 4096

/*
 ============================================================================
 Variable    : sProgramName
 Description : A string that stores the program name (argv[0])
 Date        : 2007-11-07
 ============================================================================
 */
char sProgramName[MAX_LENGTH];

/*
 ============================================================================
 Function    : InitializeErrorHandling
 Description : Initializes the variables needed for the error handling
               routines defined here.
 Date        : 2007-11-07
 Globals     : sProgramName
 ============================================================================
 */
void InitializeErrorHandling(const char *programName)
{
    if(programName == NULL)
    {
        strncpy(sProgramName, "Error", MAX_LENGTH);
    }
    else
    {
        strncpy(sProgramName, programName, MAX_LENGTH);
        sProgramName[MAX_LENGTH-1] = '\0';
    }
}

/*
 ============================================================================
 Function    : PrintError
 Description : Prints the given error message to the stderr stream and exits
               the program with return code EXIT_FAILURE after calling 
               the function stored in the freeResourcesFunction pointer.
 Date        : 2007-11-07
 Globals     : sProgramName
 ============================================================================
 */
#ifdef DEBUG
void _printError(int line, char *file, const char *message)
{
    if(errno == 0)
    	cerr << sProgramName << ": " << file << ":" << line << " - " << message << endl;
    else
    	cerr << sProgramName << ": " << file << ":" << line << " - " << message << " " << strerror(errno) << endl;
#else
void _printError(const char *message)
{
    if(errno == 0)
    	cerr << sProgramName << ": " << message << endl;
    else
    	cerr << sProgramName << ": " << message << " " << strerror(errno) << endl; 
#endif
    exit(EXIT_FAILURE);
}
