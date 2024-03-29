
#include <string>
//#include "ProcessParser.h"


using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    Process(string pid){
        this->pid = pid;
             user = ProcessParser::getProcUser(pid);
              cmd = ProcessParser::getCmd(pid);
              cpu = ProcessParser::getCpuPercent(pid);
              mem = ProcessParser::getVmSize(pid);
           upTime = ProcessParser::getSysUpTime();
    }
      void setPid(int pid); 
    string getPid()    const;
    string getUser()   const;    
    string getCmd()    const; 
       int getCpu()    const;
       int getMem()    const;
    string getUpTime() const;
    string getProcess(); 
};

void Process::setPid(int pid){
    this->pid = pid;
}
string Process::getPid() const {
    return this->pid;
}
string Process::getUser() const{
    return this->user;
}    
string Process::getCmd() const{
    return this->cmd;
}
int Process::getCpu() const{
    return stoi(this->cpu);
}
int Process::getMem() const{
    return stoi(this->mem);
}
string Process::getUpTime() const{
    return this->upTime;
}    
string Process::getProcess(){
//    if(!ProcessParser::isPidExisting(this->pid))
//        return "";
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return (this->pid + "   " + this->user + "   " +  this->mem.substr(0,5)
         + "   "+ this->cpu + "   " + this->upTime + "   " + this->cmd.substr(0,30));
}
