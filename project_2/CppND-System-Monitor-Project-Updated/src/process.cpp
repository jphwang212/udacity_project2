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
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
   cpuutil_ = (LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK))/(LinuxParser::UpTime(pid_)/1000);
   return cpuutil_;
 }

// TODO: Return the command that generated this process
string Process::Command() {
  if(cmd_ != " "){
     return cmd_;}
  else{
   cmd_ = LinuxParser::Command(pid_);
   return cmd_;}
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() {
   if(user_ != " "){
      return user_;}
   else{
      user_ = LinuxParser::User(pid_);
      return user_;}
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
   return this->cpuutil_ < a.cpuutil_ ? true : false;
 }
