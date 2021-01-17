#include <string>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string hours, minutes, sec;
  long hour = seconds/3600;
  hour = hour%99;
  seconds = seconds%3600;
  long minute = seconds/60;
  seconds = seconds%60;
  if(hour < 10){
    hours = "0" + std::to_string(hour);
  }
  else
    hours = std::to_string(hour);
  if(minute < 10){
    minutes = "0" + std::to_string(minute);
  }
  else
    minutes = std::to_string(minute);
  if(seconds < 10){
    sec = "0" + std::to_string(seconds);
  }
  else
    sec = std::to_string(seconds);

  return (hours + ":" + minutes + ":" + sec);
 }
