#ifndef THREADTASKANALSYSER_H
#define THREADTASKANALSYSER_H

#include "Util.h"
#include "MyThread.h"
#include "Record.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#define MAX_THREAD_NUM 50000

class ThreadTaskAnalyser{
    std::vector<MyThread> myThreads;
    int threadIndexDict[MAX_THREAD_NUM];
    std::ifstream file;
public:
    ThreadTaskAnalyser() = default;
    ThreadTaskAnalyser(std::string fileName){
        std::fill(threadIndexDict, threadIndexDict + MAX_THREAD_NUM, -1);
        loadFile(fileName);
    }
    void loadFile(std::string fileName){
        file.open(fileName, std::ios::in);
        if(!file.is_open()){
            std::cout << "Open file failed." << std::endl;
            return;
        }
        parseFile();
    }
    void parseFile(){
        int numCPU = 0;
        bool recordsDetected = false;
        std::string line;
        while(getline(file, line)){
            std::stringstream ss(line);
            
            std::string word;
            while(ss >> word){
                if(word == "tracer:"){
                    getline(file, line);
                    getline(file, line);
                    std::stringstream ss2(line);
                    std::string word2;
                    ss2 >> word2;
                    ss2 >> word2;
                    ss2 >> word2;// #P:XXX
                    word2.erase(0,3);// XXX
                    numCPU = string2Int(word2);
                }
                if(word == "<idle>-0" && recordsDetected == false){
                    recordsDetected = true;
                    processRecord(line);
                }
            }
        }   
    }
    void processRecord(const std::string &recodrLine){
        std::stringstream ss(recodrLine);
    }
};

#endif  //THREADTASKANALSYSER_H