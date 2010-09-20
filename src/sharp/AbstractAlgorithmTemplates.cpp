/***********************************\
|* CLASS: GenericInstantiator
\***********************************/
template<class TSolutionContent>
GenericInstantiator<TSolutionContent>::GenericInstantiator(bool lazy) 
	: Instantiator(lazy)
{
}

template<class TSolutionContent>
GenericInstantiator<TSolutionContent>::~GenericInstantiator() { }

template<class TSolutionContent>
Solution *GenericInstantiator<TSolutionContent>::createEmptySolution() const
{
	return new Solution(new TSolutionContent());
}

template<class TSolutionContent>
Solution *GenericInstantiator<TSolutionContent>::createLeafSolution(const VertexSet &partition) const
{
	return new Solution(new TSolutionContent(partition));
}

