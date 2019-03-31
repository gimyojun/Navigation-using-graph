#ifndef EDGE_H
#define EDGE_H

#ifndef NULL
#define NULL	0
#endif
//#include<iostream>

class Edge
{
private:
	// the key of this edge
	int m_key;
	// the weight of this edge
	int m_weight;
	// the next pointer for the linked list of the edges
	Edge* m_pNext;

public:
	Edge();
	Edge(int key, int weight);

	/// <summary>
	/// set the next pointer of this edge
	/// </summary>
	///
	/// <param name="pNext">
	/// the next pointer of this edge
	/// </param>
	void SetNext(Edge* pNext);
	/// <summary>
	/// get the key of this edge
	/// </summary>
	///
	/// <returns>
	/// the key of this edge
	/// </returns>
	int GetKey() const;
	/// <summary>
	/// get the weight of this edge
	/// </summary>
	///
	/// <returns>
	/// the weight of this edge
	/// </returns>
	int GetWeight() const;
	/// <summary>
	/// get the next pointer of this edge
	/// </summary>
	///
	/// <returns>
	/// the next pointer of this edge
	/// </returns>
	Edge* GetNext() const;
};

#endif