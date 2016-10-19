#ifndef _PARALLEL_ENGINE_
#define _PARALLEL_ENGINE_

#include "Buffer.h"
#include "Types.h"
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <unordered_map>

class Unit;

//多任务工作引擎类
class ParallelEngine{
protected:
	enum {DISK_IO_SIZE = 100000000};
	
	std::string _filename;
	//图数据文件名
	int _maxloop;
	//最大迭代次数
	int step_counter;
	//迭代计数器
	int vertex_num;
	//图数据集顶点数
	int edge_num;
	//图数据边数
	int _type;
	//图数据类型
	int min_offset;
	//任务单元最小的偏移量
	int unit_num;
	//任务单元数

	std::vector<std::thread*> scatter_thrd_vec;
	std::vector<std::thread*> gather_thrd_vec;
	std::vector<Unit*> unit_vec;
	std::unordered_map<Unit*, int> unit_map;

	Buffer* disk_io;

	void resetUnitQue();
 
public:
	ParallelEngine(std::string filename, int maxloop);
	
	~ParallelEngine(){
		delete disk_io;
	}

	int getVertexNum(){
		return vertex_num;
	}

	void nextSuperStep(){
		++step_counter;
	}

	int superStep(){
		return step_counter;
	}

	bool reachMaxStep(){
		return (step_counter >= _maxloop);
	}

	int fillInBuf(char* buf, int start, Unit* pUnit);
	//为任务单元的缓存填充边数据

	int getPosition(Unit* pUnit);
	//获取当前任务单元访问图数据的位置
	
	void addUnit(AlgorithmUnit choice, int arg = 0);
	//向工作引擎添加任务单元
	
	void run();
	//开启主流程
};

#endif
