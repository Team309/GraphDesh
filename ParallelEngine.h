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

//��������������
class ParallelEngine{
protected:
	enum {DISK_IO_SIZE = 100000000};
	
	std::string _filename;
	//ͼ�����ļ���
	int _maxloop;
	//����������
	int step_counter;
	//����������
	int vertex_num;
	//ͼ���ݼ�������
	int edge_num;
	//ͼ���ݱ���
	int _type;
	//ͼ��������
	int min_offset;
	//����Ԫ��С��ƫ����
	int unit_num;
	//����Ԫ��

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
	//Ϊ����Ԫ�Ļ�����������

	int getPosition(Unit* pUnit);
	//��ȡ��ǰ����Ԫ����ͼ���ݵ�λ��
	
	void addUnit(AlgorithmUnit choice, int arg = 0);
	//���������������Ԫ
	
	void run();
	//����������
};

#endif
