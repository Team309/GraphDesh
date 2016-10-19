#include "SsspUnit.h"
#include "Logwrapper.h"
#include <string>
#include <climits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>

SsspUnit::SsspUnit(ParallelEngine* engine, int root):Unit(engine){
	_root = root;
	_aux = new std::vector<array_t>(_vertex_num, {UINT_MAX,UINT_MAX});
	update_buf = new Update<update_t>(UPDATE_BUF_SIZE);

	(*_aux)[_root].pri = _root;
	(*_aux)[_root].dis = 0;
	_inited = true;
}

SsspUnit::~SsspUnit(){
	delete _aux;
	delete update_buf;
}

void SsspUnit::initStep(){}

void SsspUnit::scatter(){
	LOG_TRIVIAL(info)<<"SSSP scatter ...";
	edge_t edge;		
	while(getNextEdge(edge)){			
		update_t upt;
		auto src = (*_aux)[edge.src].dis;
		auto dst = (*_aux)[edge.dst].dis;
		if(src != UINT_MAX && dst > src + 1){
			upt.id = edge.dst;
			upt.dis = src + 1;
			upt.pri = edge.src;
			update_buf->addUpdate(upt);
		}
	}
	endScatter();	
}

void SsspUnit::gather(){
	vertex_t updated_num = 0;
	update_t upt;

	while(!update_buf->isUpdateOver()){
		while(update_buf->getUpdate(upt)){
			++updated_num;
			if( (*_aux)[upt.id].dis > upt.dis){
				(*_aux)[upt.id].dis = upt.dis;
				(*_aux)[upt.id].pri = upt.pri;
			}
		}
	}
	LOG_TRIVIAL(info)<<"SSSP gather: "<<updated_num<<" / "<<_vertex_num;
	if ( updated_num == 0 ){ 
		_gather_end = true;
	}	
}

void SsspUnit::endScatter(){
	update_buf->writeTmp();
	update_buf->setWriteOver();
}

void SsspUnit::output(){
	std::stringstream ss;
	std::srand(std::time(0));
	ss<<"output.csv."<<std::rand();
	std::string outfile;
	ss>>outfile;
	std::ofstream out(outfile, std::ios::out);
	auto bgn = (*_aux).begin();
	for (auto iter = bgn; iter != (*_aux).end(); ++iter){
		out << iter - bgn<<" "
			<<std::fixed<<std::setprecision(16)
			<<iter->dis<<std::endl;
	}
}

void SsspUnit::reset(){
	_offset = 0;
	_readed_bytes = 0;
	update_buf->reset();
}


