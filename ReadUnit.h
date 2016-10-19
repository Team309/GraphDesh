#ifndef _READ_UNIT_
#define _READ_UNIT_

#include "Unit.h"
#include "Update.h"
#include <vector>

class ParallelEngine;
class ReadUnit:public Unit{
private:
	enum {UPDATE_BUF_SIZE = 1000000};	

	typedef struct 
	{
		vertex_t id;
	}update_t;

	Update<update_t>* update_buf;

public:
	ReadUnit(ParallelEngine* engine);

	~ReadUnit();

	void initStep();

	void scatter();

	void gather();

	void endScatter();
	void output();

	void reset();
};

#endif

