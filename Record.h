#ifndef RECORD_H
#define RECORD_H

#include "MyThread.h"
#include "Util.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdio.h>

class Record{
    bool isLegal;
    int CPU_index;
    double time;
    std::string op;
    std::string prevName, nextName;
    std::string prevState;
    int prevPid, nextPid;
public:
    Record() = default;
    Record(int _index, double _time, std::string _op, std::string _prevName, std::string _nextName, std::string _prevState, int _prevPid, int _nextPid):
            isLegal(false), CPU_index(_index), time(_time), op(_op), prevName(_prevName), nextName(_nextName), prevState(_prevState), prevPid(_prevPid), nextPid(_nextPid){}
    Record(std::string line){
        // Delete all spaces in the front of line
        //std::cout << "line: " << line << std::endl;
        line.erase(0, line.find_first_not_of(" "));
        //std::cout << "line: " << line << std::endl;
        std::stringstream ss(line);
        std::string word;

        while(ss >> word){
            // [00X]: index of CPU
            if(word[0] == '[' && word[word.length() - 1] == ']'){
                CPU_index = string2Int(word.substr(1, word.length() - 2));
                break;
            }
        }
        // d..X
        ss >> word;
        
        //          dotIndex-
        //                  ↓ 
        // TIMESTAMP 2074248.138849:
        ss >> word;
        int dotIndex = word.find('.');
        setTime(string2Int(word.substr(0, dotIndex)) + string2Double(word.substr(dotIndex, word.length() - 1)));

        // op(FUNCTION): sched_switch/sched_wakeup/sched_blocked_reason/tracing_mark_write
        ss >> word;
        if(word  == "sched_switch:"){
            setOp(word.substr(0, word.length() - 1));
        }else{
            isLegal = false;
            return;
        }

        // prev_comm=FImgMed-duler 3 prev_pid=31918
        ss >> word;
        int equalIndex = word.find('=');
        prevName = word.substr(equalIndex + 1, word.length());
        while(ss >> word){
            equalIndex = word.find('=');
            if(equalIndex != std::string::npos){// "prev_pid=XXXXX"
                prevPid = string2Int(word.substr(equalIndex + 1, word.length()));
                break;
            }
            else{
                prevName += ' ';
                prevName += word;
            }
        }
        // prev_prio=XXX
        ss >> word;
        // prev_state=R/S/D/R+
        ss >> word;
        equalIndex = word.find('=');
        prevState = word.substr(equalIndex + 1, word.length());
        // ==>
        ss >> word;
        // nextcomm=FImgMed-duler 3 next_pid=31918
        ss >> word;
        equalIndex = word.find('=');
        nextName = word.substr(equalIndex + 1, word.length());
        while(ss >> word){
            equalIndex = word.find('=');
            if(equalIndex != std::string::npos){// "next_pid=XXXXX"
                nextPid = string2Int(word.substr(equalIndex + 1, word.length()));
                break;
            }
            else{
                nextName += ' ';
                nextName += word;
            }
        }
        isLegal = true;
        return;
    }

    bool getLegal(){return isLegal;}
    int getCPU_index(){return CPU_index;}
    double getTime(){return time;}
    std::string getOp(){return op;}
    std::string getPrevName(){return prevName;}
    std::string getNextName(){return nextName;}
    std::string getPrevState(){return prevState;}
    int getPrevPid(){return prevPid;}
    int getNextPid(){return nextPid;}

    void setCPU_index(int _index){CPU_index = _index;}
    void setTime(double _time){time = _time;}
    void setOp(std::string _op){op = _op;}
    void setPrevName(std::string _prevName){prevName = _prevName;}
    void setNextName(std::string _nextName){nextName = _nextName;}
    void setPrevPid(int _prevPid){prevPid = _prevPid;}
    void setNextPid(int _nextPid){nextPid = _nextPid;}

    void print(){
        std::cout << "isLegal: " << isLegal << std::endl;
        std::cout << "CPU_index: " << CPU_index << std::endl;
        std::cout << "time: " << time << std::endl;
        std::cout << "FUNCTION: " << op << std::endl;
        std::cout << "prev_comm: " << prevName << ", prev_pid: " << prevPid << ", prev_state: " << prevState << std::endl;
        std::cout << "next_comm: " << nextName << ", next_pid: " << nextPid << std::endl;
    }
};


#endif  //RECORD_H