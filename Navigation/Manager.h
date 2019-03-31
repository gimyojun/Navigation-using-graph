#ifndef SOLUTION_H
#define SOLUTION_H

#include "Result.h"
#include "Graph.h"
#include <fstream>

class Manager
{
private:
	// the filepath for the result log
	const char* RESULT_LOG_PATH = "log.txt";
	// the filepath for the error log
	const char* ERROR_LOG_PATH = "log.txt";


	

	std::ofstream fout;
	// the file stream for the error log
	std::ofstream ferr;
	// graph instance to manage the vertics.
	Graph m_graph;

public:
	Manager();
	~Manager();
	void Run(const char* filepath);//command run function
	void PrintError(Result result);//error out function

private:
	Result Load(const char* filepath);//graph making function
	Result Print();//graph print function
	Result FindPathDfs(int startVertexKey, int endVertexKey);//DFS function
	Result FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey);//Dijkstra using set function
	Result FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey);//Dijkstra using minheap function
	Result FindShortestPathBellmanFord(int startVertexKey, int endVertexKey);//BellmanFord function
	Result FindShortestPathFloyd();
};

#endif