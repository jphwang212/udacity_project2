#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "linux_parser.h"
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float previdle{0}, prevtotal{0}, prevtime{0};
  float cpuutil_;
};

#endif
