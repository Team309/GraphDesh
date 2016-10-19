#include "ParallelEngine.h"
#include "Logwrapper.h"
#include "Config.h"
#include "Unit.h"
#include "SsspUnit.h"
#include "PRUnit.h"
#include "WccUnit.h"
#include "BFSUnit.h"
#include "ReadUnit.h"
#include <iostream>
#include <mutex>

std::mutex offset_mutex;

using namespace std;

void ParallelEngine::resetUnitQue(){
	for(auto iter = unit_vec.begin(); iter != unit_vec.end(); ++iter){
		(*iter)->reset();
	}
	for(auto iter = unit_map.begin(); iter != unit_map.end(); ++iter){
		iter->second = 0;
	}
}			 


ParallelEngine::ParallelEngine(std::string filename, int maxloop)
	:_filename(filename), 
	 _maxloop(maxloop),
	 step_counter(0),
	 min_offset(0)
	 {
		std::string config_file = filename + ".ini";
		Config conf(config_file);
		vertex_num = conf.Read("vertices",0);
		edge_num = conf.Read("edges",0);
		_type = conf.Read("type",0);

		disk_io = new Buffer(DISK_IO_SIZE);
	 }

int ParallelEngine::fillInBuf(char* buf, int start, Unit* pUnit){
	 int readed_bytes = 0;
	 if(!disk_io->isOver()){
		 int pos = getPosition(pUnit);
		 readed_bytes = disk_io->readOnly(buf + start, Unit::UNIT_BUF_SIZE - start, pos);
		 std::lock_guard<std::mutex> lock(offset_mutex);
		 unit_map[pUnit] += readed_bytes;
		 int min_off = unit_map[pUnit];
		 for(auto iter = unit_map.begin(); iter != unit_map.end(); ++iter){
			 if(iter->second < min_off)min_off = iter->second;
		 }
		 if(min_off != 0){
			 disk_io->setFront(min_off);
			 for(auto iter = unit_map.begin(); iter != unit_map.end(); ++iter){
				 iter->second -= min_off;
			 }
		 }
	 }
	 else{
		 pUnit->setScaterEnd();
	 }
	 return readed_bytes;
 }

int ParallelEngine:: getPosition(Unit* pUnit){
	 std::lock_guard<std::mutex> lock(offset_mutex);
	 return disk_io->getPos(unit_map[pUnit]);
}

void ParallelEngine::run(){ 	
	while(!reachMaxStep()){
		LOG_TRIVIAL(info)<<"iterator "<<superStep();
		resetUnitQue();
		disk_io->startWrite(_filename);

		scatter_thrd_vec.clear();
		gather_thrd_vec.clear();
		for(auto iter = unit_vec.begin(); iter != unit_vec.end(); ++iter){
			if((*iter)->isInited()){
				std::thread* scatter_thrd = new std::thread(&Unit::scatter, *iter);
				scatter_thrd_vec.push_back(scatter_thrd);
				std::thread* gather_thrd = new std::thread(&Unit::gather, *iter);
				gather_thrd_vec.push_back(gather_thrd);
			}
			else{
				std::thread* scatter_thrd = new std::thread(&Unit::initStep, *iter);
				scatter_thrd_vec.push_back(scatter_thrd);
			}
		}

		disk_io->writeJoin();
		for(auto iter = scatter_thrd_vec.begin(); iter != scatter_thrd_vec.end(); ++iter){
			(*iter)->join();
		}
		for(auto iter = gather_thrd_vec.begin(); iter != gather_thrd_vec.end(); ++iter){
			(*iter)->join();
		}

		bool completed = true;
		for(auto iter = unit_vec.begin(); iter != unit_vec.end();){
			if((*iter)->isGatherEnd()){
				(*iter)->output();
				auto it = unit_map.find(*iter);
				if(it != unit_map.end())unit_map.erase(it);
				iter = unit_vec.erase(iter);
			}
			else{
				completed = false;
				 ++iter;
			}
		}
		if(completed)return;
		nextSuperStep();
	}
}

void ParallelEngine::addUnit(AlgorithmUnit choice, int arg){
	Unit* u;
	switch(choice){
		case sssp:
			LOG_TRIVIAL(info)<<"add unit sssp(root "<<arg<<")";
			u = new SsspUnit(this,arg);
			unit_vec.push_back(u);
			unit_map.insert({u,0});
			break;
		case pr:
			LOG_TRIVIAL(info)<<"add unit pr ";
			u = new PRUnit(this);
			unit_vec.push_back(u);
			unit_map.insert({u,0});
			break;
		case wcc:
			LOG_TRIVIAL(info)<<"add unit wcc ";
			u = new WccUnit(this);
			unit_vec.push_back(u);
			unit_map.insert({u,0});
			break;
		case bfs:
			LOG_TRIVIAL(info)<<"add unit bfs(root "<<arg<<")";
			u = new BFSUnit(this,arg);
			unit_vec.push_back(u);
			unit_map.insert({u,0});
			break;
		case read:
			LOG_TRIVIAL(info)<<"add unit read ";
			u = new ReadUnit(this);
			unit_vec.push_back(u);
			unit_map.insert({u,0});
			break;
	}
}

