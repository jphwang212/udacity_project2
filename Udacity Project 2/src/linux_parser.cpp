#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal, MemFree, buffer;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if(stream.is_open()){
      while(std::getline(stream, line));
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal:"){
         MemTotal = stof(value);
      }
      else if(key == "MemFree:"){
         MemFree = stof(value);
      }
      else if(key == "Buffers:"){
         buffer = stof(value);
         linestream.clear();
      }
    }
    stream.close();
  return (1.0 - (MemFree / (MemTotal - buffer))); }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string TimeSys;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
    if(stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> TimeSys;
      stream.close();
      return stol(TimeSys);
      }
return 0;
   }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
   string line;
   string key, value;
   long answer = 0;
   std::ifstream filestream(kProcDirectory + kStatFilename);
   if(filestream.is_open()){
     std::getline (filestream, line);
     std::istringstream linestream(line);
     linestream >> key;
     while(linestream >> value){
        answer += stol(value);
     }
   return answer;
   }
   return 0;
   }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
   string line;
   string jif;
   string key, value;
   long jiffies_proc = 0;
   std::ifstream filestream(kProcDirectory+ to_string(pid) + kStatFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
    for(int i = 0; i < 14; i++){
       linestream >> key;
    }
    for(int i = 0; i < 4; i++){
       linestream >> value;
       jiffies_proc += stol(value);
    }
    return jiffies_proc;
   }
   return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
   string line;
   string key, value;
   long jif_active = 0;
   vector<int> jiffies = {};
   std::ifstream filestream(kProcDirectory + kStatFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
     linestream >> key;
     while(linestream >> value){
       jiffies.push_back(stoi(value));
     }
    }
    for(int i = 0; i < 8; i++){
       jif_active += jiffies[i];
    }
return  jif_active; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
   string line;
   string key, value;
   long jif_idle = 0;
   vector<int> jiffies;
   std::ifstream filestream(kProcDirectory + kStatFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
     linestream >> key;
    while(linestream >> value){
      jiffies.push_back(stoi(value));
    }
   }
   jif_idle = jiffies.at(3) + jiffies.at(4);
return jif_idle; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
   string line;
   string key, value;
   vector<string> CpuUtil;
   std::ifstream filestream(kProcDirectory + kStatFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
       linestream >> key;
       while(linestream >> value){
          CpuUtil.push_back(value);
          return CpuUtil;
       }
     filestream.close();
    }
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, processes;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
    if(stream.is_open()){
    while(std::getline(stream, line)){
       std::istringstream linestream(line);
       linestream >> key >> processes;
       if(key == "processes"){
          stream.close();
          return stoi(processes);
       }
    }
    }
    stream.close();
    return 0;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
   string key, proc_run;
   string line;
   std::ifstream stream(kProcDirectory + kStatFilename);
      if(stream.is_open()){
      while(std::getline(stream, line)){
         std::istringstream linestream(line);
         while(linestream >> key >> proc_run);
         if(key == "procs_running"){
             stream.close();
            return stoi(proc_run);
         }
       }
       }
stream.close();
return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
   string line;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
   if(filestream.is_open()){
      std::getline(filestream, line);
   }
   filestream.close();
 return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
   string line;
   string key, value;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
   if(filestream.is_open()){
   while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:"){
         return value;
      }
   }
   }
filestream.close();
return {};
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
   string line;
   string key, value;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
     linestream >> key >> value;
     if(key == "Uid:"){
        return value;
     }
   }
filestream.close();
return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
   string line;
   string key, value, space;
   std::ifstream filestream(kPasswordPath);
   if(filestream.is_open()){
      while(std::getline(filestream, line)){
         std::replace(line.begin(), line.end(), ':', ' ');
         std::istringstream linestream(line);
         linestream >> key >> space >> value;
         if(stoi(value) == pid){
            return key;
         }
      }
   }
filestream.close();
return {};
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
   string line;
   string hold1, hold2, hold3, hold4;
   string value;
   std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
   if(stream.is_open()){
     std::getline(stream, line);
     std::istringstream linestream(line);
     linestream >> hold1 >> hold2 >> hold3 >> hold4 >> value;
     return stol(value);
   }
stream.close();
return 0;
}
