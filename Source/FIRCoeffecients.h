#ifndef FIRCOEFFECIENTS_H_INCLUDED
#define FIRCOEFFECIENTS_H_INCLUDED

typedef struct {
  const char * programName;
  int numberOfCoeffecients;
  float* data;
} tFIRProgram;

typedef struct {
  int numPrograms;
  tFIRProgram* programs;
} tFIRCoeffecients;

extern tFIRCoeffecients kFIRPrograms;

#endif // FIRCOEFFECIENTS_H_INCLUDED
