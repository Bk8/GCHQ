/*
  ==============================================================================

    GCHQKernel.h
    Created: 8 Feb 2015 10:52:39am
    Author:  fr810

  ==============================================================================
*/

#ifndef GCHQKERNEL_H_INCLUDED
#define GCHQKERNEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FIRFilter.h"

//==============================================================================
/**
   Kernel for a single audio channel
*/
class GCHQKernel  {
public:
  GCHQKernel( int nBlockSamples,
	      const Array<FFT::Complex> & firCoeffs );
  ~GCHQKernel();
  void process( float * sampleBuffer, int numSamples, float gainIn, float gainOut );
private:
  int positionInSegment;
  int blockSize, fftSize;
  FIRFilter firFilter;
  AudioSampleBuffer outputBuffer, inputBuffer;
};


#endif  // GCHQKERNEL_H_INCLUDED
