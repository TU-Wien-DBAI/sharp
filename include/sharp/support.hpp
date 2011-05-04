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

#include <sys/resource.h>
#include <iostream>

#ifndef EOF
// Define EOF (End Of File) as -1
#define EOF (-1)
#endif

namespace sharp
{

	struct nullstream : std::ostream
	{
	        struct nullbuf : std::streambuf
	        {
	                int overflow(int c)
	                {
	                        return traits_type::not_eof(c);
	                }
	        } m_sbuf;
	
	        nullstream() : std::ios(&m_sbuf), std::ostream(&m_sbuf) { }
	};
	
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
	#if defined(SHARP_ENABLE_DEBUG)
	    #define PrintError(MESSAGE) _printError(__LINE__, __FILE__, (MESSAGE))
	#else
	    #define PrintError(MESSAGE) _printError(MESSAGE)
	#endif

	void _printError(int line, const char *file, const char *message);
	void _printError(const char *message);
	    
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
	#define CEOF(CMD)  		CHECK(CMD, == EOF)
	
	/*
	 TIMER class
	 */
	
	class Timer
	{
	public:
	        Timer();
	        ~Timer();
	
	public:
	        void start();
	        std::pair<double, double> stop();
		void printStop(std::ostream &out = std::cout);
	
	private:
	        struct rusage beginning;
	        struct rusage end;
	};
	
	extern Timer stimer;

} // namespace sharp
    
#endif /*SUPPORT_H_*/
