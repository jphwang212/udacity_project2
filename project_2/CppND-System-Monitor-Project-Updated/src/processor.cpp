#include "processor.h"

#include <vector>
#include <string>

using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
   double idle, total, totald, idled, time, timed;
   float answer{};
   idle = LinuxParser::Jiffies();
   total = LinuxParser::Jiffies();
   time = LinuxParser::UpTime();
   totald = total - prevtotal;
   idled = idle - previdle;
   timed = time - prevtime;
   prevtotal = total;
   previdle = idle;
   prevtime = time;
   answer = ((totald - idled)/totald)/timed;
   return answer;
   }
