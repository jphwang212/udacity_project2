#include "processor.h"

#include <vector>
#include <string>

using std::stof;
using std::stol;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
   unsigned long int idle, nonidle, total;
   unsigned long int totald, idled;
   std::vector<std::string> util_data = LinuxParser::CpuUtilization();
   idle = stol(util_data[3]) + stol(util_data[4]);
   nonidle = stol(util_data[0]) + stol(util_data[1]) + stol(util_data[2]) + stol(util_data[5]) + stol(util_data[6]) + stol(util_data[7]);
   total = idle + nonidle;
   totald = total - prevtotal;
   idled = idle - previdle;
   cpuutil_ = (totald - idled) / totald;
   previdle = idle;
   prevtotal = total;
   return cpuutil_;
   }
