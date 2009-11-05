#include "../support/support.h"

#include "AbstractAlgorithm.h"

using namespace std;

bool lessTuple(const Tuple *t1, const Tuple *t2)
{
	return *t1 < *t2;
}

Tuple::Tuple() { }

Tuple::~Tuple() { }

SolutionContent::SolutionContent() { }

SolutionContent::~SolutionContent() { }

Solution::Solution() 
{ 
	this->content = NULL;
}

Solution::Solution(SolutionContent *content) 
{
	this->content = content;
}

Solution::~Solution() 
{	
	if(this->content) delete this->content;
}

SolutionContent *Solution::getContent()
{
	return this->content;
}

LazySolution::LazySolution(Operation operation, Solution *left, Solution *right)
{
	C0(operation == LazyCrossJoin || operation == LazyUnion);

	this->operation = operation;
	this->leftArgument = left;
	this->rightArgument = right;
}

LazySolution::LazySolution(Solution *child, int difference)
{
	this->operation = LazyAddDifference;
	this->difference = difference;
	this->leftArgument = child;
}

LazySolution::~LazySolution()
{
	if(this->leftArgument) delete this->leftArgument;
	if(this->rightArgument)	delete this->rightArgument;
}

SolutionContent *LazySolution::getContent()
{
	if(this->operation != Value) this->calculate();
	return this->content;
}

inline void LazySolution::calculate()
{
	switch(this->operation)
	{
	case LazyCrossJoin:
		this->calculateCrossJoin();
		break;
	case LazyUnion:
		this->calculateUnion();
		break;
	case LazyAddDifference:
		this->calculateAddDifference();
		break;
	default:
		break;
	}

	this->operation = Value;
}

Instantiator::Instantiator() { }

Instantiator::~Instantiator() { }

AbstractAlgorithm::AbstractAlgorithm(const Instantiator *instantiator, const ExtendedHypertree *root, const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap)
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
		CNULL(NULL);
		return NULL;
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

