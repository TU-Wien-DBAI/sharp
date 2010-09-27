
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include <getopt.h>

#include "support/support.h"
#include "Global.h"

#include "input/DIMACSHypergraph.h"
#define YY_DatalogParser_LTYPE yyltype /* otherwise double definition */ 

#include "DatalogProblem.h"
#include "ArgumentationProblem.h"

#include "sharp/AnswerSet.h"
#include "sharp/HeadCycleFreeAnswerSet.h"
#include "htree/ExtendedHypertree.h"
#include "htree/H_BucketElim.h"

using namespace std;

static bool bOpt = false;
static Timer t;

enum Algorithm
{
	SAT,
	MinSAT,
	ASP,
	HCFASP,
	AF
};

enum OutputType
{
	Consistency,
	Counting,
	Enumeration
};

static void usage();
static Hypertree *decompose(Hypergraph *);
static void printTime(pair<double, double>);
static void printSolution(SolutionContent *, OutputType, Problem *);

#ifdef DEBUG
static void printNameMap(NameMap &);
#endif

int main(int argc, char **argv)
{
	InitializeErrorHandling(argv[0]);

	/* Argument Handling */

	int arg, argCount = 1;
	char *sEnd = NULL;
	bool sOpt = false, fOpt = false, tOpt = false, oOpt = false, aOpt = false;
	unsigned int seed = (unsigned int)time(NULL);
	Algorithm algorithm = ASP;
	OutputType output = Enumeration;
	char *filename = NULL;
	ifstream file;
	istream *stream = &std::cin;

	while((arg = getopt(argc, argv, "a:bs:o:f:t")) != EOF)
	{
		++argCount;
		switch(arg)
		{
		case 'a':
			if(aOpt || !optarg) usage();
			if(!strcmp(optarg, "sat")) algorithm = SAT;
			else if(!strcmp(optarg, "minsat")) algorithm = MinSAT;
			else if(!strcmp(optarg, "asp")) algorithm = ASP;
			else if(!strcmp(optarg, "hcfasp")) algorithm = HCFASP;
			else if(!strcmp(optarg, "af")) algorithm = AF;
			else usage();
			++argCount;
			aOpt = true;
			break;
		case 's':
			if(sOpt || !optarg) usage();
			seed = (unsigned int)strtol(optarg, &sEnd, 0);
			if(*sEnd) usage();
			sOpt = true;
			++argCount;
			break;
		case 't':
			if(tOpt) usage();
			tOpt = true;
			break;
		case 'o':
			if(oOpt || !optarg) usage();
			if(!strcmp(optarg, "enum")) output = Enumeration;
			else if(!strcmp(optarg, "count")) output = Counting;
			else if(!strcmp(optarg, "yesno")) output = Consistency;
			else usage();
			++argCount;
			oOpt = true;
			break;
		case 'b':
			if(bOpt) usage();
			bOpt = true;
			break;
		case 'f':
			if(fOpt || !optarg) usage();	
			filename = optarg;
			fOpt = true;
			++argCount;
			break;
		case '?':
			usage();
			break;
		default:
			C0(0);
			break;
		}
	}

	if(argCount != argc) usage();

	if(fOpt)
	{
		file.open(filename, ios::in);
		if(!file.is_open()) { cerr << sProgramName << ": Could not find file '" << filename << "'!" << endl; exit(EXIT_FAILURE); }
		stream = &file;
	}

	/* End Argument Handling */

	srand(seed);

	if(bOpt) cout << "Using seed: " << seed << endl;

	/* Algorithm Selection */

	Problem *problem = NULL;
	Instantiator *inst = NULL;

	switch(output)
	{
	case Consistency:
		inst = new GenericInstantiator<ConsistencySolutionContent>(false);
		break;
	case Counting:
		inst = new GenericInstantiator<CountingSolutionContent>(false);
		break;
	case Enumeration:
		inst = new GenericInstantiator<EnumerationSolutionContent>(true);
		break;
	default:
		C0(0 /*ERROR: Invalid output method*/);
		break;
	}

	switch(algorithm)
	{
	case SAT:
		PrintError("The SAT algorithm has not yet been ported and is not available ATM!");
		break;
	case MinSAT:
		PrintError("The MinSAT algorithm has not yet been ported and is not available ATM!");
		break;
	case ASP:
		problem = new DatalogProblem(stream, false);
		break;
	case HCFASP:
		problem = new DatalogProblem(stream, true);
		break;
	case AF:
		problem = new ArgumentationProblem(stream);
		break;
	default:
		C0(0 /*ERROR: Invalid algorithm selection*/);
		break;
	}

	if(bOpt) { cout << "Calculating... " << endl; t.start(); }

	Solution *solution = problem->calculateSolution(inst);

	if(bOpt) { cout << "\tevaluation took " << flush; printTime(t.stop()); cout << " seconds..." << endl; }
	
	SolutionContent *sc = solution->getContent();

	if(bOpt) { cout << "done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	//TODO: Benchmarking as before and output
	printSolution(sc, output, problem);

	exit(EXIT_SUCCESS);
}

static void usage()
{
	cout 	<< "Usage: " 
		<< sProgramName 
		<< " [-b] [-t] [-s <seed>] [-f <file>] [-a <alg>] [-o <output>]" << endl 
		<< "\t-b\t\tprint benchmark information" << endl
		<< "\t-t\t\tperform only tree decomposition step" << endl
		<< "\t-s seed\t\tinitialize random number generator with <seed>." << endl
		<< "\t-f file\t\tthe file to read from" << endl
		<< "\t-a alg\t\talgorithm, one of {sat, minsat, asp (default), hcfasp}" << endl
		<< "\t-o output\toutput type, one of {enum (default), count, yesno}" << endl
		;

	exit(EXIT_FAILURE);
}

static Hypertree *decompose(Hypergraph *hg)
{
        Timer local;
        Hypertree *ht;
        H_BucketElim be;

        ht = be.buildHypertree(hg, BE_MIW_ORDER);

        ht->swapChiLambda();
        ht->shrink(true);
        ht->swapChiLambda();

        return ht;
}

static void printSolution(SolutionContent *sc, OutputType output, Problem *p)
{
	if(!sc) return;

	if(output == Enumeration)
	{
		EnumerationSolutionContent *sol = (EnumerationSolutionContent *)sc;
	
		cout << "Solutions: " << sol->enumerations.size() << endl;
		cout << "{";
		string osep = "";
		for(set<set<int> >::iterator oit = sol->enumerations.begin(); oit != sol->enumerations.end(); ++oit)
		{
			cout << osep << "{";
			string isep = "";
			for(set<int>::iterator iit = oit->begin(); iit != oit->end(); ++iit)
			{
				cout << isep << p->getVertexName(*iit);
				isep = ",";
			}
			cout << "}" << flush;
			osep = ",";
		}
		cout << "}" << endl;
	}
	else if(output == Counting)
	{
		CountingSolutionContent *sol = (CountingSolutionContent *)sc;
		cout << "Solutions: " << sol->count << endl;
	}
	else if(output == Consistency)
	{
		ConsistencySolutionContent *sol = (ConsistencySolutionContent *)sc;
		cout << (sol->consistent ? "" : "NOT ") << "CONSISTENT" << endl;
	}
}

static void printTime(pair<double, double> time)
{
	cout << time.first;
}

#ifdef DEBUG
static void printNameMap(NameMap &eht)
{
	for(unsigned int i = 0; i < eht.size(); ++i)
		cout << "(" << i << "=" << eht[i] << ")";
	cout << endl;
}
#endif

