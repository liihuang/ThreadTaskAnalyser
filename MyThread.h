#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <string>
#include <vector>

class MyThread{
    std::string name;
    int tid;
    bool state;// 1 = running, 0 = sleep
    int currentCPU, numCPU;
    double startTime, endTime;
    std::vector<double> totalTime;
public:
    MyThread() = default;
    MyThread(std::string _name, int _tid, int _numCPU): name(_name), tid(_tid), numCPU(_numCPU){
        state = false;
        currentCPU = -1;
        startTime = -1;
        endTime = -1;
        totalTime.resize(numCPU);
    }
    
    std::string getName(){return name;}
    int getTid(){return tid;}
    bool isRunning(){return state;}
    int getCurrentCPU(){return currentCPU;}
    double geteStartTime(){return startTime;}
    double getEndTime(){return endTime;}
    std::vector<double> getTotalTime(){return totalTime;}

    void setName(std::string _name){name = _name;}
    void setTid(int _tid){tid = _tid;}
    void setState(int _state){state = _state;}
    void setCurrentCPU(int _currentCPU){currentCPU = _currentCPU;}
    void setStartTime(double _startTime){
        startTime = _startTime;
        setState(1);
    }
    void setEndTime(double _endTime){
        endTime = _endTime;
        setState(0);
        updateTotalTime(currentCPU);
    }

    void updateTotalTime(int cpuIndex){
        totalTime[cpuIndex] += (endTime - startTime);
    }
};

#endif //MYTHREAD_H