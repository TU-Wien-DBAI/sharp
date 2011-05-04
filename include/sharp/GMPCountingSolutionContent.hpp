#ifndef COUNTINGSOLUTIONCONTENT_H
#define COUNTINGSOLUTIONCONTENT_H

#include <sharp/Global.hpp>

#include <sharp/AbstractAlgorithm.hpp>

#include <gmp.h>
#include <gmpxx.h>

namespace sharp
{
	class CountingSolutionContent : public SolutionContent
	{
	public:
		CountingSolutionContent();
		CountingSolutionContent(const VertexSet &partition);
		virtual ~CountingSolutionContent();
	
		virtual SolutionContent *calculateUnion(SolutionContent *other);
		virtual SolutionContent *calculateCrossJoin(SolutionContent *other);
		virtual SolutionContent *calculateAdd(const VertexSet &toAdd);
	
	public:
		mpz_class count;
	};
} // namespace sharp

#endif /*COUNTINGSOLUTIONCONTENT_H*/
