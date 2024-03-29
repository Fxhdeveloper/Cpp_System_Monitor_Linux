#pragma PROJECTPARSER_H

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
#include <constants.h>

class ProcessParser {
    public:
        static std::string get_cmd(std::string pid);
        static std::vector<std::string> get_pid_list();
        static std::string get_vm_size(std::string pid);
        static std::string get_cpu_percent(std::string pid);
        static long int get_sys_up_time();
        static std::string get_proc_up_time(std::string pid);
        static std::string get_proc_user(std::string pid);
        static std::vector<std::string> get_sys_cpu_percent(std::string coreNumber = "");
        static float get_sys_ram_percent();
        static std::string get_sys_kernel_version();
        static int get_total_threads();
        static int get_total_number_of_processes();
        static int get_number_of_running_processes();
        static string get_os_name();
        static std::string print_cpu_stats(std::vector<std::string> values1, std::vector<std::string>values2);
};