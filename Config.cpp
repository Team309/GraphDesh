#include "Config.h"	

using namespace std;

Config::Config(string filename, string delimiter, string comment)
			:m_Delimiter(delimiter),m_Comment(comment)
{
		ifstream in(filename);
		if(!in) cout<<"open config file failed"<<endl;

		in>>(*this);
}

Config::Config()  
: m_Delimiter( string(1,'=') ), m_Comment( string(1,'#') )  
{  
    // Construct a Config without a file; empty  
} 

ostream& operator<<(ostream& os, const Config& cf)  
{  
    // Save a Config to os  
    for( Config::mapci p = cf.m_Contents.begin();  
        p != cf.m_Contents.end();  
        ++p )  
    {  
        os << p->first << " " << cf.m_Delimiter << " ";  
        os << p->second << endl;  
    }  
    return os;  
} 

istream& operator>>(istream& is, Config& cf)
{
	typedef string::size_type pos;
	const string& delim = cf.m_Delimiter;   
    const string& comm = cf.m_Comment;    
    const pos skip = delim.length();   

    while(is){
    	string line;
    	getline(is,line);
    	line = line.substr(0, line.find(comm));
    	pos delimPos = line.find(delim);
    	if(delimPos < string::npos){
    		string key = line.substr(0,delimPos);
    		line.replace(0, delimPos+skip, "");
    		Config::Trim(key);
    		Config::Trim(line);
    		cf.m_Contents[key] = line;
    	}
    }
    return is;
}

void Config::Trim( string& inout_s )  
{  
    // Remove leading and trailing whitespace  
    static const char whitespace[] = " \n\t\v\r\f";  
    inout_s.erase( 0, inout_s.find_first_not_of(whitespace) );  
    inout_s.erase( inout_s.find_last_not_of(whitespace) + 1U );  
}