/*
* Author: 		Clemens Nopp, Matr.Nr. 0826054
* Date: 		September 2010
* This File:	Algorithm for (Preferred) Argumentation Frameworks
*/ 

#include <iostream>
#include <algorithm>

#include "PreferredArgumentation.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)

/*
***Description***
Pretty print for a coloring vector

INPUT: c: the coloring vector
       tab: tabs between colors if true, otherwise no tabs but ';'
*/
static void printColoring (ColoringVector c, bool tab)
{
	for(ColoringVector::iterator colIt = c.begin(); colIt != c.end(); ++colIt)
	{
		string col;

		if (*colIt == 0) col = "Att";
		else if (*colIt == 1) col = "Def";
		else if (*colIt == 2) col = "In";
		else if (*colIt == 3) col = "Out";

		if (tab) cout << col << "\t";
		else cout << col << ";";
	}
}

static void printTuples(TupleSet *tuples, const ExtendedHypertree *node, ArgumentationProblem *problem)
{
        cout << endl << "Colorings for node " << node << ":" << endl;
        //cout << "Coloring ID\t";
		for(VertexSet::iterator it = node->getVertices().begin();
			it != node->getVertices().end();
			++it) 
		{
			cout << problem->getArgumentString((Argument)*it) << "\t";
		}
		
		cout << "skep\tcertificates";
 
		cout << endl;
 
        for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
        {
			//cout << &(((PreferredArgumentationTuple *)it->first)->certificates) << "\t";
			
			ColoringVector c = ((PreferredArgumentationTuple *)it->first)->colorings;
                    
            printColoring(c, true);
			
			cout << ((PreferredArgumentationTuple *)it->first)->bSkepticalAcc << "\t";

			CertificateSet cert = ((PreferredArgumentationTuple *)it->first)->certificates;

            for(CertificateSet::iterator certIt = cert.begin(); certIt != cert.end(); ++certIt)
            {
				printColoring(*certIt, false);
				cout << "\t";
			}

			cout << endl;	
        }
}


#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PreferredArgumentationTuple::PreferredArgumentationTuple() 
{
	bSkepticalAcc = false;
}

PreferredArgumentationTuple::~PreferredArgumentationTuple() { }

bool PreferredArgumentationTuple::operator<(const Tuple &other) const
{
	PreferredArgumentationTuple &o = (PreferredArgumentationTuple &)other;

	return ((this->colorings < o.colorings)
			|| ((this->colorings == o.colorings) && (this->certificates < o.certificates)));

	}

bool PreferredArgumentationTuple::operator==(const Tuple &other) const
{
	PreferredArgumentationTuple &o = (PreferredArgumentationTuple &)other;
	bool cert = true;

	if ((this->certificates).size() != (o.certificates).size())
	{
		cert = false;
	}
	else
	{
		for(CertificateSet::iterator it1 = (this->certificates).begin(); it1 != (this->certificates).end(); ++it1)
		{
			if ((o.certificates).count(*it1) == 0) cert = false;
		}
	}
	return (cert && (this->colorings == o.colorings));
}

int PreferredArgumentationTuple::hash() const
{
	//TODO
	return -1;
}

PreferredArgumentationAlgorithm::PreferredArgumentationAlgorithm(Problem *problem, char *skepticalAcc)
	: AbstractArgumentationAlgorithm(problem)
{
	this->skepticalAcc = skepticalAcc;
	this->intSkepticalAcc = -1;

#if defined(VERBOSE) && defined(DEBUG)
	if (this->skepticalAcc != NULL)
	{	
		cout << endl << "Algorithm will check skeptical acceptance for '" << skepticalAcc << "'." << endl; //(code: " << intSkepticalAcc << ")." << endl;
	}
#endif
	
}

PreferredArgumentationAlgorithm::~PreferredArgumentationAlgorithm() { }


Solution *PreferredArgumentationAlgorithm::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
{
	
	Solution *s = this->instantiator->createEmptySolution();
	bool skepAcc = true;
	ColoringVector colorings;
	CertificateSet deletedColorings;
	TupleSet tmpSolutions;
	TupleSet finalSolutions;
	bool containsAtt = false;	

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{	
		colorings = ((PreferredArgumentationTuple *)it->first)->colorings;
		containsAtt = false;		
		
		for(unsigned int i=0; i < colorings.size(); i++)
		{
			if (colorings[i] == ATT) 
			{
				containsAtt = true;
				break;
			}
			
		}

		if (!containsAtt)
		{
			tmpSolutions.insert(*it);
		}
		else deletedColorings.insert(colorings);
	}

	//delete all certificates which are equal to deleted colorings
	for(CertificateSet::iterator delIt = deletedColorings.begin(); delIt != deletedColorings.end(); ++delIt)
	{
		for(TupleSet::iterator it = tmpSolutions.begin(); it != tmpSolutions.end(); ++it)
		{
			CertificateSet *c = &(((PreferredArgumentationTuple *)it->first)->certificates);

			if (c->count(*delIt) > 0)
			{
				c->erase(*delIt);
			}

			//if there are not any certificates left => add to solution
			if(c->size() == 0)
			{
				finalSolutions.insert(*it);
			}
		}
	}

	//insert final solutions into framework
	for(TupleSet::iterator it = finalSolutions.begin(); it != finalSolutions.end(); ++it)
	{
		if(((PreferredArgumentationTuple *)it->first)->bSkepticalAcc) skepAcc = false;
		s = this->instantiator->combine(Union, s, it->second);
	}

	if (skepAcc && (skepticalAcc != NULL && strlen(skepticalAcc) > 0))
	{
		cout << endl << "Skeptical acceptance holds for the requested variable '" << skepticalAcc << "'." << endl;
	} 	
	else if (!skepAcc && (skepticalAcc != NULL && strlen(skepticalAcc) > 0))
	{
		cout << endl << "Skeptical acceptance does not hold for the requested variable '" << skepticalAcc << "'." << endl;
	} 	

	cout << endl;

	return s;
}

TupleSet *PreferredArgumentationAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateLeafNode." << endl;
#endif

	TupleSet *ts = new TupleSet();
	const ArgumentSet arguments = (ArgumentSet) node->getVertices();

	//get integer value for skeptical acceptance argument
	if (intSkepticalAcc == -1 && this->skepticalAcc != NULL)
	{
		intSkepticalAcc = problem->getVertexId(this->skepticalAcc);
	}

	//calculate conflict free sets
	vector< set<Argument> > cfSets = this->conflictFreeSets(&arguments);
	
	//go through conflict free sets and calculate colorings and certificates
	for(unsigned int i=0; i < (cfSets.size()); ++i)
	{
		PreferredArgumentationTuple &argTuple = *new PreferredArgumentationTuple();
	
		//calculate set with args attacked by cfSet
		set<Argument> attByCF = this->attackedBySet(&cfSets[i], problem);

		//calculate coloring for every argument
		for(VertexSet::iterator it = arguments.begin();
			it != arguments.end();
			++it) 
		{
			//empty set => arg is out
			if (cfSets[i].size() == 0)
			{
				//set Skeptical acceptance flag if current arg equals intSkepticalAcc
				if(*it == intSkepticalAcc) argTuple.bSkepticalAcc = true;

				(argTuple.colorings).push_back(OUT);
				//cout << "Calculated OUT for tupel " << i << ", Argument " << *it << endl;
			}
			
			//cfSets contains arg => In
			else if ((cfSets[i]).count(*it) > 0)
			{
				(argTuple.colorings).push_back(IN);
				//cout << "Calculated IN for tupel " << i << ", Argument " << *it << endl;
			} 
		
			//if list of args attacked by conflict free sets contains arg => Def
			else if (attByCF.count(*it) > 0)
			{
				//set Skeptical acceptance flag if current arg equals intSkepticalAcc
				if(*it == intSkepticalAcc) argTuple.bSkepticalAcc = true;

				(argTuple.colorings).push_back(DEF);
				//cout << "Calculated DEF for tupel " << i << ", Argument " << *it << endl;
			}

			//call attCheck to decide if IN-args are attacked by another arg
			else if (this->attCheck(&cfSets[i], (Argument) *it, problem)) 
			{
				//set Skeptical acceptance flag if current arg equals intSkepticalAcc
				if(*it == intSkepticalAcc) argTuple.bSkepticalAcc = true;

				(argTuple.colorings).push_back(ATT);
				//cout << "Calculated ATT for tupel " << i << ", Argument " << *it << endl;
			}
		
			//otherwise OUT
			else
			{
				(argTuple.colorings).push_back(OUT);
			}
		}		
		
		pair<TupleSet::iterator,bool> x = ts->insert(TupleSet::value_type(&argTuple, this->instantiator->createLeafSolution(cfSets[i])));
	}
	
	//iterate through tupleset and calculate certificates
	for(TupleSet::iterator it = ts->begin(); it != ts->end(); ++it)
	{	
		ColoringVector c1 = ((PreferredArgumentationTuple *)it->first)->colorings;
		
		for(TupleSet::iterator inner_it = ts->begin(); inner_it != ts->end(); ++inner_it)
		{
			if (*it != *inner_it)
			{
				
				ColoringVector c2 = ((PreferredArgumentationTuple *)inner_it->first)->colorings;	
				if (this->isSubset(c1, c2)) 
				{
					CertificateSet *certificates = &((PreferredArgumentationTuple *)it->first)->certificates;
					certificates->insert((((PreferredArgumentationTuple *)inner_it->first)->colorings));
				}
			}
		}	
	}

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);

	/*cout << "Conflict free sets:" << endl;
	for(vector<set<Argument> >::iterator it1 = cfSets.begin(); it1 != cfSets.end(); ++it1)
	{			
			for(set<Argument>::iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
			{
				cout << *it2 << " ";
		    }
			cout << endl;
	}*/
#endif

	return ts;
}

TupleSet *PreferredArgumentationAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateBranchNode." << endl;
#endif

	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());

	TupleSet *ts = new TupleSet();

	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet leftArguments = (ArgumentSet) (node->firstChild())->getVertices();
	const ArgumentSet rightArguments = (ArgumentSet) (node->secondChild())->getVertices();

	//iterate through left tuples
	for(TupleSet::iterator lit = left->begin(); lit != left->end(); ++lit)
	{	
		ColoringVector leftColoring = ((PreferredArgumentationTuple *)lit->first)->colorings;
		ArgumentSet leftIn = this->getInArgs(&leftArguments, &leftColoring);
		CertificateSet leftCert = ((PreferredArgumentationTuple *)lit->first)->certificates;
		
		//iterate through right tuples and generate new tuplesets
		for(TupleSet::iterator rit = right->begin(); rit != right->end(); ++rit)
		{	
			ColoringVector rightColoring = ((PreferredArgumentationTuple *)rit->first)->colorings;
			ArgumentSet rightIn = this->getInArgs(&rightArguments, &rightColoring);
			CertificateSet rightCert = ((PreferredArgumentationTuple *)rit->first)->certificates;
			
			//check if IN-sets are equal => generate new tuple
			if ((leftIn.size() == rightIn.size()) && (equal(leftIn.begin(), leftIn.end(), rightIn.begin())))
			{
				PreferredArgumentationTuple &argTuple = *new PreferredArgumentationTuple();
				
				//calculate bSkepticalAcc
				argTuple.bSkepticalAcc = ((PreferredArgumentationTuple *)lit->first)->bSkepticalAcc ||
										 ((PreferredArgumentationTuple *)rit->first)->bSkepticalAcc;
				
				//calculate colorings
				argTuple.colorings = getBranchColorings(leftColoring, rightColoring, arguments);
				
				//calculate certificates

				//compare left coloring with right certificates
				CertificateSet c1 = this->getEqualInSets(leftColoring, rightCert, arguments);
				(argTuple.certificates).insert(c1.begin(), c1.end());
				
				//compare right coloring with left certificates
				CertificateSet c2 = this->getEqualInSets(rightColoring, leftCert, arguments);
				(argTuple.certificates).insert(c2.begin(), c2.end());

				//compare left certificates with right certificates
				CertificateSet c3;
				for(CertificateSet::iterator cit = leftCert.begin(); cit != leftCert.end(); ++cit)
				{
					c3 = this->getEqualInSets(*cit, rightCert, arguments);
					(argTuple.certificates).insert(c3.begin(), c3.end());
				}

				//generate cross join solution
				Solution *s = this->instantiator->combine(CrossJoin, lit->second, rit->second);
				
				//add to tuple set with alternative operation 'union'
				addToTupleSet(&argTuple, s, ts, Union);
			}
		}
	}

	delete left;
	delete right;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;

}

TupleSet *PreferredArgumentationAlgorithm::evaluateIntroductionNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateIntroductionNode." << endl;
#endif
	
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();
	
	map<ColoringVector, CertificateSet> updateMap;
	int newArgIndex = -1;

	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet childArguments = (ArgumentSet) (node->firstChild())->getVertices();

	//iterate through tuples and generate new tupleset
	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{	
		PreferredArgumentationTuple &argTuple = *new PreferredArgumentationTuple();	
		ColoringVector childColoring = ((PreferredArgumentationTuple *)it->first)->colorings;
		bool addTuple = false;
		
		//take over bSkepticalAcc and certificates
		argTuple.bSkepticalAcc = ((PreferredArgumentationTuple *)it->first)->bSkepticalAcc;
		argTuple.certificates = ((PreferredArgumentationTuple *)it->first)->certificates;

		ArgumentSet in = this->getInArgs(&childArguments, &childColoring);
				
		//calculate colorings
		argTuple.colorings = getIntroColorings(&in, &childColoring, node->getDifference(), &addTuple, arguments);
		
		if (addTuple)
		{	
			PreferredArgumentationTuple &additionalArgTuple = *new PreferredArgumentationTuple();	
			
			//take over bSkepticalAcc and certificates
			additionalArgTuple.bSkepticalAcc = ((PreferredArgumentationTuple *)it->first)->bSkepticalAcc;
			additionalArgTuple.certificates = ((PreferredArgumentationTuple *)it->first)->certificates;
			
			int index = 0;
			for(set<Argument>::iterator it2 = arguments.begin(); it2 != arguments.end(); ++it2)
			{
				//current arg is the new one
				if (*it2 == node->getDifference())
				{
					(additionalArgTuple.colorings).push_back(IN);	
				}
				//current arg is not the new one
				else
				{
					//IN if arg is the new one (see above) or if the corresponding child arg is IN
					if (childColoring[index] == IN)
					{
						(additionalArgTuple.colorings).push_back(IN);	
					}
					
					//DEF if the new arg attacks the current arg or if the corresponding child arg is DEF 
					else if ((problem->getAttacksFromArg(node->getDifference()) != NULL &&
					          (problem->getAttacksFromArg(node->getDifference()))->count(*it2) > 0) ||
							  childColoring[index] == DEF)
					{
						(additionalArgTuple.colorings).push_back(DEF);	

						//set Skeptical acceptance flag if current arg equals intSkepticalAcc
						if(*it2 == intSkepticalAcc) additionalArgTuple.bSkepticalAcc = true;
					}
					
					//OUT if the corresponding child arg is OUT and there is no attack from 
					//the current arg to the new one or otherwise
					else if  ((problem->getAttacksFromArg(node->getDifference()) == NULL ||
							 (problem->getAttacksFromArg(node->getDifference()))->count(*it2) == 0) &&
							  (problem->getAttacksFromArg(*it2) == NULL ||
							 (problem->getAttacksFromArg(*it2))->count(node->getDifference()) == 0) &&
							  childColoring[index] == OUT)
					{
						(additionalArgTuple.colorings).push_back(OUT);

						//set Skeptical acceptance flag if current arg equals intSkepticalAcc
						if(*it2 == intSkepticalAcc) additionalArgTuple.bSkepticalAcc = true;
					}
					
					//ATT otherwise
					else
					{
						(additionalArgTuple.colorings).push_back(ATT);	

						//set Skeptical acceptance flag if current arg equals intSkepticalAcc
						if(*it2 == intSkepticalAcc) additionalArgTuple.bSkepticalAcc = true;
					}
					
					index++;
				}
			}	
			addTuple = false;
			
			//generate 'add difference' solution
			Solution *s = this->instantiator->addDifference(it->second,	node->getDifference());
			
			//add new tuple to tuple set with alternative operation 'union'
			//updateMap.insert(pair<ColoringVector, ColoringVector>(childColoring, CertificateSet::value_type(additionalArgTuple.colorings)));
			updateMap[childColoring].insert(CertificateSet::value_type(additionalArgTuple.colorings));

			//cout << "AdditionalArgTuple: "; printColoring(additionalArgTuple.colorings, false); cout << "cert: " << (additionalArgTuple.certificates).size() << endl;

			addToTupleSet(&additionalArgTuple, s, ts, Union);
		}	

		//add 'old' tuple to tuple set with alternative operation 'union'
		//updateMap.insert(pair<ColoringVector, ColoringVector>(childColoring, CertificateSet::value_type(argTuple.colorings)));
		updateMap[childColoring].insert(CertificateSet::value_type(argTuple.colorings));

		//cout << "ArgTuple: "; printColoring(argTuple.colorings, false); cout << "cert: " << (argTuple.certificates).size() << endl;

		addToTupleSet(&argTuple, it->second, ts, Union);
	}	

	//get index of new argument
	int index = 0;
	for(ArgumentSet::iterator argIt = arguments.begin(); argIt != arguments.end(); ++argIt)
	{
		if (*argIt == node->getDifference())
		{
			newArgIndex = index;
			break;
		}

		index++;
	}


	//calculate certificates
	for(TupleSet::iterator it = ts->begin(); it != ts->end(); ++it)
	{
		CertificateSet *c = &(((PreferredArgumentationTuple *)it->first)->certificates);
		CertificateSet newCertificates;

		//distinguish between coloring 'IN' or not 'IN' of the new Argument
		if ((((PreferredArgumentationTuple *)it->first)->colorings)[newArgIndex] == IN)
		{

			for(CertificateSet::iterator cit = c->begin(); cit != c->end(); ++cit)
			{
				map<ColoringVector, CertificateSet>::iterator iter = updateMap.find(*cit);

				if( iter != updateMap.end() )
				{
					for(CertificateSet::iterator updateIt = (iter->second).begin(); updateIt != (iter->second).end(); ++updateIt)
					{
						if ((*updateIt)[newArgIndex] == IN) newCertificates.insert(*updateIt);
					}
				}
			}

			((PreferredArgumentationTuple *)it->first)->certificates = newCertificates;
		}
		else
		{
			for(CertificateSet::iterator cit = c->begin(); cit != c->end(); ++cit)
			{
				map<ColoringVector, CertificateSet>::iterator iter = updateMap.find(*cit);

				if( iter != updateMap.end() )
				{
					//newCertificates.insert((iter->second).begin(), (iter->second).end());
					for(CertificateSet::iterator updateIt = (iter->second).begin(); updateIt != (iter->second).end(); ++updateIt)
					{
						newCertificates.insert(*updateIt);
					}

				}
			}

			CertificateSet *s;

			//search for certificateset with current coloring
			ColoringVector col = ((PreferredArgumentationTuple *)it->first)->colorings;

			for(map<ColoringVector, CertificateSet>::iterator updateIt = updateMap.begin(); updateIt != updateMap.end(); ++updateIt)
			{

				if((updateIt->second).count(col) > 0)
				{
					s = &(updateIt->second);
					break;
				}
			}

			//search for coloring where the new argument is 'IN' in this certificateset
			for(CertificateSet::iterator sit = s->begin(); sit != s->end(); ++sit)
			{
				if ((*sit)[newArgIndex] == IN)
				{
					newCertificates.insert(*sit);
					break;
				}
			}

			((PreferredArgumentationTuple *)it->first)->certificates = newCertificates;
		}
	}

	delete base;
	
#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

TupleSet *PreferredArgumentationAlgorithm::evaluateRemovalNode(const ExtendedHypertree *node)
{
#if defined(VERBOSE) && defined(DEBUG)
	cout << endl << "Entered evaluateRemovalNode." << endl;
#endif

	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();
	CertificateSet deletedColorings;
	map<ColoringVector, ColoringVector> updateMap;
	
	int delIndex = 0;

	const ArgumentSet arguments = (ArgumentSet) node->getVertices();
	const ArgumentSet childArguments = (ArgumentSet) (node->firstChild())->getVertices();
	
	//iterate through tuples and generate new tupleset
	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{	
		PreferredArgumentationTuple &argTuple = *new PreferredArgumentationTuple();	
		bool insertFlag = true;
		int index = 0;
		ColoringVector childColoring = ((PreferredArgumentationTuple *)it->first)->colorings;
		
		//take over bSkepticalAcc and certificates
		argTuple.bSkepticalAcc = ((PreferredArgumentationTuple *)it->first)->bSkepticalAcc;
		argTuple.certificates = ((PreferredArgumentationTuple *)it->first)->certificates;
		
		for(set<Argument>::iterator it2 = childArguments.begin(); it2 != childArguments.end(); ++it2)
		{
			//insert coloring if current argument is not the removed one
			if (*it2 != node->getDifference())
			{	
				(argTuple.colorings).push_back(childColoring[index]);	
			}
			
			//if current argument is the removed one and the previous coloring
			//was ATT => do not insert into new tupleset
			else if ((*it2 == node->getDifference()) && childColoring[index] == ATT)
			{
				insertFlag = false;
				delIndex = index;
				break;
			}
			
			index++; 
		}
		
		if(insertFlag)
		{
			//add tuple to tuple set with alternative operation 'union'
			addToTupleSet(&argTuple, it->second, ts, Union);

			updateMap.insert(pair<ColoringVector, ColoringVector>(childColoring, argTuple.colorings));
		}
		else deletedColorings.insert(childColoring);
	}

	//delete all certificates which are equal to deleted colorings
	for(CertificateSet::iterator delIt = deletedColorings.begin(); delIt != deletedColorings.end(); ++delIt)
	{
		for(TupleSet::iterator it = ts->begin(); it != ts->end(); ++it)
		{
			CertificateSet *c = &(((PreferredArgumentationTuple *)it->first)->certificates);

			if (c->count(*delIt) > 0)
			{
				c->erase(*delIt);
			}
		}
	}

	//update certificates (replace with colorings without the deleted argument)
	for(TupleSet::iterator it = ts->begin(); it != ts->end(); ++it)
	{
		CertificateSet *c = &(((PreferredArgumentationTuple *)it->first)->certificates);
		CertificateSet newCertificates;

		for(CertificateSet::iterator cit = c->begin(); cit != c->end(); ++cit)
		{
			map<ColoringVector, ColoringVector>::iterator iter = updateMap.find(*cit);

			if( iter != updateMap.end() ) {
				newCertificates.insert(iter->second);
		    }
		}

		((PreferredArgumentationTuple *)it->first)->certificates = newCertificates;
	}


	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node, problem);
#endif

	return ts;
}

/*
***Description***
Returns true, if the first argument is a subset of the second one (when looking on IN arguments)

INPUT: v1: first colorings
       v2: second colorings
*/
bool PreferredArgumentationAlgorithm::isSubset (ColoringVector v1, ColoringVector v2)
{
	for(unsigned int i=0; i < (v1.size()); ++i)
	{
		if ((v1[i] == IN) && (v2[i] != IN)) return false;
	}
	
	return true; 
}

/*
***Description***
Compares the given ColoringVector with every element in the given CertificateSet and returns a correspondig subset where the IN-args are equal to the ColoringVector.
INPUT: cv: the coloring vector for comparing
       certs: the certificate set for comparing (every cv in this set will be compared)
*/
CertificateSet PreferredArgumentationAlgorithm::getEqualInSets(ColoringVector cv, CertificateSet certs, ArgumentSet args)
{
	ArgumentSet cvIn = this->getInArgs(&args, &cv);
	CertificateSet resultCerts;

	for(CertificateSet::iterator cit = certs.begin(); cit != certs.end(); ++cit)
	{
		ColoringVector tmpVector = *cit;
		ArgumentSet certIn = this->getInArgs(&args, &tmpVector);

		//check if IN-sets are equal => generate new tuple
		if ((cvIn.size() == certIn.size()) && (equal(cvIn.begin(), cvIn.end(), certIn.begin())))
		{
			resultCerts.insert(this->getBranchColorings(cv, tmpVector, args));
		}
	}

	return resultCerts;
}
