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