#include <gmp.h>
#include <gmpxx.h>

#include "SharpSAT.h"

//////////////////////////////////////////////
//HELPERS
//////////////////////////////////////////////

/*static mpz_class eval(ExtendedHypertree *root)
{
	return NULL;
}*/

//////////////////////////////////////////////

SharpSAT::SharpSAT(ExtendedHypertree *root)
{
	this->root = root;
}

SharpSAT::~SharpSAT()
{
}

string SharpSAT::evaluate()
{
	//return eval(this->root).get_str();		
	mpz_class b = 2;
	return b.get_str();
}
