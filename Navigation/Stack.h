#ifndef STACK_H
#define STACK_H

#ifndef NULL
#define NULL    0
#endif

template <typename T>
class Stack
{
private:
	class StackNode//inner class
	{
	public:
		T Data;//data member
		StackNode* pNext;//next info

		StackNode(T data) : Data(data), pNext(NULL) {}//constructor, initialization
	};

private:
	StackNode* m_pTop;// the head pointer of the stack

public:
	Stack()
	{
		m_pTop = NULL;//initialize Top
	}
	~Stack()
	{
		while (1)
		{
			if (!m_pTop) return;
			else//stacknode memory deallocation
			{
				StackNode *X = m_pTop;
				m_pTop = m_pTop->pNext;
				delete X;
			}
		}
	}

	/// <summary>
	/// push the data into this stack
	/// </summary>
	///
	/// <param name="data">
	/// a data to push into this stack
	/// </param>
	void Push(T data)//push new data
	{
		StackNode *pN = new StackNode(data);//new node creation
		if (!m_pTop)
		{
			m_pTop = pN;//if stack is empty
		}
		else
		{
			pN->pNext = m_pTop;//set new node top
			m_pTop = pN;
		}
	}
	/// <summary>
	/// pop(remove) the last-in data from this stack
	/// </summary>
	void Pop()
	{
		if (!m_pTop) return;//if empty
		StackNode *X = m_pTop;
		m_pTop = m_pTop->pNext;//set top to next
		delete X;
	}
	/// <summary>
	/// get the last-in data of this stack
	/// </summary>
	/// 
	/// <returns>
	/// the last-in data of this stack
	/// </returns>
	T Top()
	{
		return m_pTop->Data;//return data
	}
	/// <summary>
	/// check whether this stack is empty or not.
	/// </summary>
	///
	/// <returns>
	/// true if this stack is empty.
	/// false otherwise.
	/// </returns>
	bool IsEmpty()
	{
		if (!m_pTop) return true;//stack empty
		else return false;//stack not empty
	}
};

#endif