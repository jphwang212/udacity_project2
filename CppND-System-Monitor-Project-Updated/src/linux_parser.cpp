#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
   float MemTotal, MemFree, answer{0};
   string line, key, value, mem;
   std::ifstream stream(kProcDirectory + kMeminfoFilename);
   if(stream.is_open()){
      while(std::getline(stream, line)){
         std::istringstream linestream(line);
         linestream >> key >> value >> mem;
         if(key == "MemTotal:"){
            MemTotal = stof(value)/1000.0;
         }
         else if(key == "MemFree:"){
            MemFree = stof(value)/1000.0;
         }
            
      }
      answer = ((MemTotal - MemFree)/MemTotal);
   }
   return answer;
}

// TODO: Read and return the system uptime
float LinuxParser::UpTime() {
  string active, idle;
  float total{0.0};
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
   if(stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> active >> idle;
         total = stof(active) + stof(idle);
      }
   return total;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
   string line;
   string value;
   vector<string> CpuUtil;
   std::ifstream filestream(kProcDirectory + kStatFilename);
   if(filestream.is_open()){
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while(linestream >> value){
         CpuUtil.push_back(value);
      }
   }
   std::reverse(CpuUtil.begin(), CpuUtil.end());
   CpuUtil.pop_back();
   std::reverse(CpuUtil.begin(), CpuUtil.end());
   return CpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
   string key, processes;
   string line;
   int answer{0};
   std::ifstream stream(kProcDirectory + kStatFilename);
   if(stream.is_open()){
      while(std::getline(stream, line)){
         std::istringstream linestream(line);
         while(linestream >> key){
            if(key == "processes"){
               linestream >> key;
               answer = stoi(key);
            }
         }
      }
   }
   return answer;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
   string key, proc_run;
   string line;
   int answer{0};
   std::ifstream stream(kProcDirectory + kStatFilename);
      if(stream.is_open()){
         while(std::getline(stream, line)){
            std::istringstream linestream(line);
            while(linestream >> key >> proc_run){
               if(key == "procs_running"){
                  answer = stoi(proc_run);
               }
            }
         }
      }
   return answer;
}
vector<string> LinuxParser::CpuUtilization(int pid){
   string line, value;
   int counter = 0;
   vector<string> times{};
   std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
   if(filestream.is_open()){
      while(std::getline(filestream, line)){
         std::istringstream linestream(line);
         while(linestream >> value && counter < 22){
            if(counter > 12 && counter < 17){
               times.push_back(value);
            }
            else if(counter == 21){
               times.push_back(value);
               //break;
            }
            counter++;
         }
      }
   }
   return times;
}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
   string line;
   std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
   if(filestream.is_open()){
      std::getline(filestream, line);
   }
   return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string key, value, line, ram{"0"};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
    std::istringstream linestream(line);
      while (linestream>> key >> value){
        if (key == "VmSize:"){
          ram = value;
        }   
      }  
    }
  }
  int M = std::stoi(ram)/1000;
  string ramMB = std::to_string(M);
  return ramMB; 
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
     while(linestream >> key >> value){
     if(key == "Uid:"){
        uid = value;
     }
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
         while(linestream >> key >> space >> value){
         if(value == Uid(pid)){
            user = key;
         }
         }
      }
   }
   return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
   string line, line1;
   string key;
   long value = 0;
   int counter = 0;
   std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
   if(stream.is_open()){
      std::getline(stream, line1);
      std::istringstream linestream(line1);
      while(counter < 5){
         linestream >> key;
         counter++;
      }
      value = stol(key);
      return value/sysconf(_SC_CLK_TCK);

   }
   return value/sysconf(_SC_CLK_TCK);
//return value;
}
