/*
  ==============================================================================

    FIRFilter.h
    Created: 8 Feb 2015 11:05:27am
    Author:  fr810

  ==============================================================================
*/

#ifndef FIRFILTER_H_INCLUDED
#define FIRFILTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FFTW.h"

//==============================================================================
/**
   Asynchronous FIR Filter
*/
class FIRFilter : public Thread {
public:
  FIRFilter( const Array<FFT::Complex> & coeffecients );
  ~FIRFilter();

  void executeAsync();
  void execute();

  inline bool finished() const {
    return ( isProcessing == false );
  }

  inline float *getInputBuffer() {
    return forward.getRealBuffer();
  }

  inline float *getOutputBuffer() {
    return backward.getRealBuffer();
  }
  
protected:
  void run() override;

private:
  const Array<FFT::Complex>& firCoeffs;
  FFTW forward, backward;
  WaitableEvent triggerEvent;
  bool isProcessing;
};



#endif  // FIRFILTER_H_INCLUDED
