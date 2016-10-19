#ifndef _UPDATE_
#define _UPDATE_

#include "Buffer.h"

//更新类
//模板类，使用更新元素的数据结构特化
//继承自Buffer类，复用一些通用函数
template <class elem_type>
class Update: public Buffer{
protected:
	enum { ELEM_NUM = 100};
	elem_type* tmp_snd;
	elem_type* tmp_rcv;
	int pSnd, pRcv;
	int elem_size;
	bool update_over;
	std::mutex update_mutex;

public:
	Update(int size = Buffer::DEFAULT_BUFFER_SIZE):
		Buffer::Buffer(size),
		pSnd(0),pRcv(-1),
		elem_size(sizeof(elem_type)),
		update_over(false){
			tmp_snd = new elem_type[ELEM_NUM]();
			tmp_rcv = new elem_type[ELEM_NUM]();
		}

	~Update(){
		delete[] tmp_snd;
		delete[] tmp_rcv;
	}

	//批量写更新
	void write(char* buf, int size){
		auto total = size, tmp = size;
		while(tmp){
			std::lock_guard<std::mutex> lock(update_mutex);
			tmp -= this->enqueue(buf + total - tmp, tmp);
		}
	}

	//清理更新缓存
	void writeTmp(){
		write((char*)tmp_snd, pSnd * elem_size);
		pSnd = 0;
	}

	//设置scatter写更新阶段结束
	void setWriteOver(){
		this->update_over = true;
	}

	//任务单元添加更新的接口
	void addUpdate(elem_type upt){
		if(pSnd != ELEM_NUM){
			tmp_snd[pSnd] = upt;
			++pSnd;
		}
		else{
			write((char*)tmp_snd, ELEM_NUM * elem_size);
			pSnd = 0;
			tmp_snd[pSnd] = upt;
			++pSnd;
		}
	}

	//批量读更新
	int read(char* buf, int num){
		auto tmp = num, total = num;
		while(tmp){
			std::lock_guard<std::mutex> lock(update_mutex);
			int read_num = dequeue(buf + total - tmp, tmp);
			tmp = tmp - read_num; 
			if (update_over == true && isEmpty())
				break;
		}
		return total - tmp ;
	}

	//任务单元获取更新的接口
	bool getUpdate(elem_type& upt){
		if(pRcv != -1){
			upt = tmp_rcv[pRcv];
			--pRcv;
			return true;
		}
		else{
			auto cnt = this->read((char*)tmp_rcv, ELEM_NUM * elem_size);
			if(cnt == 0){
				return false;
			}
			else{
				pRcv = cnt / elem_size - 1;
				upt = tmp_rcv[pRcv];
				--pRcv;
				return true;
			}
		}
	}

	//清理现场
	void reset(){
		this->update_over = false;
		this->read_count = 0;
		this->write_count = 0;
	}

	//gather阶段判断更新是否全部完成
	bool isUpdateOver(){
		return update_over;
	}
	
};

#endif