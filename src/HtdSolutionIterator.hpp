#ifndef SHARP_HTDSOLUTIONITERATOR_H_
#define SHARP_HTDSOLUTIONITERATOR_H_

#include <libsharp/IHtdAlgorithm.hpp>
#include <libsharp/IInstance.hpp>
#include <libsharp/ISolution.hpp>
#include <libsharp/IIterativeSolutionBuilder.hpp>

namespace sharp
{
	class HtdSolutionIterator : public ISolutionIterator
	{
	public:
		~HtdSolutionIterator() override;

		ISolution *next() override;

	private:

	}; // struct HtdSolutionIterator

} // namespace sharp

#endif // SHARP_HTDSOLUTIONITERATOR_H_
