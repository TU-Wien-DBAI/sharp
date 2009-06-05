
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "support/support.h"

#include "input/DIMACSHypergraph.h"
#include "sharp/ExtendedHypertree.h"
#include "sharp/SharpSAT.h"

#include "htree/H_BucketElim.h"

static bool bBenchmark = false;

void usage(char *);
Hypertree *decompose(Hypergraph *);

void print(ExtendedHypertree *foo)
{
	if(foo->getParent() == NULL) cout << "root: " << foo << endl;
	int type = foo->getType();
	
	cout << "node " << foo << ", parent = " << foo->getParent() << ", type = " << type << flush;
	if(type == ExtendedHypertree::BRANCH) cout << ", children: " << foo->firstChild() << " - " << foo->secondChild();
	else if(type != ExtendedHypertree::LEAF) cout << ", child: " << foo->firstChild();

	cout << ", vars: ";
	for(set<int>::const_iterator it = foo->getVariables().begin(); it != foo->getVariables().end(); ++it) cout << *it << ", ";
	cout << "clauses: ";
	for(set<int>::const_iterator it = foo->getClauses().begin(); it != foo->getClauses().end(); ++it) cout << *it << ", ";
	cout << "DONE" << endl;

	if(type == ExtendedHypertree::BRANCH) { print(foo->firstChild()); print(foo->secondChild()); }
	else if(type == ExtendedHypertree::LEAF) {}
	else print(foo->firstChild());
}

void printSignMap(signmap &foo)
{
	for(map<int, map<int, bool> >::iterator it = foo.begin(); it != foo.end(); ++it)
	{
		cout << "clause " << it->first << ": ";
		for(map<int, bool>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
			cout << (sit->second ? "-" : "\0") << sit->first << ", "; cout << "END" << endl;
	}
}

int main(int argc, char **argv)
{
	InitializeErrorHandling(argv[0]);

	ifstream isFile;
	clock_t tStart, tEnd;
	int iFileArg;
	Hypertree *ht;
	ExtendedHypertree *eht;
	DIMACSHypergraph dhg(&isFile);
	
	if(argc != 2 && argc != 3) usage(argv[0]);
	if(argc == 2 && !strcmp("-b", argv[1])) usage(argv[0]);
	if(argc == 3 && strcmp("-b", argv[1])) usage(argv[0]);
	if(argc == 3 && !strcmp("-b", argv[1])) bBenchmark = true;

	srand(time(NULL));

	if(bBenchmark) iFileArg = 2; else iFileArg = 1;

	isFile.open(argv[iFileArg], ios::in);
	if(!isFile.is_open()) { cerr << "ERROR: Could not find file '" << argv[iFileArg] << "'!" << endl; exit(EXIT_FAILURE); }

	if(bBenchmark) { cout << "Parsing file '" << argv[2] << "' and building Hypergraph..." << flush; tStart = clock(); }

	dhg.buildHypergraph();
	dhg.reduce();

	if(bBenchmark) { tEnd = clock(); cout << " done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds)" << endl; }

	if(bBenchmark) { cout << "Decomposing hypergraph:" << endl; tStart = clock(); }

	ht = decompose(&dhg);

	if(bBenchmark) { tEnd = clock(); cout << "done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds)" << endl; }

	if(bBenchmark) { cout << "Normalizing hypergraph... " << flush; tStart = clock(); }

	eht = new ExtendedHypertree(ht); ht = NULL;
	eht->normalize();

	if(bBenchmark) { tEnd = clock(); cout << "done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds)" << endl; }

#ifdef DEBUG1
	print(eht);
	printSignMap(dhg.getSignMap());
#endif

	if(bBenchmark) { cout << "Evaluating formula:" << endl; tStart = clock(); }

	SharpSAT *ss = new SharpSAT(eht, dhg.getSignMap());
	cout << ss->evaluate() << endl;

	if(bBenchmark) { tEnd = clock(); cout << "done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds)" << endl; }

	exit(EXIT_SUCCESS);
}

void usage(char *progname)
{
	cout << "Usage: " << progname << " [-b] file" << endl << "\t-b\tprint benchmark information" << endl;
	exit(EXIT_FAILURE);
}

Hypertree *decompose(Hypergraph *hg)
{
        clock_t tStart, tEnd;
        Hypertree *ht, *temp;
        H_BucketElim be;

        if(bBenchmark) { cout << "\tBuilding hypertree with BE[MCS] heuristics... " << flush; tStart = clock(); }

        ht = be.buildHypertree(hg, BE_MCS_ORDER);

        if(bBenchmark) { tEnd = clock(); cout << "done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds, "
					<< "width: " << ht->getHTreeWidth() << " [" << ht->getTreeWidth() << "])" << endl; }

        if(bBenchmark) { cout << "\tBuilding hypertree with BE[MIW] heuristics... " << flush; tStart = clock(); }

        temp = be.buildHypertree(hg, BE_MIW_ORDER);
	if(temp->getHTreeWidth() < ht->getHTreeWidth()) { delete ht; ht = temp; }
	else delete temp;

        if(bBenchmark) { tEnd = clock(); cout << "done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds, "
					<< "width: " << ht->getHTreeWidth() << " [" << ht->getTreeWidth() << "])" << endl; }

        if(bBenchmark) { cout << "\tBuilding hypertree with BE[MF] heuristics... " << flush; tStart = clock(); }

        temp = be.buildHypertree(hg, BE_MF_ORDER);
	if(temp->getHTreeWidth() < ht->getHTreeWidth()) { delete ht; ht = temp; }
	else delete temp;

        if(bBenchmark) { tEnd = clock(); cout << "done! (took " << (double)(tEnd - tStart)/CLOCKS_PER_SEC << " seconds, "
					<< "width: " << ht->getHTreeWidth() << " [" << ht->getTreeWidth() << "])" << endl; }

        ht->swapChiLambda();
        ht->shrink(true);
        ht->swapChiLambda();

        return ht;
}

