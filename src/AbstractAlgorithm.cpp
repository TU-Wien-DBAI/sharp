#include <config.h>

#include <AbstractAlgorithm.hpp>
#include <CountingSolutionContent.hpp>

#include <iostream>
using namespace std;

#include <ExtendedHypertree.hpp>

namespace sharp
{

	/*********************************\
	|* CLASS: Tuple
	\*********************************/
	Tuple::Tuple() { }
	
	Tuple::~Tuple() { }
	
	/***********************************\
	|* CLASS: SolutionContent
	\***********************************/
	SolutionContent::SolutionContent() { }
	
	SolutionContent::~SolutionContent() { }
	
	/***********************************\
	|* CLASS: EnumerationSolutionContent
	\***********************************/
	EnumerationSolutionContent::EnumerationSolutionContent() { }

	EnumerationSolutionContent::EnumerationSolutionContent(const VertexSet &partition) 
	{ 
		this->enumerations.insert(partition);
	}
	
	EnumerationSolutionContent::~EnumerationSolutionContent() { }
	
	SolutionContent *EnumerationSolutionContent::calculateUnion(SolutionContent *other)
	{
		EnumerationSolutionContent 
			*left = this,
			*right = (EnumerationSolutionContent *)other,
			*calc = new EnumerationSolutionContent();
	
		//FIXME: re-use the old instances
		//FIXME: use swap instead
		calc->enumerations = left->enumerations;
		calc->enumerations.insert(right->enumerations.begin(), right->enumerations.end());
	
		return calc;
	}
	
	SolutionContent *EnumerationSolutionContent::calculateCrossJoin(SolutionContent *other)
	{
		EnumerationSolutionContent 
			*left = this,
			*right = (EnumerationSolutionContent *)other,
			*calc = new EnumerationSolutionContent();
		
		//FIXME: re-use the old instances
		for(set<VertexSet>::iterator i = left->enumerations.begin(); 
			i != left->enumerations.end(); ++i)
		{
			for(set<VertexSet>::iterator j = right->enumerations.begin(); 
				j != right->enumerations.end(); ++j)
			{
				VertexSet sol = *i;
				sol.insert(j->begin(), j->end());
				calc->enumerations.insert(sol);
			}
		}
	
		return calc;
	}
	
	SolutionContent *EnumerationSolutionContent::calculateAdd(const VertexSet &toAdd)
	{
		EnumerationSolutionContent 
			*child = this,
			*calc = new EnumerationSolutionContent();
	
		//FIXME: re-use the old instances
		for(set<VertexSet >::iterator i = child->enumerations.begin(); 
			i != child->enumerations.end(); ++i)
		{ 
			//FIXME: use swap instead of copying...
			VertexSet sol = *i;
			sol.insert(toAdd.begin(), toAdd.end()); 
			calc->enumerations.insert(sol);
		}
	
		return calc;
	}

	/***********************************\
	|* CLASS: CountingSolutionContent
	\***********************************/
	CountingSolutionContent::CountingSolutionContent() 
	{
		this->count = 0;
	}
	
	CountingSolutionContent::CountingSolutionContent(const VertexSet &partition) 
	{
		this->count = 1;
	}
	
	CountingSolutionContent::~CountingSolutionContent() { }
	
	SolutionContent *CountingSolutionContent::calculateUnion(SolutionContent *other)
	{
		CountingSolutionContent 
			*left = this,
			*right = (CountingSolutionContent *)other,
			*calc = new CountingSolutionContent();
		
		//FIXME: re-use the old instances
		calc->count = left->count + right->count;
	
		return calc;
	}
	
	SolutionContent *CountingSolutionContent::calculateCrossJoin(SolutionContent *other)
	{
		CountingSolutionContent 
			*left = this,
			*right = (CountingSolutionContent *)other,
			*calc = new CountingSolutionContent();
			
		//FIXME: re-use the old instances	
		calc->count = left->count * right->count;
	
		return calc;
	}
	
	SolutionContent *CountingSolutionContent::calculateAdd(const VertexSet &toAdd)
	{
		CountingSolutionContent 
			*child = this,
			*calc = new CountingSolutionContent();
	
		//FIXME: re-use the old instances
		calc->count = child->count;
	
		return calc;
	}
	
	/***********************************\
	|* CLASS: ConsistencySolutionContent
	\***********************************/
	ConsistencySolutionContent::ConsistencySolutionContent() 
	{
		this->consistent = false;
	}
	
	ConsistencySolutionContent::ConsistencySolutionContent(const VertexSet &partition) 
	{
		this->consistent = true;
	}
	
	ConsistencySolutionContent::~ConsistencySolutionContent() { }
	
	SolutionContent *ConsistencySolutionContent::calculateUnion(SolutionContent *other)
	{
		ConsistencySolutionContent 
			*left = this,
			*right = (ConsistencySolutionContent *)other,
			*calc = new ConsistencySolutionContent();
	
		//FIXME: re-use the old instances
		calc->consistent = left->consistent || right->consistent;
	
		return calc;
	}
	
	SolutionContent *ConsistencySolutionContent::calculateCrossJoin(SolutionContent *other)
	{
		ConsistencySolutionContent 
			*left = this,
			*right = (ConsistencySolutionContent *)other,
			*calc = new ConsistencySolutionContent();
	
		//FIXME: re-use the old instances	
		calc->consistent = left->consistent && right->consistent;
	
		return calc;
	}
	
	SolutionContent *ConsistencySolutionContent::calculateAdd(const VertexSet &toAdd)
	{
		ConsistencySolutionContent 
			*child = this,
			*calc = new ConsistencySolutionContent();
	
		//FIXME: re-use the old instances
		calc->consistent = child->consistent;
	
		return calc;
	}
	
	/***********************************\
	|* CLASS: Solution
	\***********************************/
	Solution::Solution(Operation operation, Solution *left, Solution *right)
	{
		CHECK0(operation == CrossJoin || operation == Union, "invalid solution operation specified");
	
		this->operation = operation;
		this->leftArgument = left;
		this->rightArgument = right;
	}
	
	Solution::Solution(Solution *child, const VertexSet &toAdd)
	{
		this->operation = Add;
		this->parameter = toAdd;
		this->leftArgument = child;
	}

	Solution::Solution(const VertexSet &partition)
	{
		this->operation = NewLeaf;
		this->parameter = partition;
	}
	
	Solution::Solution(SolutionContent *content) 
	{
		this->operation = Value;
		this->content = content;
	}

	Solution::Solution()
	{
		this->operation = NewEmpty;
	}
	
	Solution::~Solution() 
	{	
		if(this->content) delete this->content;
		if(this->leftArgument) delete this->leftArgument;
		if(this->rightArgument)	delete this->rightArgument;
	}

	SolutionContent *Solution::getContent(Instantiator *inst)
	{
		switch(this->operation)
		{
		case CrossJoin:
			this->content = this->leftArgument->getContent(inst)->
				calculateCrossJoin(this->rightArgument->getContent(inst));
			break;
		case Union:
			this->content = this->leftArgument->getContent(inst)->
				calculateUnion(this->rightArgument->getContent(inst));
			break;
		case Add:
			this->content = this->leftArgument->getContent(inst)->
				calculateAdd(this->parameter);
			break;
		case Value:
			break;
		case NewLeaf:
			CHECKNULL(inst, "instantiator needed, but is NULL");
			this->content = inst->createLeafSolution(this->parameter);
			break;
		case NewEmpty:
			CHECKNULL(inst, "instantiator needed, but is NULL");
			this->content = inst->createEmptySolution();
			break;
		default:
			CHECK0(0, "invalid Solution operation specified");
			break;
		}

		this->operation = Value;
		return this->content;
	}
	
	/***********************************\
	|* CLASS: Instantiator
	\***********************************/
	Instantiator::Instantiator() { }
	
	Instantiator::~Instantiator() { }
	
	/***********************************\
	|* CLASS: AbstractHTDAlgorithm
	\***********************************/
	AbstractHTDAlgorithm::AbstractHTDAlgorithm(Problem *problem)
	{
		this->inst = NULL;
		this->prob = problem;
	}
	
	AbstractHTDAlgorithm::~AbstractHTDAlgorithm() 
	{ 
	}
	
	Solution *AbstractHTDAlgorithm::evaluate(ExtendedHypertree *origroot, Instantiator *inst)
	{
		this->inst = inst;
		ExtendedHypertree *root = this->prepareHypertreeDecomposition(origroot);
		return selectSolution(evaluateNode(root), root);
	}

	Problem *AbstractHTDAlgorithm::problem()
	{
		return this->prob;
	}

	ExtendedHypertree *AbstractHTDAlgorithm::prepareHypertreeDecomposition(ExtendedHypertree *root)
	{
		return root;
	}

	Solution *AbstractHTDAlgorithm::createEmptySolution()
	{
		if(inst) return new Solution(inst->createEmptySolution());
		return new Solution();
	}

	Solution *AbstractHTDAlgorithm::createLeafSolution(const VertexSet &partition)
	{
		if(inst) return new Solution(inst->createLeafSolution(partition));
		return new Solution(partition);
	}

	Solution *AbstractHTDAlgorithm::combineSolutions(Operation operation, Solution *left, Solution *right)
	{
		return new Solution(operation, left, right);
	}

	Solution *AbstractHTDAlgorithm::addToSolution(Solution *solution, const VertexSet &toAdd)
	{
		return new Solution(solution, toAdd);
	}

	Solution *AbstractHTDAlgorithm::addToSolution(Solution *solution, Vertex toAdd)
	{
		VertexSet v; v.insert(toAdd);
		return this->addToSolution(solution, v);
	}

	void AbstractHTDAlgorithm::addToTupleSet(Tuple *t, Solution *s, TupleSet *ts, Operation op)
	{
		// try to insert the tuple into the tuple set
		pair<TupleSet::iterator, bool> result = ts->insert(TupleSet::value_type(t, s));
	
		// if the tuple was already in the set
		if(!result.second)
		{
			// delete it and insert it again with combined solution
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(t, combineSolutions(op, orig, s)));
		}
	}

	/*********************************************\
	|* CLASS: AbstractSemiNormalizedHTDAlgorithm
	\*********************************************/
	AbstractSemiNormalizedHTDAlgorithm::AbstractSemiNormalizedHTDAlgorithm(Problem *problem)
		: AbstractHTDAlgorithm(problem)
	{ }

	AbstractSemiNormalizedHTDAlgorithm::~AbstractSemiNormalizedHTDAlgorithm() { }

	ExtendedHypertree *AbstractSemiNormalizedHTDAlgorithm::prepareHypertreeDecomposition(ExtendedHypertree *root)
	{
		return root->normalize(SemiNormalization);
	}
	
	TupleSet *AbstractSemiNormalizedHTDAlgorithm::evaluateNode(const ExtendedHypertree *node)
	{
		switch(node->getType())
	        {
	        case Branch: 
			return this->evaluateBranchNode(node);
		case Permutation:
		case Introduction:
	        case Removal: 
		case Leaf:
			return this->evaluatePermutationNode(node);
		default:
			PrintError("invalid node type, check normalization", "");
			return NULL;
	        }
	}

	/*********************************************\
	|* CLASS: AbstractNormalizedHTDAlgorithm
	\*********************************************/
	AbstractNormalizedHTDAlgorithm::AbstractNormalizedHTDAlgorithm(Problem *problem)
		: AbstractSemiNormalizedHTDAlgorithm(problem)
	{ }

	AbstractNormalizedHTDAlgorithm::~AbstractNormalizedHTDAlgorithm() { }

	ExtendedHypertree *AbstractNormalizedHTDAlgorithm::prepareHypertreeDecomposition(ExtendedHypertree *root)
	{
		return root->normalize(DefaultNormalization);
	}

	TupleSet *AbstractNormalizedHTDAlgorithm::evaluatePermutationNode(const ExtendedHypertree *node)
	{
		switch(node->getType())
		{
		case Introduction:
			return this->evaluateIntroductionNode(node);
		case Removal:
			return this->evaluateRemovalNode(node);
		case Leaf:
			return this->evaluateLeafNode(node);
		default:
			PrintError("invalid node type, check normalization", "");
			return NULL;
		}
	}
} // namespace sharp
