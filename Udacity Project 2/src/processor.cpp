#include "processor.h"

#include <vector>
#include <string>

using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
   std::vector<std::string> processes = LinuxParser::CpuUtilization();
   std::vector<Processor> cpu_proc;
   for(long unsigned int i = 0; i < processes.size(); i++){
      Processor proc;
      proc.usage = std::stof(processes[i]);
      cpu_proc.push_back(proc);
   }
   float usertime = cpu_proc[0].usage - cpu_proc[8].usage;
   float nicetime = cpu_proc[1].usage - cpu_proc[9].usage;
   float idletime = cpu_proc[3].usage + cpu_proc[4].usage;
   float systemtime = cpu_proc[2].usage + cpu_proc[5].usage + cpu_proc[6].usage;
   float virt = cpu_proc[8].usage + cpu_proc[9].usage;
   float steal = cpu_proc[7].usage;

   return usertime + nicetime + systemtime + idletime + + steal + virt; }
