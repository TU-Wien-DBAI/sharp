#ifndef SHARPSAT_H_
#define SHARPSAT_H_

#include <string>

using namespace std;

#include "ExtendedHypertree.h"

class SharpSAT
{
public:
	SharpSAT(ExtendedHypertree *root);
	virtual ~SharpSAT();
	
	virtual string evaluate();

protected:
	ExtendedHypertree *root;
};

#endif /*SHARPSAT_H_*/
