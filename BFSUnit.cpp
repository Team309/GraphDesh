#include "BFSUnit.h"
#include "Logwrapper.h"
#include <string>
#include <climits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

BFSUnit::BFSUnit(ParallelEngine* engine, int root):Unit(engine){
	_root = root;
	_aux = new std::vector<array_t>(_vertex_num);
	for (auto iter = _aux->begin(); iter != _aux->end(); iter ++){
		iter->visited = false;
	}
	(*_aux)[_root].visited = true;
	
	update_buf = new Update<update_t>(UPDATE_BUF_SIZE);
	upt_que.reserve(_vertex_num);
	_inited = true;
}

BFSUnit::~BFSUnit(){
	delete _aux;
	delete update_buf;
}

void BFSUnit::initStep(){}

void BFSUnit::scatter(){
	LOG_TRIVIAL(info)<<"BFS scatter ...";
	edge_t edge;		
	while(getNextEdge(edge)){	
		update_t upt;
		auto dst = (*_aux)[edge.dst].visited;
		if(dst) continue;
		else {
			auto src = (*_aux)[edge.src].visited;
			if(src){
				upt.id = edge.dst;
				update_buf->addUpdate(upt);
			}
		}
	}
	endScatter();
}

void BFSUnit::gather(){
	vertex_t updated_num = 0;
	update_t upt;

	upt_que.clear();
	while(!update_buf->isUpdateOver()){
		while(update_buf->getUpdate(upt)){
			upt_que.push_back(upt.id);
		}
	}

	for(auto iter = upt_que.begin(); iter != upt_que.end(); ++iter){
		if(!(*_aux)[*iter].visited){
			(*_aux)[*iter].visited = true;
			++updated_num;
		}
	}
	
	LOG_TRIVIAL(info)<<"BFS gather: "<<updated_num<<" / "<<_vertex_num;
	if ( updated_num == 0 ){ 
       	_gather_end = true;
    }		
}

void BFSUnit::endScatter(){
	update_buf->writeTmp();
	update_buf->setWriteOver();
}

void BFSUnit::output(){

}

void BFSUnit::reset(){
	_offset = 0;
	_readed_bytes = 0;
	update_buf->reset();
}


