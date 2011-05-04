#include <config.h>

#include <ExtendedHypertree.hpp>

#include <Global.hpp>
#include <Node.hpp>
using namespace sharp;

#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;

ExtendedHypertree::ExtendedHypertree(Hypertree *node) : Hypertree()
{
	this->type = General;
	this->difference = VERTEXNOTFOUND;
	
	this->MyParent = node->getParent();
	this->MyChi = *node->getChi();

	for(list<Hypertree *>::iterator i = node->getChildren()->begin(); i != node->getChildren()->end(); ++i)
		this->insChild(*i);

	node->getChildren()->clear();

	if(dynamic_cast<ExtendedHypertree *>(node) == NULL)
	{
		for(set<Node *>::iterator i = node->getChi()->begin(); i != node->getChi()->end(); ++i)
		{
			this->vertices.insert((*i)->getName());
		}
	}
	else
	{
		this->vertices = ((ExtendedHypertree *)node)->vertices;
	}

	if(this->MyParent) { this->MyParent->remChild(node); this->MyParent->insChild(this); }

	delete node;
}

ExtendedHypertree::ExtendedHypertree(const VertexSet &vertices) : Hypertree()
{
	this->vertices = vertices;
	this->type = General;
	this->difference = VERTEXNOTFOUND;
}

ExtendedHypertree::~ExtendedHypertree() { }

ExtendedHypertree *ExtendedHypertree::createChild(ExtendedHypertree *child, VertexSet vertices)
{
	ExtendedHypertree *parent = child->parent();
	ExtendedHypertree *newChild = new ExtendedHypertree(vertices);

	parent->remChild(child);
	newChild->insChild(child);
	parent->insChild(newChild);

	parent->type = parent->calculateType();

	return child;
}

TreeNodeType ExtendedHypertree::calculateType()
{
	if(this->MyChildren.size() == 0) return Leaf;
	if(this->MyChildren.size() == 2) return Branch;

	ExtendedHypertree *child = this->firstChild();
	vector<Vertex> diff(1);

	if(child->vertices.size() != this->vertices.size() && containsAll(child->vertices, this->vertices))
	{
		set_difference(child->vertices.begin(), child->vertices.end(), this->vertices.begin(), this->vertices.end(), diff.begin());
		this->difference = diff[0];
		return Removal;
	}
	
	if(this->vertices.size() != child->vertices.size() && containsAll(this->vertices, child->vertices))
	{
		set_difference(this->vertices.begin(), this->vertices.end(), child->vertices.begin(), child->vertices.end(), diff.begin());
		this->difference = diff[0];
		return Introduction;
	}

	return General;
}

int ExtendedHypertree::getType() const
{
	return this->type;
}

Vertex ExtendedHypertree::getDifference() const
{
	return this->difference;
}

bool ExtendedHypertree::isRoot() const
{
	return this->MyParent == NULL;
}

ExtendedHypertree *ExtendedHypertree::normalize(NormalizationType normalization)
{
	if(this->MyChildren.size() > 1)
	{
		ExtendedHypertree *l = new ExtendedHypertree(this->vertices);
		ExtendedHypertree *r = new ExtendedHypertree(this->vertices);
		
		l->insChild(*this->MyChildren.begin()); this->remChild(*this->MyChildren.begin());
		for(list<Hypertree *>::iterator i = this->MyChildren.begin(); i != this->MyChildren.end(); ++i)
			r->insChild(*i);

		this->getChildren()->clear();
		this->insChild(l);
		this->insChild(r);

		l->normalize();
		r->normalize();
	}
	else if(this->MyChildren.size() == 1)
	{
		ExtendedHypertree *child = dynamic_cast<ExtendedHypertree *>(*this->MyChildren.begin());
		if(child == NULL) child = new ExtendedHypertree(*this->MyChildren.begin());
		
		child->adapt();
		child->parent()->type = child->parent()->calculateType();
		child->normalize();
	}

	this->type = this->calculateType();

	return this;
}

void ExtendedHypertree::adapt()
{
	if(this->MyParent == NULL) return;

	unsigned int changes;

	list<Vertex> currentVertices(this->parent()->vertices.begin(), this->parent()->vertices.end());

	vector<Vertex> redVertices(currentVertices.size());
	vector<Vertex> greenVertices(this->vertices.size());

	vector<Vertex>::iterator it
		= set_difference(this->parent()->vertices.begin(), this->parent()->vertices.end(), 
				this->vertices.begin(), this->vertices.end(), redVertices.begin());
	redVertices.resize(it - redVertices.begin());

	it = set_difference(this->vertices.begin(), this->vertices.end(), this->parent()->vertices.begin(),
		       		this->parent()->vertices.end(), greenVertices.begin());
	greenVertices.resize(it - greenVertices.begin());

	changes = redVertices.size() + greenVertices.size();

	for(it = redVertices.begin(); changes > 1 && it != redVertices.end(); ++it)
	{
		currentVertices.remove(*it);
		createChild(this, VertexSet(currentVertices.begin(), currentVertices.end()));
		--changes;
	}

	for(it = greenVertices.begin(); changes > 1 && it != greenVertices.end(); ++it)
	{
		currentVertices.push_back(*it);
		createChild(this, VertexSet(currentVertices.begin(), currentVertices.end()));
		--changes;
	}
}

ExtendedHypertree *ExtendedHypertree::parent() const
{
	return dynamic_cast<ExtendedHypertree *>(this->MyParent);
}

ExtendedHypertree *ExtendedHypertree::firstChild() const
{
	return dynamic_cast<ExtendedHypertree *>(*this->MyChildren.begin());
}

ExtendedHypertree *ExtendedHypertree::secondChild() const
{
	return dynamic_cast<ExtendedHypertree *>(*++this->MyChildren.begin());
}

const VertexSet &ExtendedHypertree::getVertices() const { return this->vertices; }


#ifdef DEBUG
////////// PRINTING FUNCTIONS
void ExtendedHypertree::print()
{
	ExtendedHypertree *eht = this;
        if(eht->getParent() == NULL) cout << "root: " << eht << endl;
        int type = eht->getType();

        cout << "node " << eht << ", parent = " << eht->getParent() << ", type = " << type << flush;
        if(type == Branch) cout << ", children: " << eht->firstChild() << " - " << eht->secondChild();
        else if(type != Leaf) cout << ", child: " << eht->firstChild();

        cout << ", vertices: ";
        for(VertexSet::const_iterator it = eht->getVertices().begin(); it != eht->getVertices().end(); ++it) cout << *it << ", ";
        cout << "difference: " << eht->getDifference() << ", ";
        cout << "END" << endl;

        if(type == Branch) { eht->firstChild()->print(); eht->secondChild()->print(); }
        else if(type == Leaf) {}
        else eht->firstChild()->print();
}

#endif

double ExtendedHypertree::getAverageWidth()
{
	return (double)getWidthSum()/getNumberOfDescendants();
}

unsigned int ExtendedHypertree::getWidthSum()
{
	unsigned int sum = MyChi.size();
	for(list<Hypertree *>::iterator i = MyChildren.begin(); i != MyChildren.end(); ++i)
		sum += ((ExtendedHypertree *)*i)->getWidthSum();
	return sum;
}

unsigned int ExtendedHypertree::getJoinNodeWidthSum()
{
	unsigned int sum = 0;
	if(getType() == Branch) sum += MyChi.size();
	for(list<Hypertree *>::iterator i = MyChildren.begin(); i != MyChildren.end(); ++i)
		sum += ((ExtendedHypertree *)*i)->getJoinNodeWidthSum();
	return sum;
}

double ExtendedHypertree::getAverageJoinNodeWidth()
{
	unsigned int count = getNumberOfJoinNodes();
	if(count == 0) return 0;
	return (double)getJoinNodeWidthSum()/getNumberOfJoinNodes();
}

double ExtendedHypertree::getJoinNodePercentage()
{
	return (double)getNumberOfJoinNodes()/getNumberOfDescendants();
}

unsigned int ExtendedHypertree::getNodeCountWithWidth(unsigned int width)
{
	unsigned int count = 0;
	if(MyChi.size() == width) ++count;

	for(list<Hypertree *>::iterator i = MyChildren.begin(); i != MyChildren.end(); ++i)
		count += ((ExtendedHypertree *)*i)->getNodeCountWithWidth(width);
	return count;
}

double ExtendedHypertree::getAverageJoinLeafDistance()
{
	return (double)getJoinLeafDistanceSum()/getNumberOfJoinNodes();
}

unsigned int ExtendedHypertree::getDistanceToNearestLeaf()
{
	if(getType() == Leaf) return 0;
	unsigned int ldist = firstChild()->getDistanceToNearestLeaf();
	if(getType() != Branch) return ldist;
	unsigned int rdist = secondChild()->getDistanceToNearestLeaf();
	return 1 + (ldist < rdist ? ldist : rdist);
}

unsigned int ExtendedHypertree::getJoinLeafDistanceSum()
{
	unsigned int sum = 0;
	if(getType() == Branch) sum += getDistanceToNearestLeaf();
	for(list<Hypertree *>::iterator i = MyChildren.begin(); i != MyChildren.end(); ++i)
		sum += ((ExtendedHypertree *)*i)->getJoinLeafDistanceSum();
	return sum;
}

double ExtendedHypertree::getAverageJoinJoinDistance()
{
	if(getNumberOfJoinNodes() <= 1) return 0;
	return (double)getJoinJoinDistanceSum()/(getNumberOfJoinNodes()-1);
}

unsigned int ExtendedHypertree::getDistanceToNearestJoinAncestor()
{
	ExtendedHypertree *p = parent();
	unsigned int i = 1;
	while(p) if(p->getType() == Branch) return i; else {++i; p = p->parent(); }
	return 0;
}

unsigned int ExtendedHypertree::getJoinJoinDistanceSum()
{
	unsigned int sum = 0;
	if(getType() == Branch) sum += getDistanceToNearestJoinAncestor();
	for(list<Hypertree *>::iterator i = MyChildren.begin(); i != MyChildren.end(); ++i)
		sum += ((ExtendedHypertree *)*i)->getJoinJoinDistanceSum();
	return sum;
}

unsigned int ExtendedHypertree::getNumberOfJoinNodes()
{
	unsigned int count = 0;
	if(getType() == Branch) ++count;

	for(list<Hypertree *>::iterator i = MyChildren.begin(); i != MyChildren.end(); ++i)
		count += (*i)->getNumberOfJoinNodes();
	return count;
}
