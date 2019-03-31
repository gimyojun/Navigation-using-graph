#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <utility>
#include <vector>

template<typename TKey, typename TValue>
class MinHeap
{
private:
	// array for the elements which should be heap-sorted
	std::vector<std::pair<TKey, TValue>> m_vec;

public:
	MinHeap()
	{
	}

	/// <summary>
	/// insert key-value pair
	/// </summary>
	///
	/// <param name="key">
	/// the key that is used for sorting
	/// </param>
	///
	/// <param name="value">
	/// the value that is managed in this heap
	/// </param>
	void Push(TKey key, TValue value)//push function
	{
		m_vec.push_back(std::make_pair(key, value));//push into vector
		int i = m_vec.size() - 1;//vector end index
		std::pair<TKey, TValue> temp = m_vec.at(i);//going to upper

		while (i != 1 && m_vec[i / 2].first > temp.first)//position seeking
		{
			m_vec[i] = m_vec[i / 2];//value copy
			i = i / 2;//go to parent
		}
		m_vec[i] = temp;//insert into i position
	}
	/// <summary>
	/// remove the minimum element
	/// </summary>
	void Pop()
	{
		if (m_vec.size() <= 1)return;//if element doesn't exist
		Heapify(1);//heapify call
	}
	/// <summary>
	/// get the minimum element
	/// </summary>
	///
	/// <returns>
	/// the minimum element
	/// </returns>
	std::pair<TKey, TValue> Top()
	{
		return m_vec.at(1);//return minimum element
	}
	/// <summary>
	/// get the key-value pair which the value is the same as the target
	/// </summary>
	///
	/// <returns>
	/// the key-value pair which the value is the same as the target
	/// </returns>
	std::pair<TKey, TValue> Get(TValue target)
	{
		for (int i = 1; i < m_vec.size(); i++)//search target
		{
			if (m_vec[i].second == target) return m_vec[i];//return target
		}
	}
	/// <summary>
	/// check whether this heap is empty or not
	/// </summary>
	///
	/// <returns>
	/// true if this heap is empty
	/// </returns>
	bool IsEmpty()
	{
		if (m_vec.size() <= 1) return true;//element doesn't exist
		else return false;//element exist
	}
	/// <summary>
	/// change the key of the node which the value is the target.<para/>
	/// In general, the newKey should be smaller than the old key.<para/>
	/// </summary>
	///
	/// <parma name="target">
	/// the target to change the key
	/// </param>
	///
	/// <param name="newKey">
	/// new key for the target
	/// </param>
	void DecKey(TValue target, TKey newKey)
	{
		std::pair<TKey, TValue> temp;
		int i = 1;
		for (i = 1; i < m_vec.size(); i++)//searching target
		{
			if (m_vec[i].second == target)//if target found
			{
				m_vec[i].first = newKey;
				temp = m_vec[i];
				break;
			}
		}
		while (i != 1 && m_vec[i / 2].first > newKey)//position seeking
		{
			m_vec[i] = m_vec[i / 2];//value copy
			i = i / 2;//go to parent
		}
		m_vec[i] = temp;//insert into i position
	}

private:
	/// <summary>
	/// heap-sort, heapify.<para/>
	/// this function can be called recursively
	/// </summary>
	void Heapify(int index)
	{
		std::pair<TKey, TValue> lastE = m_vec.at(m_vec.size() - 1);//last element

		int currentNode = index;//heap explorer
		int child = index * 2;//child of currentNode
		while (child <= m_vec.size() - 1)//inner heap size
		{
			if (child < m_vec.size() - 1 && m_vec[child].first > m_vec[child + 1].first) child++;//set rightchild
			if (lastE.first <= m_vec[child].first) break;//can be inserted
			m_vec[currentNode] = m_vec[child];//copy from child
			currentNode = child;//one level down
			child = child * 2;//go to child
		}
		m_vec[currentNode] = lastE;//copy from last element
		m_vec.resize(m_vec.size() - 1);//size reduce
	}
};

#endif
