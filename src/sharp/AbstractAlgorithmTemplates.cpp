/***********************************\
|* CLASS: GenericInstantiator
\***********************************/
template<class TSolution>
GenericInstantiator<TSolution>::GenericInstantiator(bool lazy) 
{
	this->lazy = lazy;
}

template<class TSolution>
GenericInstantiator<TSolution>::~GenericInstantiator() { }

template<class TSolution>
TSolution *GenericInstantiator<TSolution>::createEmptySolution() const
{
	return new TSolution();
}

template<class TSolution>
TSolution *GenericInstantiator<TSolution>::createLeafSolution(const set<Variable> &partition) const
{
	return new TSolution(partition);
}

template<class TSolution>
TSolution *GenericInstantiator<TSolution>::combine(Operation operation, 
	Solution *left, Solution *right) const
{
	TSolution *s = new TSolution(operation, left, right);
	if(!lazy) s->forceCalculation();
	return s;
}

template<class TSolution>
TSolution *GenericInstantiator<TSolution>::addDifference(Solution *child, 
	int difference) const
{
	TSolution *s = new TSolution(child, difference);
	if(!lazy) s->forceCalculation();
	return s;
}

