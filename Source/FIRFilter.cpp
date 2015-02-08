/*
  ==============================================================================

    FIRFilter.cpp
    Created: 8 Feb 2015 11:05:27am
    Author:  fr810

  ==============================================================================
*/

#include "FIRFilter.h"

FIRFilter::FIRFilter( const Array<FFT::Complex> & coeffecients )
  : Thread("FIRFilter"), firCoeffs(coeffecients),
    forward((coeffecients.size()-1)<<1,true),
    backward((coeffecients.size()-1)<<1,false),
    isProcessing( false ) {
  startThread();
}

FIRFilter::~FIRFilter() {
  signalThreadShouldExit();
  triggerEvent.signal();
  stopThread(-1);
}

void FIRFilter::run() {
  while ( true ) {
    while ( ( isProcessing == false ) && ( threadShouldExit() == false ) ) {
      triggerEvent.wait();
    }
    if ( threadShouldExit() == true ) {
      return;
    }
    
    execute();
    isProcessing = false;
  }
}

void FIRFilter::executeAsync() {
  isProcessing = true;
  triggerEvent.signal();
}

void FIRFilter::execute() {
  /* some useful variables */
  int numCoeffs = forward.getComplexSize();
  const FFT::Complex* src = forward.getComplexBuffer();
  const FFT::Complex* fir = firCoeffs.begin();
  FFT::Complex* dst = backward.getComplexBuffer();

  /* do the convolution in Fourier-Space */
  forward.execute();
  for ( int k=0; k<numCoeffs; ++k ) {
    dst[k].r = (src[k].r*fir[k].r)-(src[k].i*fir[k].i);
    dst[k].i = (src[k].r*fir[k].i)+(src[k].i*fir[k].r);
  }
  backward.execute();
}
