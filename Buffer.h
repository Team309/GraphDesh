#ifndef _BUFFER_
#define _BUFFER_

#include <thread>
#include <mutex>

extern std::mutex buffer_mutex;


//缓存类
class Buffer{
protected:
	enum {DEFAULT_BUFFER_SIZE = 500000000};
	char *data;
	std::thread* thrd;
	int len;
	int front;
	int rear;
	int write_count;
	int read_count;
	bool over;

public:
	Buffer(int buffer_size = DEFAULT_BUFFER_SIZE);

	~Buffer();

	bool isFull();
	//判断缓存是否为满

	bool isEmpty();
	//判断缓存是否为空
	
	int enqueue(char* buf, int num);
	//write函数调用，批量写入
	
	int dequeue(char* buf, int num);
	//read函数调用，批量读取

	int write(std::string filename);
	//向缓存写入数据

	int read(char* buf, int num);
	//从缓存读取数据
	
	void startWrite(std::string filename);
	//开启地盘I/O线程

	void writeJoin();
	//磁盘I/O线程技术

	bool isOver();
	//判断图数据是否全部读取完毕

	void reset();
	//重置现场
	
	void setFront(int arg);
	//设置缓存有效区域的头部

	int getPos(int offset);
	//获取任务当前读取缓存的位置

	int readOnly(char* buf, int num, int pos);
	//多任务的异步读取
	
	int dequeueReadOnly(char* buf, int num, int& pos);
	//ReadOnly调用，批量读取
};

#endif