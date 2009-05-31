
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "support/support.h"

#include "input/DIMACSHypergraph.h"
#include "sharp/ExtendedHypertree.h"

#include "htree/H_BucketElim.h"

static bool bBenchmark = false;

void usage(char *);
Hypertree *decompose(Hypergraph *);

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

