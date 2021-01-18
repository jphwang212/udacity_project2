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
  CpuUtilization();
  Command();
  Ram();
  User();
  UpTime();
}

// TODO: Return this process's ID
int Process::Pid() { 
  std::string pids = to_string(pid_);
  //pids = pids.substr(0, 6);
  return std::stoi(pids); }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
   float uptime = (float)(LinuxParser::UpTime());
   vector<string> times = LinuxParser::CpuUtilization(pid_);
   float utime = stof(times[0]);
   float stime = stof(times[1]);
   float cutime = stof(times[2]);
   float cstime = stof(times[3]);
   float starttime = stof(times[4]);
   float total_time = utime + stime + cutime + cstime;
   float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
   cpuutil_ = (((total_time) / sysconf(_SC_CLK_TCK)) / seconds);
   return cpuutil_;
 }

// TODO: Return the command that generated this process
string Process::Command() {
   cmd_ = LinuxParser::Command(pid_);
   return cmd_;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  ram_ = LinuxParser::Ram(pid_); 
  return ram_;}

// TODO: Return the user (name) that generated this process
string Process::User() {
      user_ = LinuxParser::User(pid_);
      return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  uptime_ = LinuxParser::UpTime(pid_); 
  return uptime_;}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
   return this->cpuutil_ < a.cpuutil_ ? true : false;
 }
