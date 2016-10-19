#include "ParallelEngine.h"
#include "Types.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char * argv[]){
	if(argc < 3){
        std::cout<<"Usage: execute_file filename unit1 arg1 unit2 ... "
                    << std::endl;
        return 0;
    }
	std::stringstream record;
	std::string filename;

	record.clear();
	record<< argv[1];
	record>> filename;

	AlgorithmUnit un;
	ParallelEngine obj(filename,50);
	for(auto i = 2; i < argc; ++i){
		std::string choice = "";
		record.clear();
		record << argv[i];
		record >> choice;
		for(int j = 0; j < AlgorithmType; j++){
			if(choice == AlgorithmUnitName[j]){
				un = AlgorithmUnit(j);
				if(un == sssp || un == bfs){
					vertex_t root;
					record.clear();
					record<< argv[++i];
					record>> root;
					obj.addUnit(un, root);
				}
				else{
					obj.addUnit(un);
				}
				break;
			}
		}
	}
	
	obj.run();
	return 0;
}

