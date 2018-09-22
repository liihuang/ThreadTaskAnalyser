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

#define MAX_THREAD_NUM 100000

class ThreadTaskAnalyser{
    std::vector<MyThread> myThreads;
    int threadIndexDict[MAX_THREAD_NUM];
    int numCPU;
    std::ifstream inFile;
    std::ofstream outFile;
public:
    ThreadTaskAnalyser(): numCPU(0){
        std::fill(threadIndexDict, threadIndexDict + MAX_THREAD_NUM, -1);
    }
    ThreadTaskAnalyser(std::string inFileName, std::string outFileName): numCPU(0){
        std::fill(threadIndexDict, threadIndexDict + MAX_THREAD_NUM, -1);
        loadFile(inFileName, outFileName);
    }
    void loadFile(std::string inFileName, std::string outFileName){
        inFile.open(inFileName, std::ifstream::in);
        outFile.open(outFileName, std::ofstream::out);
        if(!inFile.is_open() || !outFile.is_open()){
            std::cout << "Open file failed." << std::endl;
            return;
        }
        //outFile << "isLegal,CPU_index,time,FUNCTION,prev_comm,prev_pid,prev_state,next_comm,next_pid"<< std::endl; 
        parseInFile();
        output2File();
    }
    void parseInFile(){
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
                }
            }else{
                ss >> word;
                if(!recordsDetected){
                    if(word == "#####") recordsDetected = true;
                }else{
                    if(word == "</script>") break;// No data to process
                    else processRecord(line);
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
            // #1: process previous thread
            if(threadIndexDict[record.getPrevPid()] == -1){// New thread
                MyThread newThread(record.getPrevName(), record.getPrevPid(), numCPU);

                newThread.setStartTime(record.getTime());
                newThread.setEndTime(record.getTime());
                
                myThreads.push_back(newThread);
                threadIndexDict[record.getPrevPid()] = myThreads.size() - 1;
            }else{//Exit thread
                int threadIndex = threadIndexDict[record.getPrevPid()];
                
                myThreads[threadIndex].setEndTime(record.getTime());
            }
            // #2: process next thread
            if(threadIndexDict[record.getNextPid()] == -1){// New thread
                MyThread newThread(record.getNextName(), record.getNextPid(), numCPU);

                newThread.setCurrentCPU(record.getCPU_index());
                newThread.setStartTime(record.getTime());

                myThreads.push_back(newThread);
                threadIndexDict[record.getNextPid()] = myThreads.size() - 1;
            }else{//Exist thread
                int threadIndex = threadIndexDict[record.getNextPid()];

                myThreads[threadIndex].setCurrentCPU(record.getCPU_index());
                myThreads[threadIndex].setStartTime(record.getTime());
            }
        }
    }
    void output2File(){
        outFile << "Thread,Pid";
        for(int i = 0; i < numCPU; i++){
            outFile << ",CPU" << i;
            if(i == numCPU - 1) outFile << std::endl;
        }
        for(int i = 0; i < myThreads.size(); i++){
            outFile << myThreads[i].getName() << "," << myThreads[i].getPid();
            const std::vector<double> &totalTime = myThreads[i].getTotalTime();
            for(int j = 0; j < numCPU; j++){
                outFile << "," << totalTime[j];
                if(j == numCPU - 1) outFile << std::endl;
            }
        }
    }
};

#endif  //THREADTASKANALSYSER_H