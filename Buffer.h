#ifndef _BUFFER_
#define _BUFFER_

#include <thread>
#include <mutex>

extern std::mutex buffer_mutex;


//������
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
	//�жϻ����Ƿ�Ϊ��

	bool isEmpty();
	//�жϻ����Ƿ�Ϊ��
	
	int enqueue(char* buf, int num);
	//write�������ã�����д��
	
	int dequeue(char* buf, int num);
	//read�������ã�������ȡ

	int write(std::string filename);
	//�򻺴�д������

	int read(char* buf, int num);
	//�ӻ����ȡ����
	
	void startWrite(std::string filename);
	//��������I/O�߳�

	void writeJoin();
	//����I/O�̼߳���

	bool isOver();
	//�ж�ͼ�����Ƿ�ȫ����ȡ���

	void reset();
	//�����ֳ�
	
	void setFront(int arg);
	//���û�����Ч�����ͷ��

	int getPos(int offset);
	//��ȡ����ǰ��ȡ�����λ��

	int readOnly(char* buf, int num, int pos);
	//��������첽��ȡ
	
	int dequeueReadOnly(char* buf, int num, int& pos);
	//ReadOnly���ã�������ȡ
};

#endif