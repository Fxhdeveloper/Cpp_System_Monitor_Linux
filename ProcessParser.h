#pragma once

#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"

class ProcessParser{
private:
    std::ifstream stream;
public:
    static                 std::string getCmd(std::string pid);
    static    std::vector<std::string> getPidList();
    static                 std::string getVmSize(std::string pid);
    static                 std::string getCpuPercent(std::string pid);
    static                    long int getSysUpTime();
    static                 std::string getProcUpTime(std::string pid);
    static                 std::string getProcUser(std::string pid);
    static    std::vector<std::string> getSysCpuPercent(std::string coreNumber = "");
    static                       float getSysRamPercent();
    static                 std::string getSysKernelVersion();
    static                         int getTotalThreads();
    static                         int getTotalNumberOfProcesses();
    static                         int getNumberOfRunningProcesses();
    static                 std::string getOSName();
    static                 std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
    static                        bool isPidExisting(std::string pid);
    static                         int getNumberOfCores();
    static                       float get_sys_active_cpu_time(vector<string> values);
    static                       float get_sys_idle_cpu_time(vector<string>values);
};

// TODO: Define all of the above functions below:
std::string ProcessParser::getVmSize(string pid){

    std::string line, value, name = "VmData";
    float result;
    std::string path = Path::basePath() + pid + Path::statusPath();
    //Open Stream
    std::ifstream stream = Util::getStream(path);
    
    while(std::getline(stream, line)){
        //search line by line
        if(line.compare(0, name.size(), name) == 0){
            std::istringstream reader(line);
            istream_iterator<string> begin(reader), end;
            std::vector<std::string> values(begin, end);
            //convert to MB
            result = (stof(values[1])/ float(1024));
            break;
        }
    }
    return to_string(result);

}

std::string ProcessParser::getCpuPercent(std::string pid){

    std::string line;
    std::string path = Path::basePath() + pid + "/" + Path::statPath();
    //open stream and read line
    std::ifstream stream = Util::getStream(path);
    std::getline(stream, line);
    //split line on whitespace
    std::istringstream reader(line);
    std::istream_iterator<std::string> begin(reader), end;
    std::vector<std::string> values(begin, end);

    float     utime = stof(ProcessParser::getProcUpTime(pid));
    float     stime = stof(values[14]);
    float    cutime = stof(values[15]);
    float    cstime = stof(values[16]);
    float starttime = stof(values[21]);
    float    uptime = ProcessParser::getSysUpTime();
    float      freq = sysconf(_SC_CLK_TCK);

    //perform calculations 
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    float result = 100.0 * ((total_time/freq)/seconds);

    return to_string(result); 
    
}

std::string ProcessParser::getProcUpTime(std::string pid){

    std::string line;
    std::string path = Path::basePath() + pid + "/" + Path::statPath();
    //open stream and read line
    std::ifstream stream = Util::getStream(path);
    std::getline(stream, line);

    std::istringstream reader(line);
    std::istream_iterator<std::string> begin(reader), end;
    std::vector<std::string> values(begin, end);

    return to_string(float( stof(values[13]) / sysconf(_SC_CLK_TCK) ));


}

long int ProcessParser::getSysUpTime(){

    std::string line;
    std::string path = Path::basePath() + Path::upTimePath();
    //open stream and read line
    std::ifstream stream = Util::getStream(path);
    std::getline(stream, line);

    std::istringstream reader(line);
    std::istream_iterator<std::string> begin(reader), end;
    std::vector<std::string> values(begin, end);

    return stoi(values[0]);

}

std::string ProcessParser::getProcUser(std::string pid){

    std::string name = "Uid", line, result;
    std::string path = Path::basePath() + pid + Path::statusPath();
   //open stream and read line
    std::ifstream stream = Util::getStream(path);
    while(std::getline(stream, line)){
        std::istringstream reader(line);
        std::istream_iterator<std::string> begin(reader), end;
        std::vector<string> values(begin, end); 
        result = values[1];
        break;
    }
    stream = Util::getStream("/etc/passwd");
    name = ("x:"+ result);
    //search for the name of the user with uid
    while(std::getline(stream, line)){
        if(line.find(name) != std::string::npos){
            result = line.substr(0, line.find(":"));
            return result;
        }
    }
    return "";
}

vector<string> ProcessParser::getPidList(){
    DIR* dir;
    // Basically, we are scanning /proc dir for all directories with numbers as their names
    // If we get valid check we store dir names in vector as list of machine pids
    std::vector<std::string> container;
    if(!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));

    while (dirent* dirp = readdir(dir)) {
        // is this a directory?
        if(dirp->d_type != DT_DIR)
            continue;
        // Is every character of the name a digit?
        if (all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c){ return std::isdigit(c); })) {
            container.push_back(dirp->d_name);
        }
    }
    //Validating process of directory closing
    if(closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
}

std::string ProcessParser::getCmd(std::string pid){
    
    std::string line;
    std::string path = Path::basePath() + pid + Path::cmdPath();
    std::ifstream stream = Util::getStream(path);
    std::getline(stream, line);
    return line;
}

int ProcessParser::getNumberOfCores(){
    //Get the number of cpu cores
    std::string line, name = "cpu cores";
    std::string path = Path::basePath() + "cpuinfo" ;
    std::ifstream stream = Util::getStream(path);

    while(std::getline(stream, line)){

        if( (line.compare(0, name.size(), name)) == 0 ){

            std::istringstream reader(line);
            std::istream_iterator<std::string> begin(reader), end;
            std::vector<std::string> values(begin, end);

            return stoi(values[3]);
        }
    }
    return 0;
}

std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber = ""){
    std::string line, name = "cpu" + coreNumber, result;

    std::string path = Path::basePath() + Path::statPath();
    ifstream stream = Util::getStream(path);

    while(std::getline(stream, line)){
        if(line.compare(0, name.size(), name) == 0){
            std::istringstream reader(line);
            istream_iterator<string> begin(line), end;
            std::vector<std::string> values(begin, end);
            return values;
        }
    } 
    return (std::vector<std::string>());
}

float ProcessParser::get_sys_active_cpu_time(vector<string> values){
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}

float ProcessParser::get_sys_idle_cpu_time(vector<string>values){
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2){

    float activeTime = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);
    float   idleTime = get_sys_idle_cpu_time(values2) - get_sys_idle_cpu_time(values1);
    float  totalTime = activeTime + idleTime;
    float     result = 100.0 * (activeTime / totalTime);
    std::string s = (to_string(result));
}

float ProcessParser::getSysRamPercent(){
    std::string line;
    std::string name1 = "MemAvailable:";
    std::string name2 = "MemFree:";
    std::string name3 = "Buffers:";

    std::string value;
    int result;
    std::ifstream stream = Util::getStream((Path::basePath() + Path::memInfoPath()));
    float total_mem = 0;
    float free_mem = 0;
    float buffers = 0;

    while (std::getline(stream, line)) {
        if (totalMem != 0 && freeMem != 0)
            break;
        if (line.compare(0, name1.size(), name1) == 0) {
            std::istringstream buf(line);
            istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            total_mem = stof(values[1]);
        }
        if (line.compare(0, name2.size(), name2) == 0) {
            std::istringstream buf(line);
            istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            free_mem = stof(values[1]);
        }
        if (line.compare(0, name3.size(), name3) == 0) {
            std::istringstream buf(line);
            istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            buffers = stof(values[1]);
        }
    }
    //calculating usage:
    return float(100.0*(1-(free_mem/(total_mem-buffers))));
}

std::string ProcessParser::getSysKernelVersion(){

    std::string line;
    std::string name = "Linux version ";
    std::ifstream stream = Util::getStream((Path::basePath() + Path::versionPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            std::istringstream buf(line);
            istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            return values[2];
        }
    }
    return "";
}

std::string ProcessParser::getOsName(){

    std::string line;
    std::string name = "PRETTY_NAME=";

    std::ifstream stream = Util::getStream(("/etc/os-release"));

    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
              std::size_t found = line.find("=");
              found++;
              std::string result = line.substr(found);
              result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
              return result;
        }
    }
    return "";

}

int ProcessParser::getTotalThreads(){

    std::string line;
    int result = 0;
    std::string name = "Threads:";
    std::vector<std::string>_list = ProcessParser::getPidList();
    for (int i=0 ; i<_list.size();i++) {
        std::string pid = _list[i];
        //getting every process and reading their number of their threads
        std::ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
        while (std::getline(stream, line)) {
            if (line.compare(0, name.size(), name) == 0) {
                std::istringstream buf(line);
                istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                result += stoi(values[1]);
                break;
            }
        }
    }
    return result;
}

int ProcessParser::getTotalNumberOfProcesses(){

    std::string line;
    int result = 0;
    std::string name = "processes";
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}

int ProcessParser::getNumberOfRunningProcesses(){

    std::string line;
    int result = 0;
    std::string name = "procs_running";
    std::ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}

