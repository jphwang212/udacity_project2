#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

#include "linux_parser.h"

using std::stol;
using std::stoi;
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
  float MemTotal, MemFree;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if(stream.is_open()){
      while(std::getline(stream, line));
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal"){
         MemTotal = stof(value);
      }
      else if(key == "MemFree"){
         MemFree = stof(value);
      }
    }
  return ((MemTotal - MemFree)/MemTotal); }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long active, idle;
  string line, a1, a2;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
    if(stream.is_open()){
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), '.', ' ');
      std::istringstream linestream(line);
      linestream >> active >> a1 >> idle >> a2;
      }
   return (active + idle)/10;
   }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
   long answer;
   for(int i = 0; i < 8; i++){
     answer += stol(LinuxParser::CpuUtilization()[i]);
   }
    return answer;
   }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
   string line;
   string value;
   int counter = 0;
   long jiffies_proc = 0;
   std::ifstream filestream(kProcDirectory+ to_string(pid) + kStatFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
     while(linestream >> value){
       if(counter > 12 && counter < 18){
         jiffies_proc += stol(value);
       }
       else if(counter == 22){
         jiffies_proc += stol(value);
       }
       counter++;
     }
    }
   return jiffies_proc;
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
   long answer;
    for(int i = 0; i < 8; i++){
       if(i != 3 && i != 4){
          answer += stol(LinuxParser::CpuUtilization()[i]);
       }
    }
return  answer; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
   return stol(LinuxParser::CpuUtilization()[3]) + stol(LinuxParser::CpuUtilization()[4]); }

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
          return stoi(processes);
       }
    }
    }
    return stoi(processes);
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
   string key, proc_run;
   string line;
   std::ifstream stream(kProcDirectory + kStatFilename);
      if(stream.is_open()){
      while(std::getline(stream, line)){
         std::istringstream linestream(line);
         while(linestream >> key >> proc_run){
         if(key == "procs_running"){
             stream.close();
            return stoi(proc_run);
         }
         }
       }
       }
return stoi(proc_run);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
   string line;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
   if(filestream.is_open()){
      std::getline(filestream, line);
      filestream.close();
   }
 return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
   string line;
   string key, value, ram;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
   if(filestream.is_open()){
   while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:"){
         ram = value;
      }
   }
   }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
   string line;
   string key, value, uid;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
   if(filestream.is_open()){
     std::getline(filestream, line);
     std::istringstream linestream(line);
     linestream >> key >> value;
     if(key == "Uid:"){
        uid = value;
     }
   }
return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
   string line;
   string key, value, space, user;
   std::ifstream filestream(kPasswordPath);
   if(filestream.is_open()){
      while(std::getline(filestream, line)){
         std::replace(line.begin(), line.end(), ':', ' ');
         std::istringstream linestream(line);
         linestream >> key >> space >> value;
         if(stoi(value) == pid){
            user = key;
         }
         else break;
      }
   }
return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
   string line;
   string key;
   long value;
   int counter = 0;
   std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
   if(stream.is_open()){
     std::getline(stream, line);
     std::istringstream linestream(line);
     while(linestream >> key){
       if(counter == 23){
         value = stol(key);
       }
      counter++;
     }
     return (UpTime() - (value/sysconf(_SC_CLK_TCK)));
   }
return 0;
}
