#ifndef _WCC_UNIT_
#define _WCC_UNIT_

#include "Unit.h"
#include "Update.h"
#include <vector>

class ParallelEngine;
//弱连通子图算法单元
class WccUnit:public Unit{
private:
	enum {UPDATE_BUF_SIZE = 1000000};
	
	typedef struct{
		vertex_t label;
	}array_t;

	typedef struct 
	{
		vertex_t id;
    	vertex_t value;
	}update_t;
	
	std::vector<array_t>* _aux;
	Update<update_t>* update_buf;

public:
	WccUnit(ParallelEngine* engine);

	~WccUnit();

	void initStep();

	void scatter();

	void gather();

	void endScatter();
	void output();

	void reset();
};

#endif

