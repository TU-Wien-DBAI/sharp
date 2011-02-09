#include <iostream>

#include <AbstractAlgorithm.hpp>

using namespace std;

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
	
	SolutionContent::SolutionContent(const VertexSet &partition) { }
	
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
	
	SolutionContent *EnumerationSolutionContent::calculateAddDifference(Vertex difference)
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
			sol.insert(difference); 
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
	
	SolutionContent *CountingSolutionContent::calculateAddDifference(Vertex difference)
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
		calc->consistent = left->consistent || right->consistent;
	
		return calc;
	}
	
	SolutionContent *ConsistencySolutionContent::calculateAddDifference(Vertex difference)
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
	
	Solution::Solution(SolutionContent *content) 
	{
		this->operation = Value;
		this->content = content;
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
	
	void Solution::forceCalculation()
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
	
	void Solution::calculateCrossJoin()
	{
		this->content = this->leftArgument->getContent()->calculateCrossJoin(this->rightArgument->getContent());
	}
	
	void Solution::calculateUnion()
	{
		this->content = this->leftArgument->getContent()->calculateUnion(this->rightArgument->getContent());
	}
	
	void Solution::calculateAddDifference()
	{
		this->content = this->leftArgument->getContent()->calculateAddDifference(this->difference);
	}
	
	/***********************************\
	|* CLASS: Instantiator
	\***********************************/
	Instantiator::Instantiator(bool lazy) { this->lazy = lazy; }
	
	Instantiator::~Instantiator() { }
	
	Solution *Instantiator::combine(Operation operation, 
			Solution *left, Solution *right) const
	{
		Solution *s = new Solution(operation, left, right);
		if(!lazy) s->forceCalculation();
		return s;
	}
	
	Solution *Instantiator::addDifference(Solution *child, 
			int difference) const
	{
		Solution *s = new Solution(child, difference);
		if(!lazy) s->forceCalculation();
		return s;
	}
	
	/***********************************\
	|* CLASS: AbstractAlgorithm
	\***********************************/
	AbstractAlgorithm::AbstractAlgorithm(Problem *problem)
	{
		this->instantiator = NULL;
		this->problem = problem;
	}
	
	AbstractAlgorithm::~AbstractAlgorithm() 
	{ 
	}
	
	void AbstractAlgorithm::setInstantiator(Instantiator *instantiator)
	{
		this->instantiator = instantiator;
	}
	
	Solution *AbstractAlgorithm::evaluate(const ExtendedHypertree *root)
	{
		CNULL(this->instantiator /*the instantiator has to be set*/);
		return selectSolution(evaluateNode(root), root);
	}
	
	TupleSet *AbstractAlgorithm::evaluateNode(const ExtendedHypertree *node)
	{
		switch(node->getType())
	        {
	        case Leaf: 
			return this->evaluateLeafNode(node);
	        case Branch: 
			return this->evaluateBranchNode(node);
	        case Removal: 
			return this->evaluateRemovalNode(node);
	        case Introduction: 
			return this->evaluateIntroductionNode(node);
		default:
			C0(0 /*invalid node type*/); return NULL;
	        }
	}

} // namespace sharp
