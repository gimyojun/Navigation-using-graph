#include "Edge.h"

Edge::Edge()//constructor
{
	m_key = 0;//initialization
	m_weight = 0;
	m_pNext = NULL;
}

Edge::Edge(int key, int weight)//key and weight initialize constructor
{
	m_key = key;//initialization
	m_weight = weight;
	m_pNext = NULL;
}

void Edge::SetNext(Edge * pNext)//next edge set function
{
	m_pNext = pNext;
}

int Edge::GetKey() const//key return function
{
	return m_key;
}

int Edge::GetWeight() const//weight return function
{
	return m_weight;
}

Edge * Edge::GetNext() const//next edge return function
{
	return m_pNext;
}
