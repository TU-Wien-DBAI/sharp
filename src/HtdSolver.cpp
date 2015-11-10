#include "HtdSolver.hpp"

#include <stack>
#include <utility>

namespace sharp
{
	using std::unique_ptr;
	using std::stack;
	using std::pair;
	using std::make_pair;
	using htd::UNKNOWN_VERTEX;
	using htd::vertex_t;
	using htd::HypertreeDecompositionAlgorithmImpl;
	using htd::MinFillOrderingAlgorithmImpl;
	using htd::IHypergraph;
	using htd::IHypertreeDecomposition;

	HtdSolver::Impl::Impl(const IHtdNodeEvaluator &evaluator)
		: evaluator(evaluator)
	{ }

	HtdSolver::HtdSolver(const IHtdNodeEvaluator &evaluator)
	{
		this->impl = new Impl(evaluator);
	}

	HtdSolver::~HtdSolver()
	{
		delete this->impl;
	}

	void HtdSolver::solve(const IInstance &instance)
		const
	{
		//TODO: parameterize this
		HypertreeDecompositionAlgorithmImpl decomposer;
		MinFillOrderingAlgorithmImpl ordering;

		unique_ptr<IHypergraph> hg = instance.toHypergraph();

		unique_ptr<IHypertreeDecomposition> htd =
			decomposer.computeDecomposition(hg, ordering);

		vertex_t current = htd->getRoot();
		stack<pair<vertex_t, int> > parents;

		while(!parents.empty() || current != UNKNOWN_VERTEX)
		{
			if(current != UNKNOWN_VERTEX)
			{
				parents.push(make_pair(current, 1));
				current = htd->childAt(current, 0);
				continue;
			}

			pair<vertex_t, int> top = parents.top();
			current = htd->childAt(top.first, top.second);
			parents.pop();

			if(current != UNKNOWN_VERTEX)
			{
				parents.push(make_pair(top.first,
							top.second + 1));
				continue;
			}

			// TODO: visit node
		}
	}

} // namespace sharp
