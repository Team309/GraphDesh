#include "PRUnit.h"
#include "Logwrapper.h"
#include <string>
#include <climits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>

PRUnit::PRUnit(ParallelEngine* engine):Unit(engine){
	init_wgt = 0.15 / _vertex_num;
	init_array.degree = 0;
	init_array.res = init_wgt;
	init_array.tmp = 0;
	edge_size = sizeof(edge_t);
	updated_num = 0;
	
	update_bitset = new std::vector<bool>(_vertex_num, false);
	_aux = new std::vector<array_t>(_vertex_num, init_array);
	update_buf = new Update<update_t>(UPDATE_BUF_SIZE);
}

PRUnit::~PRUnit(){
	delete update_bitset;
	delete _aux;
	delete update_buf;
}

void PRUnit::initStep(){
	edge_t edge;
	while(getNextEdge(edge)){
		(*_aux)[edge.src].degree += 1;	
	}
	_inited = true;
}

void PRUnit::scatter(){
	LOG_TRIVIAL(info)<<"PR scatter ...";
	edge_t edge;		
	while(getNextEdge(edge)){	
		update_t upt;
		if ((*update_bitset)[edge.dst] == false){			
			upt.id = edge.dst;
			upt.value = (*_aux)[edge.src].res / (*_aux)[edge.src].degree;
			update_buf->addUpdate(upt);
		}
	}
	endScatter();	
}

void PRUnit::gather(){
	update_t upt;
	weight_t tmp;
	
	while(update_buf->getUpdate(upt)){
		if ((*update_bitset)[upt.id] == false){
			(*_aux)[upt.id].tmp += upt.value ;
		}
	}

	for (auto iter = _aux->begin(); iter != _aux->end(); iter++){
		if((*update_bitset)[iter - _aux->begin()])continue;
		tmp = init_wgt + 0.85 * iter->tmp;
		if (std::fabs(iter->res - tmp ) < 0.000000001){
			(*update_bitset)[iter - _aux->begin()] = true;
			updated_num ++ ;	
		}	
		else{
			iter->res = tmp;
		}
		iter->tmp = 0.0;
	}
	
	LOG_TRIVIAL(info)<<"PR gather: "<<_vertex_num - updated_num<<" / "<<_vertex_num;
	if ( updated_num == _vertex_num){ 
       	_gather_end = true;
    }		
}

void PRUnit::endScatter(){
	update_buf->writeTmp();
	update_buf->setWriteOver();
}

void PRUnit::output(){
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
        	<<iter->res<<std::endl;
	}
}

void PRUnit::reset(){
	_offset = 0;
	_readed_bytes = 0;
	update_buf->reset();
}


