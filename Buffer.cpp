#include "Buffer.h"
#include "Logwrapper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <functional>
#include <cstdlib>

std::mutex buffer_mutex;

Buffer::Buffer(int buffer_size):
	thrd(NULL), len(buffer_size),
	front(0), rear(0),
	write_count(0), read_count(0), over(false)
{
	data = new char[len];
}

Buffer::~Buffer(){
	delete[] data;
	delete thrd;
}

bool Buffer::isFull(){
	return ((rear + 1) % len == front);
}

bool Buffer::isEmpty(){
	return (front == rear);
}

int Buffer::enqueue(char* buf, int num){
	auto in = num;
	if(isFull()) return 0;
	if(isEmpty()) {
		in = ((len - 1) < num) ? (len - 1) : num;
	}
	else if (num > (len + front - rear - 1) % len){
		in = (len + front - rear -1) % len;
	}

	if((rear + in) <= len){
		memcpy((void*)(data + rear), (void*)buf, in);
	}
	else{
		auto tmp = len - rear;
		memcpy((void*)(data + rear), (void*)buf, tmp);
		memcpy((void*)data, (void*)(buf + tmp), in-tmp);
	}

	rear = (rear + in) % len;
	return in;
}

int Buffer::dequeue(char* buf, int num){
	auto out = num;
	if(isEmpty()) return 0;
	if(isFull()) {
		out = ((len - 1) < num) ? (len -1) : num;
	}
	else if (num > (len + rear - front) % len){
		out = (len + rear - front) % len;
	}

	if((front + out) <= len){
		memcpy((void*)buf, (void*)(data + front), out); 	
	} 
	else{
		auto tmp = len - front;
		memcpy((void*)buf, (void*)(data + front), tmp);
		memcpy((void*)(buf + tmp), (void*)data, out - tmp);
	}

	front = (front + out) % len;
	return out;
} 

int Buffer::write(std::string filename){
	std::ifstream infile(filename, std::ios::in | std::ios::binary);
	if(!infile.is_open()){
		LOG_TRIVIAL(fatal)<<"read "<<filename<<" failed";
		std::exit(0);
	}
	const int size = 2000;
	char buf[size];
	while(int total = infile.read((char*)buf, size).gcount()){
		auto tmp = total;
		while(tmp){
			std::lock_guard<std::mutex> lock(buffer_mutex);
			auto write_num = enqueue(buf + total - tmp, tmp);
			tmp = tmp - write_num;
			write_count += write_num;
		}
		if(infile.eof()){
			std::lock_guard<std::mutex> lock(buffer_mutex);
			over = true;
		}
	}
	return write_count;
}

int Buffer::read(char* buf, int num){
	auto tmp = num, total = num;
	while(tmp){
		std::lock_guard<std::mutex> lock(buffer_mutex);
		int read_num = dequeue(buf + total - tmp, tmp);
		tmp = tmp - read_num; 
		read_count += read_num; 
		if (over == true && isEmpty())
			break;
	}
	return total - tmp ;
}

void Buffer::startWrite(std::string filename){
	reset();
	auto f = std::bind(&Buffer::write, this, filename);
	thrd = new std::thread(f);
}

void Buffer::writeJoin(){
	if(thrd){
		if(thrd->joinable()){
			thrd->join();
		}
		delete thrd;
		thrd = NULL;
	}
}

bool Buffer::isOver(){	
	std::lock_guard<std::mutex> lock(buffer_mutex);
	return (over == true) && (isEmpty());
}

void Buffer::reset(){
	over = false;
	read_count = 0;
	write_count = 0;
	writeJoin();
}

void Buffer::setFront(int arg){
	front = (front + arg) % len;
	//read_count += arg;
}

int Buffer::getPos(int offset){
	return (front + offset) % len;
}

int Buffer::readOnly(char* buf, int num, int pos){
	auto tmp = num, total = num;
	while(tmp){
		int read_num = dequeueReadOnly(buf + total - tmp, tmp, pos);
		tmp = tmp - read_num; 
		if (over == true && pos == rear)
			break;
	}
	return total - tmp ;
}


int Buffer::dequeueReadOnly(char* buf, int num, int& pos){
	auto out = num;
	if(pos == rear) return 0;
	if((rear + 1) % len == pos) {
		out = ((len - 1) < num) ? (len -1) : num;
	}
	else if (num > (len + rear - pos) % len){
		out = (len + rear - pos) % len;
	}

	if((pos + out) <= len){
		memcpy((void*)buf, (void*)(data + pos), out);	
	} 
	else{
		auto tmp = len - pos;
		memcpy((void*)buf, (void*)(data + pos), tmp);
		memcpy((void*)(buf + tmp), (void*)data, out - tmp);
	}		
	pos = (pos + out) % len;
	return out;
} 

