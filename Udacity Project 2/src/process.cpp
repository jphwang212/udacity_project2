#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
  pid_ = pid;
  cmd_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
  user_ = LinuxParser::User(pid_);
  uptime_ = LinuxParser::UpTime(pid_);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
   long uptime = LinuxParser::UpTime();
   string line;
   string a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a18, a19, a20, a21;
   string utime, stime, cutime, cstime, starttime;
   std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
   if(filestream.is_open()){
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10 >>
      a11 >> a12 >> a13 >> utime >> stime >> cutime >> cstime >> a18 >> a19 >> a20 >>
      a21 >> starttime;
      filestream.close();
   }
   float total_time = std::stof(utime) + std::stof(stime) + std::stof(cutime) + std:: stof(cstime);
   float seconds = uptime - (stof(starttime) / sysconf(_SC_CLK_TCK));
   float cpuutil_ = 100.0 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
   return cpuutil_;
 }

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
   return this->cpuutil_ < a.cpuutil_ ? true : false;
 }
