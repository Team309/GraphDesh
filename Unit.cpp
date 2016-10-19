#include "Unit.h"
#include "ParallelEngine.h"


Unit::Unit(ParallelEngine* engine):
	_engine(engine), _vertex_num(engine->getVertexNum()), _edge_size(sizeof(edge_t)),
	_scatter_end(false), _gather_end(false), _inited(false){
		buf = new char[UNIT_BUF_SIZE];
}
	
bool Unit::getNextEdge(edge_t& edge){
	if((_offset + _edge_size) <= _readed_bytes){
		Types::readEdge(buf + _offset, edge);
		_offset += _edge_size;
		return true;
	}
	else if(_offset == _readed_bytes){
		if((_readed_bytes = _engine->fillInBuf(buf, 0, this)) > _edge_size){
			Types::readEdge(buf, edge);
			_offset = _edge_size;
			return true;
		}
	}
	else if(_offset < _readed_bytes) {
		int start = _readed_bytes - _offset;
		memcpy(buf, buf + _offset, start);
		if((_readed_bytes = _engine->fillInBuf(buf, start, this) + start) > _edge_size){
			Types::readEdge(buf, edge);
			_offset = _edge_size;
			return true;
		}
	}
	return false;
}

