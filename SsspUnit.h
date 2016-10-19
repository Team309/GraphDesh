#ifndef _SSSP_UNIT_
#define _SSSP_UNIT_

#include "Unit.h"
#include "Update.h"
#include <vector>

class ParallelEngine;
//单源点最短路径算法单元
class SsspUnit:public Unit{
private:
	enum {UPDATE_BUF_SIZE = 1000000};
	
	typedef struct{
		vertex_t dis;
	   	vertex_t pri;
	}array_t;

	typedef struct 
	{
		vertex_t id;
		vertex_t dis;
		vertex_t pri;
	}update_t;
	
	vertex_t _root;
	std::vector<array_t>* _aux;
	Update<update_t>* update_buf;

public:
	SsspUnit(ParallelEngine* engine, int root);

	~SsspUnit();

	void initStep();

	void scatter();

	void gather();

	void endScatter();

	void output();

	void reset();
	
};

#endif