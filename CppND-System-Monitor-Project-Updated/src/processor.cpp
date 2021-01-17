#include "processor.h"

#include <vector>
#include <string>

using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
   float idle, nonidle, total;
   float totald, idled;
   std::vector<std::string> util_data = LinuxParser::CpuUtilization();
   idle = stof(util_data[3]) + stof(util_data[4]);
   nonidle = stof(util_data[0]) + stof(util_data[1]) + stof(util_data[2]) + stof(util_data[5]) + stof(util_data[6]) + stof(util_data[7]);
   total = idle + nonidle;
   totald = total - prevtotal;
   idled = idle - previdle;
   cpuutil_ = (totald - idled) / totald;
   previdle = idle;
   prevtotal = total;
   return cpuutil_;
   }
