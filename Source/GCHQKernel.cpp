/*
  ==============================================================================

    GCHQKernel.cpp
    Created: 8 Feb 2015 10:52:39am
    Author:  fr810

  ==============================================================================
*/

#include "GCHQKernel.h"

/* Use shorter name for common vector operations */
class FVO : public FloatVectorOperations {
};

GCHQKernel::GCHQKernel( int nBlockSamples, 
			const Array<FFT::Complex> & firCoeffs ) 
  : positionInSegment(0), blockSize(nBlockSamples), fftSize((firCoeffs.size()-1)<<1),
    firFilter(firCoeffs), outputBuffer(1, fftSize), inputBuffer(1, blockSize) {
  /* clear buffers */
  outputBuffer.clear( 0, 0, outputBuffer.getNumSamples() );
  inputBuffer.clear( 0, 0, inputBuffer.getNumSamples() );
}

GCHQKernel::~GCHQKernel() {
}

void GCHQKernel::process( float * sampleBuffer, int numSamples, float gainIn, float gainOut ) {
  /* some useful variables */
  float * firIn = firFilter.getInputBuffer();
  float * firOut = firFilter.getOutputBuffer();
  float * outBuffer = outputBuffer.getWritePointer(0);
  float * inBuffer = inputBuffer.getWritePointer(0);

  /* process all samples */
  int maxSamplesToCopy = 0;
  for ( int j=0; j<numSamples; j+=maxSamplesToCopy ) {
    /* max number of samples for processing */
    maxSamplesToCopy = jmin( blockSize-positionInSegment, numSamples-j );

    /* copy input into FIR buffer and apply gainIn */
    FVO::copyWithMultiply( inBuffer+positionInSegment,sampleBuffer+j, gainIn,
			   maxSamplesToCopy );
    /* and copy output from the output buffer */
    FVO::copyWithMultiply( sampleBuffer+j, outBuffer+positionInSegment,
			   gainOut, maxSamplesToCopy );

    positionInSegment += maxSamplesToCopy;
    
    /* do we have a full block to execute the FIR? */
    if ( positionInSegment == blockSize ) {
      /* shift the output buffer so it is ready to be mixed with new data */
      for ( int k=0; k<(fftSize-blockSize); ++k ) {
	outBuffer[k] = outBuffer[k+blockSize];
      }
      FVO::fill( outBuffer+(fftSize-blockSize), 0.f, blockSize );

      if ( firFilter.finished() == true ) {
	/* mix in result with output */
	FVO::add( outBuffer, firOut, fftSize );

	/* copy in next data to process */
	FVO::copy( firIn, inBuffer, blockSize );
	
	/* schedule new FIR execution block */
	firFilter.executeAsync();
      } else {
	/* FFTW was too slow! Don't do anything,
	   buffers will be zero already */
      }

      positionInSegment = 0;
    }
  }
}
