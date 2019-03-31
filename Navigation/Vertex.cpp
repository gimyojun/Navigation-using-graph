#include "Vertex.h"

Vertex::Vertex()//constructor
{
	m_key = 0;//initialize members
	m_size = 0;
	m_pEHead = NULL;
	m_pNext = NULL;
}

Vertex::Vertex(int key)//key initialize constructor
{
	m_key = key;//initialize members
	m_size = 0;
	m_pEHead = NULL;
	m_pNext = NULL;
}

Vertex::~Vertex()//destructor
{
	Clear();//execute Clear
}

void Vertex::SetNext(Vertex * pNext)//next vertex set function
{
	m_pNext = pNext;
}

int Vertex::GetKey() const//key return function
{
	return m_key;
}

Vertex * Vertex::GetNext() const//next vertex return function
{
	return m_pNext;
}

int Vertex::Size() const//vertex size return function
{
	return m_size;
}

void Vertex::AddEdge(int edgeKey, int weight)//edge add function
{
	Edge *newpE = new Edge(edgeKey, weight);//make new edge

	if (!m_pEHead) m_pEHead = newpE;//if edge doesn't exist
	else
	{
		Edge *pE = m_pEHead;//start with edge head
		while (pE->GetNext()) pE = pE->GetNext();//go to last edge
		pE->SetNext(newpE);//set next
	}
	m_size++;//size addition
}

Edge * Vertex::GetHeadOfEdge() const//edge head return function
{
	return m_pEHead;
}

void Vertex::Clear()//edge memory deallocation function
{
	Edge *pE = m_pEHead;//start with edge head
	Edge *X = NULL;//deleter
	while (pE)//explore edge
	{
		X = pE;
		pE = pE->GetNext();
		delete X;//edge memory deallocation
	}
}
