#ifndef LAZYSOLUTION_H_
#define LAZYSOLUTION_H_

using namespace std;

#include "../Global.h"

enum LazySolutionSetType
{
	Value = 1,
	LazyCrossJoin = 2,
	LazyUnion = 3,
	LazyAddDifference = 4
};

class LazySolutionSet
{
public:
	LazySolutionSet(LazySolutionSetType type, LazySolutionSet &left, LazySolutionSet &right);
	LazySolutionSet(SolutionSet &value);
	LazySolutionSet(LazySolutionSet &original, int differenceVariable);
	
	~LazySolutionSet();

	SolutionSet &solutions();

private:
	LazySolutionSetType type;

	SolutionSet *value;

	LazySolutionSet *leftArgument;
	LazySolutionSet *rightArgument;
	int differenceVariable;
};

#endif
