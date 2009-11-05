
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
#include "htree/H_BucketElim.h"

using namespace std;

static bool bOpt = false;
static Timer t;

enum Algorithm
{
	NoAlg,
	SAT,
	MinSAT,
	ASP
};

enum OutputType
{
	NoType,
	YesNo,
	Counting,
	Enumeration
};

static void usage();
static Hypertree *decompose(Hypergraph *);
static void printTime(pair<double, double>);
static void printSolution(SolutionContent *, Algorithm, OutputType, NameMap &);

#ifdef DEBUG
static void print(ExtendedHypertree *);
static void printSignMap(SignMap &);
#endif

int main(int argc, char **argv)
{
	InitializeErrorHandling(argv[0]);

	/* Argument Handling */

	int arg, argCount = 1;
	char *sEnd = NULL;
	bool sOpt = false, fOpt = false;
	unsigned int seed = (unsigned int)time(NULL);
	Algorithm algorithm = NoAlg;
	OutputType output = NoType;
	char *filename = NULL;
	ifstream file;
	istream *stream = &std::cin;

	if(argc < 5) usage();

	while((arg = getopt(argc, argv, "a:bs:o:f:")) != EOF)
	{
		++argCount;
		switch(arg)
		{
		case 'a':
			if(algorithm || !optarg) usage();
			if(!strcmp(optarg, "sat")) algorithm = SAT;
			else if(!strcmp(optarg, "minsat")) algorithm = MinSAT;
			else if(!strcmp(optarg, "asp")) algorithm = ASP;
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
		case 'o':
			if(output || !optarg) usage();
			if(!strcmp(optarg, "enum")) output = Enumeration;
			else if(!strcmp(optarg, "count")) output = Counting;
			else if(!strcmp(optarg, "yesno")) output = YesNo;
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
			CNULL(NULL);
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

	Hypertree *ht;
	ExtendedHypertree *eht;
	AbstractHypergraph *hg;
	Instantiator *inst;
	AbstractAlgorithm *alg;

	if(algorithm == SAT)
	{
		PrintError("The SAT algorithm is currently not implemented!");
	}
	else if(algorithm == MinSAT)
	{
		if(output == Enumeration)
		{
			inst = new CircumscriptionEnumInstantiator();
			hg = new DIMACSHypergraph(stream);
		}
		else if(output == Counting)
		{
			PrintError("Counting for the MinSAT algorithm is currently not implemented!");
		}
		else if(output == YesNo)
		{
			PrintError("Consistency evaluation for the MinSAT algorithm is currently not implemented!");
		}
	}
	else if(algorithm == ASP)
	{
		if(output == Enumeration)
		{
			inst = new AnswerSetEnumInstantiator();
			hg = new DatalogHypergraph(stream);
		}
		else if(output == Counting)
		{
			PrintError("Counting for the AnswerSet algorithm is currently not implemented!");
		}
		else if(output == YesNo)
		{
			PrintError("Consistency evaluation for the AnswerSet algorithm is currently not implemented!");
		}
	}

	if(bOpt) { cout << "Parsing input and building Hypergraph..." << flush; t.start(); }

	CNOT0(hg->buildHypergraph());
	hg->reduce();

	if(bOpt) { cout << " done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	if(bOpt) { cout << "Decomposing hypergraph:" << endl; t.start(); }

	ht = decompose(hg);

	if(bOpt) { cout << "done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	if(bOpt) { cout << "Normalizing hypergraph... " << flush; t.start(); }

	eht = new ExtendedHypertree(ht); ht = NULL;
	eht->normalize();

	if(bOpt) { cout << "done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

#ifdef DEBUG
	print(eht);
	printSignMap(hg->getSignMap());
#endif

	if(algorithm == SAT) { }
	else if(algorithm == MinSAT) { alg = new CircumscriptionAlgorithm(inst, eht, hg->getSignMap(), hg->getHeadMap(), hg->getNameMap()); }
	else if(algorithm == ASP) { alg = new AnswerSetAlgorithm(inst, eht, hg->getSignMap(), hg->getHeadMap(), hg->getNameMap()); }
	delete hg;

	if(bOpt) { cout << "Evaluating formula... " << flush; t.start(); }

	SolutionContent *sc = alg->evaluate()->getContent();

	if(bOpt) { cout << "done! (took "; printTime(t.stop()); cout << " seconds)" << endl; }

	printSolution(sc, algorithm, output, alg->getNameMap());

	delete sc;
	delete alg;


	exit(EXIT_SUCCESS);
}

static void usage()
{
	cout 	<< "Usage: " 
		<< sProgramName 
		<< " [-b] [-s <seed>] [-f <file>] -a <alg> -o <output>" << endl 
		<< "\t-b\t\tprint benchmark information" << endl
		<< "\t-s seed\t\tinitialize random number generator with <seed>." << endl
		<< "\t-f file\t\tthe file to read from" << endl
		<< "\t-a alg\t\talgorithm, one of {sat, minsat, asp}" << endl
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

static void printSolution(SolutionContent *sc, Algorithm algorithm, OutputType output, NameMap &nameMap)
{
	//TODO: implement correctly for all algorithm/output variation
	AnswerSetEnumSolutionContent *sol = (AnswerSetEnumSolutionContent *)sc;

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
#endif

