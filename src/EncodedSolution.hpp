#ifndef SHARP_ENCODEDSOLUTION_H_
#define SHARP_ENCODEDSOLUTION_H_

#include <libsharp/EncodedSolution.hpp>

#include "HtdSolutionTree.hpp"

#include <vector>

namespace sharp
{
	struct EncodedSolution::Impl
	{
		std::vector<HtdSolutionTree *> trees;
	
	}; // struct EncodedSolution::Impl

} // namespace sharp

#endif // SHARP_ENCODEDSOLUTION_H_
