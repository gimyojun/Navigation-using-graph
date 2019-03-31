#include "Graph.h"
#include "Stack.h"
#include "MinHeap.h"
#include <set>
#include <algorithm>
#define DFS_FIRST_PATH

Graph::Graph()//instructor
{
	m_pVHead = NULL;
	m_vSize = 0;
}
Graph::~Graph()//destructor
{
	Clear();//vertex memory deallocation
}

void Graph::AddVertex(int vertexKey)
{
	Vertex * newpV = new Vertex(vertexKey);//make new vertex

	if (!m_pVHead) m_pVHead = newpV;//vertex not exist
	else
	{
		Vertex *pV = m_pVHead;//start with head
		while (pV->GetNext()) pV = pV->GetNext();//go to last vertex
		pV->SetNext(newpV);//set next
	}
	m_vSize++;//size addition
}

void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight)
{
	Vertex * pV = FindVertex(startVertexKey);//find vertex
	pV->AddEdge(endVertexKey, weight);//edge addition
}

Vertex * Graph::FindVertex(int key)//vertex find function
{
	Vertex *pV = m_pVHead;//start with head
	while (pV)//explore vertex
	{
		if (pV->GetKey() == key) return pV;//vertex found
		pV = pV->GetNext();
	}
	return NULL;//not exist
}

Vertex * Graph::Head()//head return function
{
	return m_pVHead;
}

int Graph::Size() const//size return function
{
	return m_vSize;
}

void Graph::Clear()//vertex memory deallocation
{
	Vertex *pV = m_pVHead;//start with head
	Vertex *X = NULL;//deleter
	while (pV)//explore vertex
	{
		X = pV;
		pV = pV->GetNext();
		delete X;//memory deallocation
	}
}

void Graph::Print(std::ofstream & fout)
{
	Vertex *pV = m_pVHead;//start with head vertex
	for (int i = 0; i < m_vSize; i++)
	{
		if (pV)//vertex end check
		{
			if (pV->GetKey() == i)//edge existence check
			{
				Edge *pE = pV->GetHeadOfEdge();//start on head edge
				for (int j = 0; j < m_vSize; j++)
				{
					if (pE)//edge end check
					{
						if (pE->GetKey() == j)//weight existence check
						{
							fout << pE->GetWeight() << " ";//print weight
							pE = pE->GetNext();//go to next edge
						}
						else fout << "0 ";//weight is inf
					}
					else fout << "0 ";//edge end
				}
				pV = pV->GetNext();//go to next vertex
				fout << std::endl;
			}
			else//vertex has no edge
			{
				for (int j = 0; j < m_vSize; j++) fout << "0 ";//print full 0
				fout << std::endl;
			}
		}
		else//vertex end
		{
			for (int j = 0; j < m_vSize; j++) fout << "0 ";//print full 0
			fout << std::endl;
		}
	}
}

bool Graph::IsNegativeEdge()
{
	bool negative = false;//return value
	Vertex *pV = m_pVHead;//start with head vertex
	while (pV)//explore vertex
	{
		Edge *pE = pV->GetHeadOfEdge();//start with edge head
		while (pE)//explore edge
		{
			if (pE->GetWeight() < 0) negative = true;//if negative value found, set return value false 
			pE = pE->GetNext();//go next
		}
		pV = pV->GetNext();//go next
	}
	return negative;//value return
}

std::vector<int> Graph::FindPathDfs(int startVertexKey, int endVertexKey)//DFS function
{
	std::vector<int> Path;//define path vector
	int cost = 0;//total cost
	bool *Table = new bool[m_vSize];//visit table
	for (int i = 0; i < m_vSize; i++)Table[i] = false;//visit table initialization
	Stack<std::pair<int, int>> st;//stack define
	Vertex *pV = FindVertex(startVertexKey);//get startvertexkey vertex
	st.Push(std::make_pair(pV->GetKey(), pV->Size()));//first push
	while (1)//path find loop
	{

		std::pair<int, int> popped = st.Top();//get top of stack
		pV = FindVertex(popped.first);//get vertex from popped data key
		if (Path.empty())Path.push_back(pV->GetKey());//insert verkey into path array(when empty)
		else if (Path.at(Path.size() - 1) != pV->GetKey())Path.push_back(pV->GetKey());//insert verkey into path array(when not come back)
		Table[pV->GetKey()] = true;//visit table
		if (pV->GetKey() == endVertexKey)break;//if end vertex found, break loop
		if (Table[st.Top().first] == true && st.Top().second == 0)//if blocked path
		{
			st.Pop();//pop one data from stack
			Path.pop_back();//
			continue;//skip loop
		}

		popped.second = popped.second - 1;//if goable path, reduce case(second)
		st.Pop();
		st.Push(popped);//renew data

		Edge *pE = pV->GetHeadOfEdge();//explore edge of popped vertex
		while (pE)
		{
			if (Table[pE->GetKey()] == false)//if edge is non-visited path
			{
				pV = FindVertex(pE->GetKey());//edge to vertex
				Edge *pE2 = pV->GetHeadOfEdge();//edge of vertex explorer
				int minus = 0;//counter
				while (pE2)//explore edge of vertex
				{
					if (Table[pE2->GetKey()]) minus++;//count visited vertex
					pE2 = pE2->GetNext();//go to next
				}
				st.Push(std::make_pair(pV->GetKey(), pV->Size() - minus));//push key with possible path number
			}
			pE = pE->GetNext();//go to next
		}
	}
	for (int i = 0; i < Path.size() - 1; i++)//cost loop
	{
		Edge*pE = FindVertex(Path.at(i))->GetHeadOfEdge();//get vertex to get weight
		while (pE)
		{
			if (pE->GetKey() == Path.at(i + 1))break;//get weight to next path
			pE = pE->GetNext();//go to next
		}
		cost += pE->GetWeight();//cost addition
	}
	Path.push_back(cost);//push cost sum to print
	return Path;//return path
}

std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
	std::set<std::pair<int, int>> distance;//define set
	for (int i = 0; i < m_vSize; i++)//initialize loop
	{
		if (i == startVertexKey)distance.insert(std::make_pair(0, i));
		else distance.insert(std::make_pair(999999, i));
	}
	int *distarr = new int[m_vSize];//distance array
	for (int i = 0; i < m_vSize; i++)distarr[i] = 999999;//distance array initailize
	distarr[startVertexKey] = 0;//starting point zero

	bool *visit = new bool[m_vSize];//visit table
	for (int i = 0; i < m_vSize; i++)visit[i] = false;//initialize

	int *path = new int[m_vSize];//path array
	for (int i = 0; i < m_vSize; i++)path[i] = -1;//initialize

	while (1)
	{
		if (distance.empty()) break;//if visit all vertex, break
		Vertex *pV = FindVertex(distance.begin()->second);//get smallest distance vertex
		distance.erase(distance.begin());//erase from set
		visit[pV->GetKey()] = true;//visit mark
		Edge *pE = pV->GetHeadOfEdge();//edge explorer
		while (pE)//update information
		{
			if (visit[pE->GetKey()] == 0 && distarr[pE->GetKey()] > distarr[pV->GetKey()] + pE->GetWeight())//if updated value is smaller
			{
				path[pE->GetKey()] = pV->GetKey();//path mark
				distarr[pE->GetKey()] = distarr[pV->GetKey()] + pE->GetWeight();//distance array update
				std::set<std::pair<int, int>>::iterator it;//define iterator
				for (it = distance.begin(); it != distance.end(); it++) if (it->second == pE->GetKey()) break;//finding key
				distance.erase(it);//erase previous value
				distance.insert(std::make_pair(distarr[pV->GetKey()] + pE->GetWeight(), pE->GetKey()));//insert updated distance
			}
			pE = pE->GetNext();//go to next edge
		}
	}
	std::vector<int> finalpath;//finalpath vector
	int i = endVertexKey;//start with endvertex
	while (1)
	{
		finalpath.push_back(i);//push path into vector
		if (i == startVertexKey) break;//endvertex to startvertex
		i = path[i];//follow path
	}
	std::reverse(finalpath.begin(), finalpath.end());//reverse vector
	finalpath.push_back(distarr[endVertexKey]);//push distance into vector
	return finalpath;//return path
}

std::vector<int> Graph::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)
{
	MinHeap<int, int> distance;//define set
	distance.Push(-1, 0);
	for (int i = 0; i < m_vSize; i++)//initialize loop
	{
		if (i == startVertexKey)distance.Push(0, i);
		else distance.Push(999999, i);
	}
	int *distarr = new int[m_vSize];//distance array
	for (int i = 0; i < m_vSize; i++)distarr[i] = 999999;//distance array initailize
	distarr[startVertexKey] = 0;//starting point zero

	bool *visit = new bool[m_vSize];//visit table
	for (int i = 0; i < m_vSize; i++)visit[i] = false;//initialize

	int *path = new int[m_vSize];//path array
	for (int i = 0; i < m_vSize; i++)path[i] = -1;//initialize

	while (1)
	{
		if (distance.IsEmpty()) break;//if visit all vertex, break
		Vertex *pV = FindVertex(distance.Top().second);//get smallest distance vertex
		distance.Pop();//erase from set
		visit[pV->GetKey()] = true;//visit mark
		Edge *pE = pV->GetHeadOfEdge();//edge explorer
		while (pE)//update information
		{
			if (visit[pE->GetKey()] == 0 && distarr[pE->GetKey()] > distarr[pV->GetKey()] + pE->GetWeight())//if updated value is smaller
			{
				path[pE->GetKey()] = pV->GetKey();//path mark
				distarr[pE->GetKey()] = distarr[pV->GetKey()] + pE->GetWeight();//distance array update
				distance.DecKey(pE->GetKey(), distarr[pV->GetKey()] + pE->GetWeight());
			}
			pE = pE->GetNext();//go to next edge
		}
	}
	std::vector<int> finalpath;//finalpath vector
	int i = endVertexKey;//start with endvertex
	while (1)
	{
		finalpath.push_back(i);//push path into vector
		if (i == startVertexKey) break;//endvertex to startvertex
		i = path[i];//follow path
	}
	std::reverse(finalpath.begin(), finalpath.end());//reverse vector
	finalpath.push_back(distarr[endVertexKey]);//push distance into vector
	return finalpath;//return path
}

std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
	int *distarr = new int[m_vSize];//distance arr 
	for (int i = 0; i < m_vSize; i++)distarr[i] = 999999;//distance arr
	distarr[startVertexKey] = 0;//initialize

	int *path = new int[m_vSize];//path array
	for (int i = 0; i < m_vSize; i++)path[i] = -1;//path initialize

	for (int i = 0; i < m_vSize; i++)//Bellman-ford algorithm loop
	{
		for (int j = 0; j < m_vSize; j++)//inner loop
		{
			Vertex *pV = FindVertex(j);//explore each vertex
			Edge *pE = pV->GetHeadOfEdge();//edge explorer
			while (pE)
			{
				if (distarr[pE->GetKey()] > distarr[pV->GetKey()] + pE->GetWeight())//if updated distance is shorter
				{
					path[pE->GetKey()] = pV->GetKey();//path mark
					distarr[pE->GetKey()] = distarr[pV->GetKey()] + pE->GetWeight();//distance update
				}
				pE = pE->GetNext();//go to next edge
			}
		}
	}
	bool change = false;//change checker
	for (int j = 0; j < m_vSize; j++)//last check, same algorithm once
	{
		Vertex *pV = FindVertex(j);
		Edge *pE = pV->GetHeadOfEdge();
		while (pE)
		{
			if (distarr[pE->GetKey()] > distarr[pV->GetKey()] + pE->GetWeight())
			{
				path[pE->GetKey()] = pV->GetKey();
				distarr[pE->GetKey()] = distarr[pV->GetKey()] + pE->GetWeight();
				change = true;//if distance changed, flag on
			}
			pE = pE->GetNext();
		}
	}
	if (change == true) return std::vector<int>();//Exception: negative edge exist
	std::vector<int> finalpath;//finalpath vector
	int i = endVertexKey;//start with endvertex
	while (1)
	{
		finalpath.push_back(i);//push path into vector
		if (i == startVertexKey) break;//endvertex to startvertex
		i = path[i];//follow path
	}
	std::reverse(finalpath.begin(), finalpath.end());//reverse vector
	finalpath.push_back(distarr[endVertexKey]);//push distance into vector
	return finalpath;//return path
}

std::vector<std::vector<int>> Graph::FindShortestPathFloyd()
{
	std::vector<std::vector<int>> v;
	for (int i = 0; i < m_vSize; i++) {
		std::vector<int>in_v(m_vSize);
		v.push_back(in_v);
	}
	for (int i = 0; i < m_vSize; i++) {
		for (int j = 0; j < m_vSize; j++) {
			if (i == j) {
				v[i][j] = 0;
			}
			else 
				v[i][j] = 999999;
		}
	}
	Vertex* V_temp = m_pVHead;
	Edge* E_temp;
	int i_v = 0; 
	while (V_temp) {
		E_temp = V_temp->GetHeadOfEdge();
		while(E_temp) {
			v[i_v][E_temp->GetKey()] = E_temp->GetWeight();
			E_temp = E_temp->GetNext();
		}
		V_temp = V_temp->GetNext();
		i_v++;
	}
	
	for (int k = 0; k < m_vSize; k++) {
		for (int i = 0; i < m_vSize; i++) {
			for (int j = 0; j < m_vSize; j++) {
				if (v[i][k] + v[k][j] < v[i][j]) {
					v[i][j] = v[i][k] + v[k][j];
				}
			}
		}
	}
	for (int i = 0; i < m_vSize; i++) {
		for (int j = 0; j < m_vSize; j++) {
			if (i == j && v[i][j] < 0) {
				std::vector<std::vector<int>> null_vector;
				return null_vector;
			}
		}
	}
	return v;
}

void Graph::InsertionSort(std::vector<int>& v,int low,int high)
{
	if (low < high) { 
		for (int i = (low + 1); i <= high; i++)
		{
			int key = v[i]; 
			int j = i - 1;
			while (j > 0 && key < v[j])
			{
				v[j + 1] = v[j];
				j--;
			}
			// insert key
			v[j + 1] = key;
		}
	}

}

std::vector<int> Graph::QuickSort(std::vector<int>& arr, int left, int right)
{
	if (left < right)
	{
		if (right - left + 1 <= 6)
			InsertionSort(arr,left ,right);
		else {
			int mid = pivot(arr, left, right);
			QuickSort(arr, left, mid - 1);
			QuickSort(arr, mid + 1, right);
		}
	}
	return arr;
}

int Graph::pivot(std::vector<int>& arr, int left, int right)
{
	int p_value = arr[left];
	int p_index = left;

	for (int i = left + 1; i <= right; i++)
	{
		if (arr[i] < p_value)
		{
			p_index++;
			swap(arr, i, p_index);
		}
	}
	swap(arr, left, p_index);

	return p_index;
}

void Graph::swap(std::vector<int>& v, int x, int y)
{
	int temp = v[x];
	v[x] = v[y];
	v[y] = temp;
}
