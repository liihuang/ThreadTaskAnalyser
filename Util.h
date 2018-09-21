#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>

inline int string2Int(std::string str){
    std::stringstream ss;
    int result;
    ss << str;
    ss >> result;
    return result;
}

inline double string2Double(std::string str){
    std::stringstream ss;
    double result;
    ss << str;
    ss >> result;
    return result;
}

#endif  //UTIL_H