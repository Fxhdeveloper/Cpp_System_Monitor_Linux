#pragma CONSTANTS_H

#include <string>
using std::string;

class Path{
    public:
        static string    basePath() { return "/proc";    }
        static string     cmdPath() { return "/cmdline"; }
        static string  statusPath() { return "/status";  }
        static string    statPath() { return "stat";     }
        static string  upTimePath() { return "uptime";   }
        static string memInfoPath() { return "meminfo";  }
        static string versionPath() { return "version";  }

};