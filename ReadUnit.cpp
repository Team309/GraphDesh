#include "ReadUnit.h"
#include "Logwrapper.h"
#include <string>
#include <climits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

ReadUnit::ReadUnit(ParallelEngine* engine):Unit(engine){
	
	update_buf = new Update<update_t>(UPDATE_BUF_SIZE);
	_inited = true;
}

ReadUnit::~ReadUnit(){
	delete update_buf;
}

void ReadUnit::initStep(){}

void ReadUnit::scatter(){
	LOG_TRIVIAL(info)<<"Read scatter ...";
	edge_t edge;		
	while(getNextEdge(edge)){	
		
	}
	endScatter();
}

void ReadUnit::gather(){
	vertex_t updated_num = 0;

	while(!update_buf->isUpdateOver()){

	}

	
	LOG_TRIVIAL(info)<<"Read gather: "<<updated_num<<" / "<<_vertex_num;
	if ( updated_num == 0 ){ 
       	_gather_end = true;
    }		
}

void ReadUnit::endScatter(){
	update_buf->writeTmp();
	update_buf->setWriteOver();
}

void ReadUnit::output(){

}

void ReadUnit::reset(){
	_offset = 0;
	_readed_bytes = 0;
	update_buf->reset();
}


