#include "WccUnit.h"
#include "Logwrapper.h"
#include <string>
#include <climits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

WccUnit::WccUnit(ParallelEngine* engine):Unit(engine){
	_aux = new std::vector<array_t>(_vertex_num);
	for (auto iter = _aux->begin(); iter != _aux->end(); iter ++){
		iter->label = iter - _aux->begin() + _vertex_num - 1;
	}
	
	update_buf = new Update<update_t>(UPDATE_BUF_SIZE);
	_inited = true;
}

WccUnit::~WccUnit(){
	delete _aux;
	delete update_buf;
}

void WccUnit::initStep(){}

void WccUnit::scatter(){
	LOG_TRIVIAL(info)<<"WCC scatter ...";
	edge_t edge;		
	while(getNextEdge(edge)){	
		update_t upt;
		auto src = (*_aux)[edge.src].label;
		auto dst = (*_aux)[edge.dst].label;
		if (src < dst){
			upt.id = edge.dst;
			upt.value = edge.src;
		}
		else if (src > dst){
			upt.id = edge.src;
			upt.value = edge.dst;
		}
		else continue;
		update_buf->addUpdate(upt);
	}
	endScatter();
}

void WccUnit::gather(){
	vertex_t updated_num = 0;
	update_t upt;

	while(!update_buf->isUpdateOver()){
		while(update_buf->getUpdate(upt)){
			if ( (*_aux)[upt.id].label > upt.value ){
				(*_aux)[upt.id].label = upt.value;
				++updated_num;
			}
		}
	}
	LOG_TRIVIAL(info)<<"WCC gather: "<<updated_num<<" / "<<_vertex_num;
	if ( updated_num == 0 ){ 
       	_gather_end = true;
    }		
}

void WccUnit::endScatter(){
	update_buf->writeTmp();
	update_buf->setWriteOver();
}

void WccUnit::output(){
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
        	<<iter->label<<std::endl;
	}
}

void WccUnit::reset(){
	_offset = 0;
	_readed_bytes = 0;
	update_buf->reset();
}


