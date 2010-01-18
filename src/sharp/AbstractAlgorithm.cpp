#include <iostream>

#include "../support/support.h"

#include "AbstractAlgorithm.h"

using namespace std;

/*********************************\
|* CLASS: Tuple
\*********************************/
Tuple::Tuple() { }

Tuple::~Tuple() { }

/***********************************\
|* CLASS: SolutionContent
\***********************************/
SolutionContent::SolutionContent() { }

SolutionContent::SolutionContent(const std::set<Variable> &partition) { }

SolutionContent::~SolutionContent() { }

/***********************************\
|* CLASS: EnumerationSolutionContent
\***********************************/
EnumerationSolutionContent::EnumerationSolutionContent() { }

EnumerationSolutionContent::EnumerationSolutionContent(const std::set<Variable> &partition) 
{ 
	this->enumerations.insert(partition);
}

EnumerationSolutionContent::~EnumerationSolutionContent() { }

/***********************************\
|* CLASS: CountingSolutionContent
\***********************************/
CountingSolutionContent::CountingSolutionContent() 
{
	this->count = 0;
}

CountingSolutionContent::CountingSolutionContent(const std::set<Variable> &partition) 
{
	this->count = 1;
}

CountingSolutionContent::~CountingSolutionContent() { }

/***********************************\
|* CLASS: ConsistencySolutionContent
\***********************************/
ConsistencySolutionContent::ConsistencySolutionContent() 
{
	this->consistent = false;
}

ConsistencySolutionContent::ConsistencySolutionContent(const std::set<Variable> &partition) 
{
	this->consistent = true;
}

ConsistencySolutionContent::~ConsistencySolutionContent() { }

/***********************************\
|* CLASS: Solution
\***********************************/
Solution::Solution(Operation operation, Solution *left, Solution *right)
{
	C0(operation == CrossJoin || operation == Union);

	this->operation = operation;
	this->leftArgument = left;
	this->rightArgument = right;
}

Solution::Solution(Solution *child, int difference)
{
	this->operation = AddDifference;
	this->difference = difference;
	this->leftArgument = child;
}

Solution::Solution(const std::set<Variable> &partition) 
{
	this->operation = Value;
}

Solution::Solution() 
{
	this->operation = Value;
}

Solution::~Solution() 
{	
	if(this->content) delete this->content;
	if(this->leftArgument) delete this->leftArgument;
	if(this->rightArgument)	delete this->rightArgument;
}

SolutionContent *Solution::getContent()
{
	if(this->operation != Value) this->forceCalculation();
	return this->content;
}

inline void Solution::forceCalculation()
{
	switch(this->operation)
	{
	case CrossJoin:
		this->calculateCrossJoin();
		break;
	case Union:
		this->calculateUnion();
		break;
	case AddDifference:
		this->calculateAddDifference();
		break;
	case Value:
		break;
	default:
		C0(0 /*invalid operation*/);
		break;
	}

	this->operation = Value;
}

/***********************************\
|* CLASS: EnumerationSolution
\***********************************/
EnumerationSolution::EnumerationSolution(Operation operation, Solution *left, Solution *right)
	: Solution(operation, left, right) { }

EnumerationSolution::EnumerationSolution(Solution *child, int difference)
	: Solution(child, difference) { }

EnumerationSolution::EnumerationSolution(const std::set<Variable> &partition)
	: Solution(partition) 
{
	this->content = new EnumerationSolutionContent(partition);
}

EnumerationSolution::EnumerationSolution()
	: Solution() 
{
	this->content = new EnumerationSolutionContent();
}

EnumerationSolution::~EnumerationSolution() { }

void EnumerationSolution::calculateUnion()
{
	EnumerationSolutionContent 
		*left = (EnumerationSolutionContent *)leftArgument->getContent(),
		*right = (EnumerationSolutionContent *)rightArgument->getContent(),
		*calc = new EnumerationSolutionContent();
	
	this->content = calc;

	//FIXME: re-use the old instances
	//FIXME: use swap instead
	calc->enumerations = left->enumerations;
	calc->enumerations.insert(right->enumerations.begin(), right->enumerations.end());
}

void EnumerationSolution::calculateCrossJoin()
{
	EnumerationSolutionContent 
		*left = (EnumerationSolutionContent *)leftArgument->getContent(),
		*right = (EnumerationSolutionContent *)rightArgument->getContent(),
		*calc = new EnumerationSolutionContent();
	
	this->content = calc;
			
	//FIXME: re-use the old instances
	for(set<set<Variable> >::iterator i = left->enumerations.begin(); 
		i != left->enumerations.end(); ++i)
	{
		for(set<set<Variable> >::iterator j = right->enumerations.begin(); 
			j != right->enumerations.end(); ++j)
		{
			set<Variable> sol = *i;
			sol.insert(j->begin(), j->end());
			calc->enumerations.insert(sol);
		}
	}
}

void EnumerationSolution::calculateAddDifference()
{
	EnumerationSolutionContent 
		*child = (EnumerationSolutionContent *)leftArgument->getContent(),
		*calc = new EnumerationSolutionContent();
	
	this->content = calc;

	//FIXME: re-use the old instances
	for(set<set<Variable> >::iterator i = child->enumerations.begin(); 
		i != child->enumerations.end(); ++i)
	{ 
		//FIXME: use swap instead of copying...
		set<Variable> sol = *i;
		sol.insert(this->difference); 
		calc->enumerations.insert(sol);
	}
}

/***********************************\
|* CLASS: CountingSolution
\***********************************/
CountingSolution::CountingSolution(Operation operation, Solution *left, Solution *right)
	: Solution(operation, left, right) { }

CountingSolution::CountingSolution(Solution *child, int difference)
	: Solution(child, difference) { }

CountingSolution::CountingSolution(const std::set<Variable> &partition)
	: Solution(partition) 
{
	this->content = new CountingSolutionContent(partition);
}

CountingSolution::CountingSolution()
	: Solution() 
{
	this->content = new CountingSolutionContent();
}

CountingSolution::~CountingSolution() { }

void CountingSolution::calculateUnion()
{
	CountingSolutionContent 
		*left = (CountingSolutionContent *)leftArgument->getContent(),
		*right = (CountingSolutionContent *)rightArgument->getContent(),
		*calc = new CountingSolutionContent();
	
	this->content = calc;

	//FIXME: re-use the old instances
	calc->count = left->count + right->count;
}

void CountingSolution::calculateCrossJoin()
{
	CountingSolutionContent 
		*left = (CountingSolutionContent *)leftArgument->getContent(),
		*right = (CountingSolutionContent *)rightArgument->getContent(),
		*calc = new CountingSolutionContent();
	
	this->content = calc;
		
	//FIXME: re-use the old instances	
	calc->count = left->count * right->count;
}

void CountingSolution::calculateAddDifference()
{
	CountingSolutionContent 
		*child = (CountingSolutionContent *)leftArgument->getContent(),
		*calc = new CountingSolutionContent();
	
	this->content = calc;

	//FIXME: re-use the old instances
	calc->count = child->count;
}

/***********************************\
|* CLASS: ConsistencySolution
\***********************************/
ConsistencySolution::ConsistencySolution(Operation operation, Solution *left, Solution *right)
	: Solution(operation, left, right) { }

ConsistencySolution::ConsistencySolution(Solution *child, int difference)
	: Solution(child, difference) { }

ConsistencySolution::ConsistencySolution(const std::set<Variable> &partition)
	: Solution(partition) 
{
	this->content = new ConsistencySolutionContent(partition);
}

ConsistencySolution::ConsistencySolution()
	: Solution() 
{
	this->content = new ConsistencySolutionContent();
}

ConsistencySolution::~ConsistencySolution() { }

void ConsistencySolution::calculateUnion()
{
	ConsistencySolutionContent 
		*left = (ConsistencySolutionContent *)leftArgument->getContent(),
		*right = (ConsistencySolutionContent *)rightArgument->getContent(),
		*calc = new ConsistencySolutionContent();
	
	this->content = calc;

	//FIXME: re-use the old instances
	calc->consistent = left->consistent || right->consistent;
}

void ConsistencySolution::calculateCrossJoin()
{
	ConsistencySolutionContent 
		*left = (ConsistencySolutionContent *)leftArgument->getContent(),
		*right = (ConsistencySolutionContent *)rightArgument->getContent(),
		*calc = new ConsistencySolutionContent();
	
	this->content = calc;
		
	//FIXME: re-use the old instances	
	calc->consistent = left->consistent || right->consistent;
}

void ConsistencySolution::calculateAddDifference()
{
	ConsistencySolutionContent 
		*child = (ConsistencySolutionContent *)leftArgument->getContent(),
		*calc = new ConsistencySolutionContent();
	
	this->content = calc;

	//FIXME: re-use the old instances
	calc->consistent = child->consistent;
}

/***********************************\
|* CLASS: Instantiator
\***********************************/
Instantiator::Instantiator() { }

Instantiator::~Instantiator() { }

/***********************************\
|* CLASS: AbstractAlgorithm
\***********************************/
AbstractAlgorithm::AbstractAlgorithm(const Instantiator *instantiator, const ExtendedHypertree *root, 
	const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap)
{
	this->instantiator = instantiator;
	this->root = root;

	this->signMap = signMap;
	this->headMap = headMap;
	this->nameMap = nameMap;
}

AbstractAlgorithm::~AbstractAlgorithm() 
{ 
	if(root) delete root;
	if(instantiator) delete instantiator;
}

Solution *AbstractAlgorithm::evaluate()
{
	return selectSolution(evaluateNode(root));
}

TupleSet *AbstractAlgorithm::evaluateNode(const ExtendedHypertree *node)
{
	switch(node->getType())
        {
        case Leaf: 
		return this->evaluateLeafNode(node);
        case VariableRemoval: 
		return this->evaluateVariableRemovalNode(node);
        case VariableIntroduction: 
		return this->evaluateVariableIntroductionNode(node);
        case Branch: 
		return this->evaluateBranchNode(node);
        case RuleRemoval: 
		return this->evaluateRuleRemovalNode(node);
        case RuleIntroduction: 
		return this->evaluateRuleIntroductionNode(node);
	default:
		C0(0 /*invalid node type*/); return NULL;
        }
}

HeadMap &AbstractAlgorithm::getHeadMap()
{
	return this->headMap;
}

SignMap &AbstractAlgorithm::getSignMap()
{
	return this->signMap;
}

NameMap &AbstractAlgorithm::getNameMap()
{
	return this->nameMap;
}

