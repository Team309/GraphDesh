#ifndef _CONFIG_
#define _CONFIG_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

//读取配置文件类
class Config{
protected:
	std::string m_Delimiter;
	std::string m_Comment; 
	std::map<std::string,std::string> m_Contents;
	typedef std::map<std::string,std::string>::iterator mapi;
	typedef std::map<std::string,std::string>::const_iterator mapci;

	template<class T> static T string_as_T( const std::string& s );  
	static void Trim( std::string& inout_s ); 

public:
	Config();
	Config(std::string config_filename, std::string delimiter = "=", std::string comment = "#");

	template<class T> T Read( const std::string& in_key ) const;
    template<class T> T Read( const std::string& in_key, const T& in_value ) const; 

	friend std::ostream& operator<<( std::ostream& os, const Config& cf );  
    friend std::istream& operator>>( std::istream& is, Config& cf );  
};

template<class T>  
T Config::Read( const std::string& key ) const  
{  
    mapci p = m_Contents.find(key);  
    if( p == m_Contents.end() ) std::cout<<"read key "<<key<<" not found"<<std::endl;  
    return string_as_T<T>( p->second );  
} 
 
template<class T>  
T Config::Read( const std::string& key, const T& value ) const  
{  
    // Return the value corresponding to key or given default value  
    // if key is not found  
    mapci p = m_Contents.find(key);  
    if( p == m_Contents.end() ) return value;  
    return string_as_T<T>( p->second );  
}  
 
template<class T>  
T Config::string_as_T( const std::string& s )  
{  
    // Convert from a string to a T  
    // Type T must support >> operator  
    T t;  
    std::istringstream ist(s);  
    ist >> t;  
    return t;  
}  
     
template<>  
inline std::string Config::string_as_T<std::string>( const std::string& s )  
{  
    // Convert from a string to a string  
    // In other words, do nothing  
    return s;  
}  
    
template<>  
inline bool Config::string_as_T<bool>( const std::string& s )  
{  
    // Convert from a string to a bool  
    // Interpret "false", "F", "no", "n", "0" as false  
    // Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true  
    bool b = true;  
    std::string sup = s;  
    for( std::string::iterator p = sup.begin(); p != sup.end(); ++p )  
        *p = toupper(*p);  // make string all caps  
    if( sup==std::string("FALSE") || sup==std::string("F") ||  
        sup==std::string("NO") || sup==std::string("N") ||  
        sup==std::string("0") || sup==std::string("NONE") )  
        b = false;  
    return b;  
}


#endif