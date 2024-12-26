#include <cstdlib>
#include <string>
#include <iostream>
#include <map>

int main(int argc,char** argv){
    // std::string xxx = "#EXT-X-VERSION:3";
    // char* start = nullptr;
    // int version = std::strtol(xxx.c_str()+sizeof("#EXT-X-VERSION:")-1,&start,10);
    
    // std::cout << version << std::endl;
    // std::cout << start << std::endl;

    // std::map<int,std::string> maps;
    // int index =0;
    // maps.emplace(++index,"1");
    // maps.emplace(++index,"2");

    // for(const auto& pair : maps){
    //     std::cout << pair.first << ":" << pair.second << std::endl;
    // }

    std::string source = "http://127.0.0.1:8080/xxx/xxx.m3u8";
    std::string substr = source;
    size_t pos = substr.find("://");
    if(pos == std::string::npos){
        // not valid xxxx:// start
        throw std::runtime_error("xxxx");
    }
    std::string protocol = substr.substr(0,pos);
    substr = substr.substr(pos+ 3);

    std::cout << "protocol:"  << protocol << std::endl;
    std::cout << "substr:" << substr << std::endl;
    
}