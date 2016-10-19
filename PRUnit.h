#ifndef _PR_UNIT_
#define _PR_UNIT_

#include "Unit.h"
#include "Update.h"
#include <vector>

class ParallelEngine;
//PageRankËã·¨µ¥Ôª
class PRUnit:public Unit{
private:
	enum {UPDATE_BUF_SIZE = 1000000};

	typedef struct{
	    int degree;
	    weight_t res;
		weight_t tmp;
	}array_t;

	typedef struct{
		vertex_t id;
	    weight_t value;
	}update_t;
	
	array_t init_array;
	weight_t init_wgt;
	int edge_size;
	int flag;
	vertex_t updated_num;
	std::vector<bool>* update_bitset;
	std::vector<array_t>* _aux;
	Update<update_t>* update_buf;
	
public:
	PRUnit(ParallelEngine* engine);

	~PRUnit();

	void initStep();

	void scatter();

	void gather();

	void endScatter();

	void output();

	void reset();
	
};

#endif