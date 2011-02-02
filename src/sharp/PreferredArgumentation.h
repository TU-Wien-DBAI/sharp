/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for (Preferred) Argumentation Frameworks (Header)
*/ 

#ifndef PREFARGUMENTATIONPROGRAM_H_
#define PREFARGUMENTATIONPROGRAM_H_

#include "AbstractArgumentation.h"

typedef std::set<ColoringVector> CertificateSet;

class PreferredArgumentationTuple : public Tuple
{
public:
	PreferredArgumentationTuple();
	virtual ~PreferredArgumentationTuple();

	//used for the mapping Argument<>Coloring
	ColoringVector colorings;
		
	//true, if skeptical acceptance for the given argument does not hold
	bool bSkepticalAcc;

	CertificateSet certificates;
	//int coloringNbr;
	//std::set<int> certificates;

	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class PreferredArgumentationAlgorithm : public AbstractArgumentationAlgorithm
{
public:
	PreferredArgumentationAlgorithm(Problem *problem, char *credulousAcc);
	virtual ~PreferredArgumentationAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node);

protected:
	Argument intSkepticalAcc;
	char *skepticalAcc;
	
private:
	//Returns true, if the first argument is a subset of the second one (just args with IN)
	bool isSubset (ColoringVector *v1, ColoringVector *v2);

	//Compares the given ColoringVector with every element in the given CertificateSet and returns a correspondig subset where the IN-args are equal to the ColoringVector.
	CertificateSet getEqualInSets(ColoringVector *cv, CertificateSet *certs, const ArgumentSet *args);

	//Tries to insert a new element in the tupleset
	void addToTupleSet(PreferredArgumentationTuple *t, Solution *s, TupleSet *ts, Operation op);
};

#endif
