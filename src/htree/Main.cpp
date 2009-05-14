
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

#include "Parser.h"
#include "Hypergraph.h"
#include "Hypertree.h"
#include "Component.h"
#include "Node.h"
#include "Hyperedge.h"
#include "Globals.h"
#include "H_BucketElim.h"


void usage(int, char **, bool *, bool *);
Hypertree *decompose(Hypergraph *);
Hypertree *decompBE(Hypergraph *);
Hypertree *decompDualBE(Hypergraph *);
bool verify(Hypergraph *, Hypertree *);
void writeStats(const char *, time_t &, time_t &, Hypergraph *, Hypertree *, const char * = "");


char *cInpFile, *cOutFile;
bool bEnableStatistics = false;



int htreemain(int argc, char **argv)
{
	bool bDef;
	time_t start, end;
	Hypergraph HG;
	Parser *P;
	Hypertree *HT;
	
	// Check command line arguments
	usage(argc, argv, &bDef, &bEnableStatistics);
	srand(time(NULL));

	// Create parser object
	if((P = new Parser(bDef)) == NULL)
		writeErrorMsg("Error assigning memory.", "main");

	// Parse file
	cout << "Parsing input file \"" << cInpFile << "\" ... " << endl;
	time(&start);
	P->parseFile(cInpFile);
	time(&end);
	cout << "Parsing input file done in " << difftime(end, start) << " sec";
	cout << " (" << P->getNbrOfAtoms() << " atoms, " << P->getNbrOfVars() << " variables)." << endl << endl;

	// Build hypergraph
	cout << "Building hypergraph ... " << endl;
	time(&start);
	HG.buildHypergraph(P);
	HG.reduce();
	if(!HG.isConnected())
		cerr << "Warning: Hypergraph is not connected." << endl;
	time(&end);
	cout << "Building hypergraph done in " << difftime(end, start) << " sec." << endl << endl;
	delete P;

	HT = decompose(&HG);

	// Check hypertree conditions
	cout << "Checking hypertree conditions ... " << endl;
	time(&start);
	verify(&HG, HT);
	time(&end);
	cout << "Checking hypertree conditions done in " << difftime(end, start) << " sec ";
	cout << "(hypertree width: " << HT->getHTreeWidth() << ")." << endl << endl;

	HT->outputToGML(&HG, cOutFile);
	cout << "GML output written to: " << cOutFile << endl << endl;
	delete HT;

	return 0;
}


/*
***Description***
The function checks the command-line arguments.

INPUT:	argc: Number of command-line arguments
		argv: Command-line arguments
OUTPUT:	bDef: Boolean value indicating whether the command-line option -def was set
		bEnableStatistics: true if command-line option -stats was set

***History***
Written: (23.11.04, MS)
------------------------------------------------------------------------------------------------
Changed: (11.04.05, TG)
Comments: read command-line option -stats enabling statistical output for automated benchmarks
------------------------------------------------------------------------------------------------
Changed: (date, author)
Comments:
------------------------------------------------------------------------------------------------
*/

void usage(int argc, char **argv, bool *bDef, bool *bEnableStatistics)
{
	int i;
	*bDef = false;
	*bEnableStatistics = false;

	// Check arguments
	for (i=1; (i < argc) && (argv[i][0] == '-'); i++)
		if (strcmp(argv[i], "-def") == 0)
			*bDef = true;
		else if (strcmp(argv[i], "-stats") == 0)
			*bEnableStatistics = true;
		else {
			cerr << "Unknown argument \"" << argv[i] << "\"." << endl;
			exit(EXIT_FAILURE);
		}

	// Write usage error message
	if ((argc < 2) || (i < argc-1)) {
		cerr << "Usage: htdecomp [-opt] <filename>" << endl;
		cerr << "-opt: def - definitions have to be checked" << endl;
		cerr << "      stats - enable statistical output for automated benchmarks" << endl;
		exit(EXIT_FAILURE);
	}

	cInpFile = argv[i];

	// Construct output-file name
	for(i=strlen(cInpFile)-1; i > 0; i--)
		if(cInpFile[i] == '.')
			break;
	if(i > 0)
		cInpFile[i] = '\0';
	cOutFile = new char[strlen(cInpFile)+5];
	if(cOutFile == NULL)
		writeErrorMsg("Error assigning memory.", "usage");
	strcpy(cOutFile, cInpFile);
	strcat(cOutFile, ".gml");
	if(i > 0)
		cInpFile[i] = '.';
}


/*
***Description***
The function applys various hypertree decomposition heuristics in order to decompose the 
given hypergraph.

INPUT:	HG: Hypergraph to be decomposed
OUTPUT:	return: Hypertree decomposition of HG

***History***
Written: (02.03.05, MS)
------------------------------------------------------------------------------------------------
Changed: (date, author)
Comments:
------------------------------------------------------------------------------------------------
*/

Hypertree *decompose(Hypergraph *HG)
{
	Hypertree *HT;

	HT = decompBE(HG);
	// HT = decompDualBE(HG);

	return HT;
}



/*
***Description***
The function applies bucket elimination hypertree decomposition heuristics in order to decompose 
the given hypergraph. In particular, a tree decomposition is built by bucket elimination which 
is afterwards extended to a hypertree decomposition by using set covering heuristics.

INPUT:	HG: Hypergraph to be decomposed
OUTPUT:	return: Hypertree decomposition of HG

***History***
Written: (02.03.05, MS)
------------------------------------------------------------------------------------------------
Changed: (date, author)
Comments:
------------------------------------------------------------------------------------------------
*/

Hypertree *decompBE(Hypergraph *HG)
{
	time_t start, end;
	Hypertree *HT, *TmpHT;
	H_BucketElim BE;

	// Apply bucket elimination with maximum cardinality search order heuristic
	cout << "Building hypertree (BE[MCS] heuristic) ... " << endl;
	time(&start);
	HT = BE.buildHypertree(HG, BE_MCS_ORDER);
	time(&end);
	cout << "Building hypertree done in " << difftime(end, start) << " sec";
	cout << " (hypertree width: " << HT->getHTreeWidth() << " [" << HT->getTreeWidth() << "])." << endl << endl;

	// output for automated statistics
	writeStats("BE[MCS]", start, end, HG, HT);

	// Apply bucket elimination with minimum induced width order heuristic
	cout << "Building hypertree (BE[MIW] heuristic) ... " << endl;
	time(&start);
	TmpHT = BE.buildHypertree(HG, BE_MIW_ORDER);
	time(&end);
	cout << "Building hypertree done in " << difftime(end, start) << " sec";
	cout << " (hypertree width: " << TmpHT->getHTreeWidth() << " [" << TmpHT->getTreeWidth() << "])." << endl << endl;

	// output for automated statistics
	writeStats("BE[MIW]", start, end, HG, TmpHT);

	if(TmpHT->getHTreeWidth() < HT->getHTreeWidth()) {
		delete HT;
		HT = TmpHT;
	}
	else
		delete TmpHT;

	// Apply bucket elimination with minimum fill-in order heuristic
	cout << "Building hypertree (BE[MF] heuristic) ... " << endl;
	time(&start);
	TmpHT = BE.buildHypertree(HG, BE_MF_ORDER);
	time(&end);
	cout << "Building hypertree done in " << difftime(end, start) << " sec";
	cout << " (hypertree width: " << TmpHT->getHTreeWidth() << " [" << TmpHT->getTreeWidth() << "])." << endl << endl;

	// output for automated statistics
	writeStats("BE[MF]", start, end, HG, TmpHT);

	if(TmpHT->getHTreeWidth() < HT->getHTreeWidth()) {
		delete HT;
		HT = TmpHT;
	}
	else
		delete TmpHT;

	HT->swapChiLambda();
	HT->shrink(true);
	HT->swapChiLambda();

	return HT;
}


/*
***Description***
The function applies bucket elimination hypertree decomposition heuristics in order to decompose 
the given hypergraph. In particular, a tree decomposition of the dual hypergraph is built by 
bucket elimination which is afterwards extended to a hypertree decomposition by adding chi-labels.

INPUT:	HG: Hypergraph to be decomposed
OUTPUT:	return: Hypertree decomposition of HG

***History***
Written: (02.03.05, MS)
------------------------------------------------------------------------------------------------
Changed: (date, author)
Comments:
------------------------------------------------------------------------------------------------
*/

Hypertree *decompDualBE(Hypergraph *HG)
{
	time_t start, end;
	Hypertree *HT, *TmpHT;
	H_BucketElim BE;

	// Apply dual bucket elimination with maximum cardinality search order heuristic
	cout << "Building hypertree (DBE[MCS] heuristic) ... " << endl;
	time(&start);
	HT = BE.buildHypertree(HG, BE_MCS_ORDER, true);
	time(&end);
	cout << "Building hypertree done in " << difftime(end, start) << " sec";
	cout << " (hypertree width: " << HT->getHTreeWidth() << " [" << HT->getTreeWidth() << "])." << endl << endl;

	// output for automated statistics
	writeStats("DBE[MCS]", start, end, HG, HT);

	// Apply dual bucket elimination with minimum induced width order heuristic
	cout << "Building hypertree (DBE[MIW] heuristic) ... " << endl;
	time(&start);
	TmpHT = BE.buildHypertree(HG, BE_MIW_ORDER, true);
	time(&end);
	cout << "Building hypertree done in " << difftime(end, start) << " sec";
	cout << " (hypertree width: " << TmpHT->getHTreeWidth() << " [" << TmpHT->getTreeWidth() << "])." << endl << endl;

	// output for automated statistics
	writeStats("DBE[MIW]", start, end, HG, TmpHT);

	if(TmpHT->getHTreeWidth() < HT->getHTreeWidth()) {
		delete HT;
		HT = TmpHT;
	}
	else
		delete TmpHT;

	// Apply dual bucket elimination with minimum fill-in order heuristic
	cout << "Building hypertree (DBE[MF] heuristic) ... " << endl;
	time(&start);
	TmpHT = BE.buildHypertree(HG, BE_MF_ORDER, true);
	time(&end);
	cout << "Building hypertree done in " << difftime(end, start) << " sec";
	cout << " (hypertree width: " << TmpHT->getHTreeWidth() << " [" << TmpHT->getTreeWidth() << "])." << endl << endl;

	// output for automated statistics
	writeStats("DBE[MF]", start, end, HG, TmpHT);

	if(TmpHT->getHTreeWidth() < HT->getHTreeWidth()) {
		delete HT;
		HT = TmpHT;
	}
	else
		delete TmpHT;
	
	return HT;
}


/*
***Description***
The function verifies whether a given hypertree decomposition to a given hypergraph satisfies
the conditions of a hypertree decompositon.

INPUT:	HG: Hypergraph
		HT: Hypertree decomposition
OUTPUT:	return: true if HT is a hypertree decomposition of HG; otherwise false

***History***
Written: (01.03.05, MS)
------------------------------------------------------------------------------------------------
Changed: (date, author)
Comments:
------------------------------------------------------------------------------------------------
*/

bool verify(Hypergraph *HG, Hypertree *HT)
{
	bool bAllCondSat = true;
	Node *WitnessNode;
	Hyperedge *WitnessEdge;
	Hypertree *WitnessTree;

	// Check acyclicity of the hypertree
	HT->resetLabels();
	if(HT->isCyclic())
		writeErrorMsg("Hypertree contains cycles.", "verify");
	HT->setIDLabels();

	// Check condition 1
	cout << "Condition 1: ";
	if((WitnessEdge = HT->checkCond1(HG)) == NULL)
		cout << "satisfied." << endl;
	else {
		cout << "violated! (see atom \"" << WitnessEdge->getName() << "\")" << endl;
		bAllCondSat = false;
	}

	// Check condition 2
	cout << "Condition 2: ";
	if((WitnessNode = HT->checkCond2(HG)) == NULL)
		cout << "satisfied." << endl;
	else {
		cout << "violated! (see variable \"" << WitnessNode->getName() << "\")" << endl;
		bAllCondSat = false;
	}

	// Check condition 3
	cout << "Condition 3: ";
	if((WitnessTree = HT->checkCond3(HG)) == NULL)
		cout << "satisfied." << endl;
	else {
		cout << "violated! (see hypertree node \"" << WitnessTree->getLabel() << "\")" << endl;
		bAllCondSat = false;
	}

	// Check condition 4
	cout << "Condition 4: ";
	if((WitnessTree = HT->checkCond4(HG)) == NULL)
		cout << "satisfied." << endl;
	else
		cout << "violated!" << endl; 
		// (see hypertree node \"" << WitnessTree->getLabel() << "\")" << endl;

	return bAllCondSat;
}


/*
***Description***
The function writes detailed statistics about decomposition to stdout.

INPUT:	cAlgorithm: Name of used algorithm and heuristic (e.g., BE[MCS])
		start, end: time at which decomposition started and finished, respectively
		HG: hypergraph which was decomposed
		HT: hypertree-decomposition of HG
		cComment: optional comment written to statistics file (defaults to "")
OUTPUT:

***History***
Written: (11.04.05, TG)
------------------------------------------------------------------------------------------------
Changed: (date, author)
Comments:
------------------------------------------------------------------------------------------------
*/

void writeStats(
	const char *cAlgorithm,
	time_t &start, time_t &end, 
	Hypergraph *HG, Hypertree *HT,
	const char *cComment)
{
	if(bEnableStatistics) {
		bool bValid = verify(HG, HT);
		// output for automated statistics
		cout << "*** STATISTICS ***," << cInpFile << "," << cAlgorithm << "," << difftime(end, start) << ",";
		cout << HT->getHTreeWidth() << ",";
		bValid ? cout << "t" : cout << "f";
		if(strlen(cComment) > 0)
			cout << "," << cComment;
		cout << endl;
	}
}


