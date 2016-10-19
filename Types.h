#ifndef _TYPES_
#define _TYPES_

#include<iostream>
#include<string>
#include<cstring>

typedef unsigned int vertex_t;
typedef struct {
	vertex_t src;
	vertex_t dst;
}edge_t;
typedef double weight_t;

//֧�ֵ��㷨��Ԫ
enum AlgorithmUnit {sssp, pr, wcc, bfs, read};
const std::string AlgorithmUnitName[] = {"sssp", "pr", "wcc", "bfs", "read"};
const int AlgorithmType = 5;

//���Ͷ�����
class Types{
public:
	static void readEdge(const char* buf, edge_t& edge){
		edge = *(edge_t* )buf;
	}

	static void writeEdge(char* buf, edge_t& edge){
		memcpy(buf,&edge,sizeof(edge_t));
	}
};

#endif