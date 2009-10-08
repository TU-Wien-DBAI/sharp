#include <iostream>

#include "../support/support.h"

#include "LazySolutionSet.h"

LazySolutionSet::LazySolutionSet(LazySolutionSetType type, LazySolutionSet& left, LazySolutionSet &right)
{
	C0((type == LazyCrossJoin || type == LazyUnion));

	this->type = type;
	this->leftArgument = &left;
	this->rightArgument = &right;
}

LazySolutionSet::LazySolutionSet(SolutionSet &value)
{
	this->type = Value;
	this->value = &value;
}

LazySolutionSet::LazySolutionSet(LazySolutionSet &original, int differenceVariable)
{
	this->type = LazyAddDifference;
	this->leftArgument = &original;
}

SolutionSet &LazySolutionSet::solutions()
{
	//TODO

	switch(this->type)
	{
		case LazyCrossJoin:
			this->type = Value;

			this->value = new SolutionSet();
			
			for(SolutionSet::iterator i = this->leftArgument->solutions().begin(); i != this->leftArgument->solutions().end(); ++i)
			{
				for(SolutionSet::iterator j = this->rightArgument->solutions().begin(); j != this->rightArgument->solutions().end(); ++j)
				{
					Solution ns = *i;
					ns.insert(j->begin(), j->end());
					this->value->insert(ns);
				}
			}

			break;

		case LazyUnion:
			this->type = Value;

			this->value = &this->leftArgument->solutions();
			this->value->insert(this->rightArgument->solutions().begin(), this->rightArgument->solutions().end());			

			break;

		case LazyAddDifference:
			this->type = Value;

			this->value = new SolutionSet();

			for(SolutionSet::iterator i = this->leftArgument->solutions().begin(); i != this->leftArgument->solutions().end(); ++i)
			{ 
				Solution temp(*i); 
				temp.insert(this->differenceVariable); 
				this->value->insert(temp);
			}
			
			break;

		default:
			break;
	}

	return *this->value;
}


