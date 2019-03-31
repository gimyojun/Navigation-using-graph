#include "Manager.h"
#include <cstring>
#pragma warning(disable:4996)
Manager::Manager() {
	fout.open(RESULT_LOG_PATH);

}
Manager::~Manager()
{
	if (fout.is_open())//result log file close
		fout.close();

}

void Manager::Run(const char* filepath)
{
	std::ifstream fin;
	fin.open(filepath);//command file open
	if (!fin)//Error: Command file not exist
	{
		Result result(CommandFileNotExist);//make enum by result
		fout << "======= SYSTEM =======" << std::endl
			<< "CommandFileNotExist" << std::endl
			<< "======================" << std::endl << std::endl;//print result
		PrintError(result);//print error code
		return;//exit function
	}

	char *Command = NULL;
	char* Para1, *Para2 = NULL;
	char C_name[128];
	while (!fin.eof()) {
		fin.getline(C_name, 128);
		if (!strcmp(C_name, ""))
			break;
		Command = strtok(C_name, " \r");
		Para1 = strtok(NULL, " \r");
		Para2 = strtok(NULL, " \r");
			Result result;

		if (!strcmp(Command, "LOAD")) {
			result = Load(Para1);
			PrintError(result);
		}
		else if (!strcmp(Command, "PRINT")) {
			result = Print();
			PrintError(result);
		}
		else if (!strcmp(Command, "DFS")) {
			if (Para1 == NULL || Para2 == NULL) {
				fout << "========= DFS ==========" <<std:: endl;
				fout << "VertexKeyNotExist" << std::endl;
				fout << "=======================" << std::endl << std::endl;
				PrintError(VertexKeyNotExist);
			}
			else {
				int a = atoi(Para1);
				int b = atoi(Para2);
				result = FindPathDfs(a, b);
				PrintError(result);
			}
		}
		else if (!strcmp(Command, "DIJKSTRA")) {
			if (Para1 == NULL || Para2 == NULL) {
				fout << "========= DIJKSTRA ==========" << std::endl;
				fout << "VertexKeyNotExist" << std::endl;
				fout << "=======================" << std::endl << std::endl;
				PrintError(VertexKeyNotExist);
			}
			else {

				int a = atoi(Para1);
				int b = atoi(Para2);
				result = FindShortestPathDijkstraUsingSet(a, b);
				PrintError(result);
			}
		}
		else if (!strcmp(Command, "DIJKSTRAMIN")) {
			if (Para1 == NULL || Para2 == NULL) {
				fout << "========= DIJKSTRAMIN ==========" << std::endl;
				fout << "VertexKeyNotExist" << std::endl;
				fout << "=======================" << std::endl << std::endl;
				PrintError(VertexKeyNotExist);
			}
			else {

				int a = atoi(Para1);
				int b = atoi(Para2);
				result = FindShortestPathDijkstraUsingMinHeap(a, b);
				PrintError(result);
			}
		}
		else if (!strcmp(Command, "BELLMANFORD")) {
			if (Para1 == NULL || Para2 == NULL) {
				fout << "========= BELLMANFORD ==========" << std::endl;
				fout << "VertexKeyNotExist" << std::endl;
				fout << "=======================" << std::endl << std::endl;
				PrintError(VertexKeyNotExist);
			}
			else {
				int a = atoi(Para1);
				int b = atoi(Para2);
				result = FindShortestPathBellmanFord(a, b);
				PrintError(result);
			}
		}
		else if (!strcmp(Command, "FLOYD")) {
			FindShortestPathFloyd();
		
		}
		else { //exception case
			fout << "======= " << Command << " =======" << std::endl
				<< "NonDefinedCommand" << std::endl
				<< "=====================" << std::endl << std::endl;
			PrintError(NonDefinedCommand);//print error code
		}

	}
	fin.close();
}
void Manager::PrintError(Result result)
{
	
	fout << "===============" <<std:: endl;
	fout << "Error code: " << result << std::endl;
	fout << "===============" << std::endl << std::endl;
	
}

/// <summary>
/// make a graph
/// </summary>
///
/// <param name="filepath">
/// the filepath to read to make the graph
/// </param>
///
/// <returns>
/// Result::Success if load is successful.
/// Result::LoadFileNotExist if file is not exist.
/// </returns>
Result Manager::Load(const char* filepath)
{
	std::ifstream fin;
	fin.open(filepath);
	Result result;
	if (!fin.is_open()) {
		fout << "====== LOAD ======" << std::endl << "LoadFileNotExist" << std::endl
			<< "=================" << std::endl << std::endl;
		result = LoadFileNotExist;
		return result;
	}
	else {
		char buf[512] = {};//text line receiver
		fin.getline(buf, sizeof(buf));//read first text line
		int size = atoi(buf);//get size info
		for (int i = 0; i < size; i++)//graph making loop
		{
			m_graph.AddVertex(i);//add vertex key of i
			fin.getline(buf, sizeof(buf));//read text line by line
			for (int j = 0; j < size; j++)//edge addition
			{
				char *tok = NULL;//token to get weight
				if (j == 0)tok = strtok(buf, " \r");//case j==0
				else tok = strtok(NULL, " \r");//case j!=0
				int wei = atoi(tok);//get weight
				if (wei != 0) m_graph.AddEdge(i, j, wei);//if weight!=0 add edge to vertex
			}
		}
		fin.close();
		fout << "======== LOAD  =======" << std::endl;
		fout << "Success" << std::endl;
		fout << "=====================" << std::endl << std::endl;
		result = Success;
		return result;
	}
}
/// <summary>
/// print out the graph as matrix form
/// </summary>
///
/// <returns>
/// Result::Success if the printing is successful
/// Result::GraphNotExist if there is no graph
/// </returns>
Result Manager::Print()
{
	if (!m_graph.Head())//Exception: Graph doesn't exist
	{
		fout << "======= PRINT =======" << std::endl
			<< "GraphNotExist" << std::endl
			<< "=====================" << std::endl << std::endl;
		return Result(GraphNotExist);//return error case
	}
	else
	{
		fout << "======= PRINT =======" << std::endl;
		m_graph.Print(fout);//Print function execute
		fout << "=====================" << std::endl << std::endl;
		return Result(Success);//return error case
	}
}
/// <summary>
/// find the path from startVertexKey to endVertexKey with DFS (stack)
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindPathDfs(int startVertexKey, int endVertexKey)
{
	if (!m_graph.Head())//Exception: Graph doesn't exist
	{
		fout << "======= DFS =======" << std::endl
			<< "GraphNotExist" << std::endl
			<< "===================" << std::endl << std::endl;
		return Result(GraphNotExist);//return error case
	}
	else if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)//Exception: vertex key doesn't exist
	{
		fout << "======= DFS =======" << std::endl
			<< "InvalidVertexKey" << std::endl
			<< "===================" << std::endl << std::endl;
		return Result(InvalidVertexKey);//return error case
	}
	else if (m_graph.IsNegativeEdge())//Exception: Negative edge exist
	{
		fout << "======= DFS =======" << std::endl
			<< "InvalidAlgorithm" << std::endl
			<< "===================" << std::endl << std::endl;
		return Result(InvalidAlgorithm);//return error case
	}
	else
	{
		std::vector<int> a = m_graph.FindPathDfs(startVertexKey, endVertexKey);//DFS execution
		std::vector<int> sort_a;
		std::vector<int> copy_a;
		fout << "======= DFS =======" << std::endl;
		fout << "shortest path: ";
		for (int i = 0; i < a.size() - 1; i++)
			fout << a.at(i) << " ";//print path and total distance
		fout << std::endl;

		for (int i = 0; i < a.size() - 1; i++)
			sort_a.push_back(a[i]);
		fout << "sorted nodes: ";
		copy_a = m_graph.QuickSort(sort_a, 0, sort_a.size() - 1);
		for (int i = 0; i < sort_a.size(); i++)
			fout << copy_a.at(i) << " ";//print path and total distance
		fout << std::endl;


		fout << "path length: " << a.at(a.size() - 1) << std::endl;
		fout << "===================" << std::endl;
		return Result(Success);//return error case
	}
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
	if (!m_graph.Head())//Exception: Graph doesn't exist
	{
		fout << "====== DIJKSTRA ======" << std::endl
			<< "GraphNotExist" << std::endl
			<< "======================" << std::endl << std::endl;
		return Result(GraphNotExist);//return error case
	}
	else if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)//Exception: vertex key doesn't exist
	{
		fout << "====== DIJKSTRA ======" << std::endl
			<< "InvalidVertexKey" << std::endl
			<< "======================" << std::endl << std::endl;
		return Result(InvalidVertexKey);//return error case
	}
	else if (m_graph.IsNegativeEdge())//Exception: Negative edge exist
	{
		fout << "====== DIJKSTRA ======" << std::endl
			<< "InvalidAlgorithm" << std::endl
			<< "======================" << std::endl << std::endl;
		return Result(InvalidAlgorithm);//return error case
	}
	else
	{
		std::vector<int> a = m_graph.FindShortestPathDijkstraUsingSet(startVertexKey, endVertexKey);//Dijkstra execution
		std::vector<int> sort_a;
		std::vector<int> copy_a;

		fout << "====== DIJKSTRA ======" << std::endl; fout << "shortest path: ";
		for (int i = 0; i < a.size() - 1; i++)
			fout << a.at(i) << " ";//print path and total distance
		fout << std::endl;

		for (int i = 0; i < a.size() - 1; i++)
			sort_a.push_back(a[i]);
		fout << "sorted nodes: ";
		copy_a = m_graph.QuickSort(sort_a, 0, sort_a.size() - 1);
		for (int i = 0; i < sort_a.size(); i++)
			fout << copy_a.at(i) << " ";//print path and total distance
		fout << std::endl;


		fout << "path length: " << a.at(a.size() - 1) << std::endl;
		fout << "===================" << std::endl;
		return Result(Success);//return error case
	}
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using MinHeap
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)
{
	if (!m_graph.Head())//Exception: Graph doesn't exist
	{
		fout << "====== DIJKSTRAMIN ======" << std::endl
			<< "GraphNotExist" << std::endl
			<< "=========================" << std::endl << std::endl;
		return Result(GraphNotExist);//return error case
	}
	else if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)//Exception: vertex key doesn't exist
	{
		fout << "====== DIJKSTRAMIN ======" << std::endl
			<< "InvalidVertexKey" << std::endl
			<< "=========================" << std::endl << std::endl;
		return Result(InvalidVertexKey);//return error case
	}
	else if (m_graph.IsNegativeEdge())//Exception: Negative edge exist
	{
		fout << "====== DIJKSTRAMIN ======" << std::endl
			<< "InvalidAlgorithm" << std::endl
			<< "=========================" << std::endl << std::endl;
		return Result(InvalidAlgorithm);//return error case
	}
	else
	{
		std::vector<int> a = m_graph.FindShortestPathDijkstraUsingMinHeap(startVertexKey, endVertexKey);//Dijkstra execution
		std::vector<int> sort_a;
		std::vector<int> copy_a;
		fout << "====== DIJKSTRAMIN ======" << std::endl; fout << "shortest path: ";
		for (int i = 0; i < a.size() - 1; i++)
			fout << a.at(i) << " ";//print path and total distance
		fout << std::endl;

		for (int i = 0; i < a.size() - 1; i++)
			sort_a.push_back(a[i]);
		fout << "sorted nodes: ";
		copy_a = m_graph.QuickSort(sort_a, 0, sort_a.size() - 1);
		for (int i = 0; i < sort_a.size(); i++)
			fout << copy_a.at(i) << " ";//print path and total distance
		fout << std::endl;
		
			
		

		fout << "path length: " << a.at(a.size() - 1) << std::endl;
		fout << "===================" << std::endl;
		return Result(Success);//return error case
	}
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
	if (!m_graph.Head())//Exception: Graph doesn't exist
	{
		fout << "====== BELLMANFORD ======" << std::endl
			<< "GraphNotExist" << std::endl
			<< "=========================" << std::endl << std::endl;
		return Result(GraphNotExist);//return error case
	}
	else if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)//Exception: vertex key doesn't exist
	{
		fout << "====== BELLMANFORD ======" << std::endl
			<< "InvalidVertexKey" << std::endl
			<< "=========================" << std::endl << std::endl;
		return Result(InvalidVertexKey);//return error case
	}
	else
	{
		std::vector<int> a = m_graph.FindShortestPathBellmanFord(startVertexKey, endVertexKey);//BellmanFord execution
		if (a.empty())//Exception: negative cycle exist
		{
			fout << "====== BELLMANFORD ======" << std::endl
				<< "NegativeCycleDetected" << std::endl
				<< "=========================" << std::endl << std::endl;
			return Result(NegativeCycleDetected);//return error case
		}
		else
		{
			std::vector<int> sort_a;
			std::vector<int> copy_a;
			fout << "====== BELLMANFORD ======" << std::endl; fout << "shortest path: ";
			for (int i = 0; i < a.size() - 1; i++)
				fout << a.at(i) << " ";//print path and total distance
			fout << std::endl;

			for (int i = 0; i < a.size() - 1; i++)
				sort_a.push_back(a[i]);
			fout << "sorted nodes: ";
			copy_a = m_graph.QuickSort(sort_a, 0, sort_a.size() - 1);
			for (int i = 0; i < sort_a.size(); i++)
				fout << copy_a.at(i) << " ";//print path and total distance
			fout << std::endl;


			fout << "path length: " << a.at(a.size() - 1) << std::endl;
			fout << "===================" << std::endl;
			return Result(Success);//return error case
		}
	}
}

Result Manager::FindShortestPathFloyd()
{
	if (!m_graph.Head())//Exception: Graph doesn't exist
	{
		fout << "====== FLOYD ======" << std::endl
			<< "GraphNotExist" << std::endl
			<< "=========================" << std::endl << std::endl;
		return Result(GraphNotExist);//return error case
	}
	else {

		std::vector<std::vector<int>>a;

		a = m_graph.FindShortestPathFloyd();

		if (a.size()==0) {
			fout << "====== FLOYD ======" << std::endl
				<< "NegativeCycleDetected" << std::endl
				<< "=========================" << std::endl << std::endl;
			return Result(NegativeCycleDetected);
		}
		else {
			fout << "====== FLOYD ======" << std::endl;
			for (int i = 0; i < a.size(); i++) {
				for (int j = 0; j < a.size(); j++)
					fout << a[i][j] << " ";
				fout << std::endl;
			}
			fout<< "=========================" << std::endl << std::endl;
		}

		return Result(Success);
	}
	

}
