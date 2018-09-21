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
#define MAX_CPU_NUM 100
class ThreadTaskAnalyser{
    std::vector<MyThread> myThreads;
    int threadIndexDict[MAX_THREAD_NUM];
    int threadOnCPU[MAX_CPU_NUM];
    std::ifstream inFile;
    std::ofstream outFile;
public:
    ThreadTaskAnalyser() = default;
    ThreadTaskAnalyser(std::string inFileName, std::string outFileName){
        std::fill(threadIndexDict, threadIndexDict + MAX_THREAD_NUM, -1);
        std::fill(threadOnCPU, threadOnCPU + MAX_CPU_NUM, -1);
        loadFile(inFileName, outFileName);
    }
    void loadFile(std::string inFileName, std::string outFileName){
        inFile.open(inFileName, std::ifstream::in);
        outFile.open(outFileName, std::ofstream::out);
        if(!inFile.is_open() || !outFile.is_open()){
            std::cout << "Open file failed." << std::endl;
            return;
        }
        outFile << "isLegal,CPU_index,time,FUNCTION,prev_comm,prev_pid,prev_state,next_comm,next_pid"<< std::endl; 
        parseInFile();
    }
    void parseInFile(){
        int numCPU = 0;
        bool recordsDetected = false;
        std::string line;
        while(getline(inFile, line)){
            std::stringstream ss(line);
            std::string word;
            if(numCPU == 0){
                std::string word1, word2;
                ss >> word1 >> word2;
                if(word1 == "#" && word2 == "tracer:"){
                    getline(inFile, line);
                    getline(inFile, line);
                    std::stringstream ss2(line);
                    ss2 >> word;// #
                    ss2 >> word;// entries-in-buffer/entries-written:
                    ss2 >> word;// 972935/982732
                    ss2 >> word;// #P:XXX
                    word.erase(0,3);// XXX
                    numCPU = string2Int(word);
                    std::cout << numCPU << std::endl;
                }
            }else{
                ss >> word;
                if(!recordsDetected){
                    if(word == "<idle>-0"){
                        recordsDetected = true;
                        std::cout << "record detected!" << std::endl;
                        processRecord(line);
                    }
                }else{
                    if(word == "</script>") break;// No data to process
                    processRecord(line);
                }
            }
        }   
    }
    void processRecord(const std::string &recordLine){
        Record record(recordLine);
        //record.print();
        /*if(record.getLegal()){
            outFile << record.getLegal() << ','
                    << record.getCPU_index() << ','
                    << record.getTime() << ','
                    << record.getOp() << ','
                    << record.getPrevName() << ','
                    << record.getPrevPid() << ','
                    << record.getPrevState() << ','
                    << record.getNextName() << ','
                    << record.getNextPid() << std::endl;
        }*/
        if(record.getLegal()){
            
        }
    }
};

#endif  //THREADTASKANALSYSER_H