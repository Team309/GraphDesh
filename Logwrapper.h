#ifndef _LOGWRAPPER_
#define _LOGWRAPPER_
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <ctime>

enum log_state {
	fatal = 0,
	error = 1,
	warn = 2,
	info = 3,
	total = 4,
};
static const char * log_header[total]=
{
	"FATAL", "ERROR", "WARNING", "INFO"
};

//日志文件输出的包装类
class LOG_TRIVIAL {
	enum log_state level;
public:
	std::stringstream state;
	LOG_TRIVIAL(enum log_state level_in)
		:level(level_in){}
	
	template<typename T>
	LOG_TRIVIAL & operator<< (T value){
		state<< value;
		return *this;
	}
	const std::string get_current_time(){
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::string ts = std::ctime(&t);
		ts.resize(ts.size()-1);
		return ts;
	}	
	~LOG_TRIVIAL(){
		std::stringstream final;
		final << get_current_time();
		final << " < " <<log_header[level] <<" ";
		final << std::this_thread::get_id() <<"> ";
		final << state.str() <<std::endl;
		std::cerr <<final.str();
	}
	

};
#endif
