#ifndef _BFS_UNIT_
#define _BFS_UNIT_

#include "Unit.h"
#include "Update.h"
#include <vector>

class ParallelEngine;
//广度优先遍历算法单元
class BFSUnit:public Unit{
private:
	enum {UPDATE_BUF_SIZE = 1000000};
	
	typedef struct{
		bool visited;
	}array_t;

	typedef struct 
	{
		vertex_t id;
	}update_t;

	vertex_t _root;
	std::vector<array_t>* _aux;
	std::vector<vertex_t> upt_que; 
	Update<update_t>* update_buf;

public:
	BFSUnit(ParallelEngine* engine, int root);

	~BFSUnit();

	void initStep();

	void scatter();

	void gather();

	void endScatter();
	void output();

	void reset();
};

#endif

