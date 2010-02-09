
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include <getopt.h>

#include "support/support.h"
#include "Global.h"

#include "input/DIMACSHypergraph.h"
#include "input/DatalogHypergraph.h"
#include "sharp/ExtendedHypertree.h"
#include "sharp/Circumscription.h"
#include "sharp/AnswerSet.h"
#include "sharp/HeadCycleFreeAnswerSet.h"
#include "htree/H_BucketElim.h"

using namespace std;

static bool bOpt = false;
static Timer t;

enum Algorithm
{
	NoAlg,
	SAT,
	MinSAT,
	ASP,
	HCFASP
};

enum OutputType
{
	NoType,
	Consistency,
	Counting,
	Enumeration
};

static void usage();
static Hypertree *decompose(Hypergraph *);
static void printTime(pair<double, double>);
static void printSolution(SolutionContent *, OutputType, NameMap &);

#ifdef DEBUG
static void print(ExtendedHypertree *);
static void printSignMap(SignMap &);
static void printNameMap(NameMap &);
#endif

int main(int argc, char **argv)
{
	InitializeErrorHandling(argv[0]);

	/* Argument Handling */

	int arg, argCount = 1;
	char *sEnd = NULL;
	bool sOpt = false, fOpt = false, tOpt = false;
	unsigned int seed = (unsigned int)time(NULL);
	Algorithm algorithm = NoAlg;
	OutputType output = NoType;
	char *filename = NULL;
	ifstream file;
	istream *stream = &std::cin;

	if(argc < 5) usage();

	while((arg = getopt(argc, argv, "a:bs:o:f:t")) != EOF)
	{
		++argCount;
		switch(arg)
		{
		case 'a':
			if(algorithm || !optarg) usage();
			if(!strcmp(optarg, "sat")) algorithm = SAT;
			else if(!strcmp(optarg, "minsat")) algorithm = MinSAT;
			else if(!strcmp(optarg, "asp")) algorithm = ASP;
			else if(!strcmp(optarg, "hcfasp")) algorithm = HCFASP;
			else usage();
			++argCount;
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
			if(output || !optarg) usage();
			if(!strcmp(optarg, "enum")) output = Enumeration;
			else if(!strcmp(optarg, "count")) output = Counting;
			else if(!strcmp(optarg, "yesno")) output = Consistency;
			else usage();
			++argCount;
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

	/* Algorithm Selection */

	Hypertree *ht = NULL;
	ExtendedHypertree *eht = NULL;
	Instantiator *inst = NULL;
	AbstractHypergraph *hg = NULL;
	AbstractAlgorithm *alg = NULL;

	if(algorithm == SAT) hg = new DIMACSHypergraph(stream);
	else if(algorithm == MinSAT) hg = new DIMACSHypergraph(stream);
	else if(algorithm == ASP) hg = new DatalogHypergraph(stream);
	else if(algorithm == HCFASP) hg = new DatalogHypergraph(stream);

	if(output == Consistency) inst = new GenericInstantiator<ConsistencySolution>(false);
	else if(output == Counting) inst = new GenericInstantiator<CountingSolution>(false);
	else if(output == Enumeration) inst = new GenericInstantiator<EnumerationSolution>(true);

	if(bOpt) { cout << "Using seed: " << seed << endl; }

	if(bOpt) { cout << "Parsing input and building Hypergraph..." << flush; t.start(); }

	CNOT0(hg->buildHypergraph());
	hg->reduce();

	if(bOpt) { cout << " done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	if(bOpt) { cout << "Decomposing hypergraph:" << endl; t.start(); }

	ht = decompose(hg);

	if(bOpt) { cout << "done! (hypertree width: " << ht->getHTreeWidth() << " - took "; printTime(t.stop()); cout << " seconds)" << endl; }

	if(bOpt) { cout << "Normalizing hypergraph... " << flush; t.start(); }

	eht = new ExtendedHypertree(ht); ht = NULL;
	eht->normalize();

	if(bOpt) { cout << "done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	if(tOpt) { exit(EXIT_SUCCESS); }

#ifdef DEBUG
	print(eht);
	printSignMap(hg->getSignMap());
	printNameMap(hg->getNameMap());
#endif

	switch(algorithm)
	{
	case SAT:
		PrintError("The SAT algorithm is currently not implemented!");
		break;
	case MinSAT:
		alg = new CircumscriptionAlgorithm(inst, eht, hg->getSignMap(), hg->getHeadMap(), hg->getNameMap());
		break;
	case ASP:
		alg = new AnswerSetAlgorithm(inst, eht, hg->getSignMap(), hg->getHeadMap(), hg->getNameMap());
		break;
	case HCFASP:
		alg = new HeadCycleFreeAnswerSetAlgorithm(inst, eht, hg->getSignMap(), hg->getHeadMap(), hg->getNameMap());
		break;
	default:
		C0(0 /*ERROR: Invalid algorithm assignment*/);
		break;
	}

	delete hg;

	if(bOpt) { cout << "Calculating formula... " << endl; t.start(); }

	Solution *temp = alg->evaluate();

	if(bOpt) { cout << "\ttree-evaluation took " << flush; printTime(t.stop()); cout << " seconds..." << endl; }
	
	SolutionContent *sc = temp->getContent();

	if(bOpt) { cout << "done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	printSolution(sc, output, alg->getNameMap());

	delete sc;
	delete alg;

	exit(EXIT_SUCCESS);
}

static void usage()
{
	cout 	<< "Usage: " 
		<< sProgramName 
		<< " [-b] [-t] [-s <seed>] [-f <file>] -a <alg> -o <output>" << endl 
		<< "\t-b\t\tprint benchmark information" << endl
		<< "\t-t\t\tperform only hypertree decomposition step" << endl
		<< "\t-s seed\t\tinitialize random number generator with <seed>." << endl
		<< "\t-f file\t\tthe file to read from" << endl
		<< "\t-a alg\t\talgorithm, one of {sat, minsat, asp, hcfasp}" << endl
		<< "\t-o output\toutput type, one of {enum, count, yesno}" << endl
		;

	exit(EXIT_FAILURE);
}

static Hypertree *decompose(Hypergraph *hg)
{
        Timer local;
        Hypertree *ht, *temp;
        H_BucketElim be;

        if(bOpt) { cout << "\tBuilding hypertree with BE[MCS] heuristics... " << flush; local.start(); }

        ht = be.buildHypertree(hg, BE_MCS_ORDER);

        if(bOpt) { cout << "done! (took "; printTime(local.stop()); cout << " seconds, "
			<< "width: " << ht->getHTreeWidth() << " [" << ht->getTreeWidth() << "])" << endl; }

        if(bOpt) { cout << "\tBuilding hypertree with BE[MIW] heuristics... " << flush; local.start(); }

        temp = be.buildHypertree(hg, BE_MIW_ORDER);
	if(temp->getHTreeWidth() < ht->getHTreeWidth()) { delete ht; ht = temp; }
	else delete temp;

        if(bOpt) { cout << "done! (took "; printTime(local.stop()); cout << " seconds, "
			<< "width: " << ht->getHTreeWidth() << " [" << ht->getTreeWidth() << "])" << endl; }

        if(bOpt) { cout << "\tBuilding hypertree with BE[MF] heuristics... " << flush; local.start(); }

        temp = be.buildHypertree(hg, BE_MF_ORDER);
	if(temp->getHTreeWidth() < ht->getHTreeWidth()) { delete ht; ht = temp; }
	else delete temp;

        if(bOpt) { cout << "done! (took "; printTime(local.stop()); cout << " seconds, "
			<< "width: " << ht->getHTreeWidth() << " [" << ht->getTreeWidth() << "])" << endl; }

        ht->swapChiLambda();
        ht->shrink(true);
        ht->swapChiLambda();

        return ht;
}

static void printSolution(SolutionContent *sc, OutputType output, NameMap &nameMap)
{
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
				cout << isep << nameMap[*iit];
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
static void print(ExtendedHypertree *eht)
{
        if(eht->getParent() == NULL) cout << "root: " << eht << endl;
        int type = eht->getType();

        cout << "node " << eht << ", parent = " << eht->getParent() << ", type = " << type << flush;
        if(type == Branch) cout << ", children: " << eht->firstChild() << " - " << eht->secondChild();
        else if(type != Leaf) cout << ", child: " << eht->firstChild();

        cout << ", vars: ";
        for(set<int>::const_iterator it = eht->getVariables().begin(); it != eht->getVariables().end(); ++it) cout << *it << ", ";
        cout << "clauses: ";
        for(set<int>::const_iterator it = eht->getRules().begin(); it != eht->getRules().end(); ++it) cout << *it << ", ";
        cout << "difference: " << eht->getDifference() << ", ";
        cout << "END" << endl;

        if(type == Branch) { print(eht->firstChild()); print(eht->secondChild()); }
        else if(type == Leaf) {}
        else print(eht->firstChild());
}

static void printSignMap(SignMap &eht)
{
        for(map<int, map<int, bool> >::iterator it = eht.begin(); it != eht.end(); ++it)
        {
                cout << "clause " << it->first << ": ";
                for(map<int, bool>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
                        cout << (sit->second ? "-" : "\0") << sit->first << ", "; cout << "END" << endl;
        }
}

static void printNameMap(NameMap &eht)
{
	for(unsigned int i = 0; i < eht.size(); ++i)
		cout << "|" << i << "->" << eht[i];
	cout << endl;
}
#endif

