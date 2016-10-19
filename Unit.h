#ifndef _UNIT_
#define _UNIT_

#include "Types.h"

class ParallelEngine;

//任务单元类
//虚基类，需要算法单元继承补充接口函数
class Unit{
protected:
	ParallelEngine* _engine;
	int _vertex_num;
	int _edge_size;
	int _offset;
	//图数据缓存偏移量
	int _readed_bytes;
	//缓存的有效图数据大小
	bool _scatter_end;
	bool _gather_end;
	bool _inited;
	//是否需要初始化

	char* buf;

public:
	enum {UNIT_BUF_SIZE = 800000};
	
	Unit(ParallelEngine* engine);

	void setScaterEnd(){
		_scatter_end = true;
	}
		
	bool isGatherEnd(){
		return _gather_end;
	}

	bool isInited(){
		return _inited;
	}

	bool getNextEdge(edge_t& edge);

	virtual void initStep() = 0;
	//初始化需要的工作
	virtual void scatter() = 0;
	//scatter阶段接口
	virtual void gather() = 0;
	//gather阶段接口
	virtual void endScatter() = 0;
	//结束gather阶段，需要清理缓存
	virtual void output() = 0;
	//输出算法结果
	virtual void reset() = 0;
	//重置现场

};

#endif