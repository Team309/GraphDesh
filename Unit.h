#ifndef _UNIT_
#define _UNIT_

#include "Types.h"

class ParallelEngine;

//����Ԫ��
//����࣬��Ҫ�㷨��Ԫ�̳в���ӿں���
class Unit{
protected:
	ParallelEngine* _engine;
	int _vertex_num;
	int _edge_size;
	int _offset;
	//ͼ���ݻ���ƫ����
	int _readed_bytes;
	//�������Чͼ���ݴ�С
	bool _scatter_end;
	bool _gather_end;
	bool _inited;
	//�Ƿ���Ҫ��ʼ��

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
	//��ʼ����Ҫ�Ĺ���
	virtual void scatter() = 0;
	//scatter�׶νӿ�
	virtual void gather() = 0;
	//gather�׶νӿ�
	virtual void endScatter() = 0;
	//����gather�׶Σ���Ҫ������
	virtual void output() = 0;
	//����㷨���
	virtual void reset() = 0;
	//�����ֳ�

};

#endif