/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "FFTW.h"

//==============================================================================

FFTW::FFTW( unsigned int time_domain_size, bool forwardDirection )
  : size(time_domain_size), realBuffer(nullptr), complexBuffer(nullptr), plan(nullptr) {
  realBuffer = (float*)fftwf_malloc( sizeof(float)*size );
  if ( realBuffer == nullptr ) {
    throw std::bad_alloc();
  }
  
  complexBuffer = (FFT::Complex*)fftwf_malloc( sizeof(FFT::Complex) * ((size>>1)+1) );
  if ( complexBuffer == nullptr ) {
    throw std::bad_alloc();
  }

  if ( forwardDirection == true ) {
    plan = fftwf_plan_dft_r2c_1d(size, realBuffer, (fftwf_complex*)complexBuffer, FFTW_ESTIMATE);
  } else {
    plan = fftwf_plan_dft_c2r_1d(size, (fftwf_complex*)complexBuffer, realBuffer, FFTW_ESTIMATE);
  }
  if ( plan == nullptr ) {
    throw std::bad_alloc();
  }

  /* zero-out buffers */
  std::fill( realBuffer, realBuffer+size, 0.f );
  std::fill( complexBuffer, complexBuffer+((size>>1)+1), FFT::Complex({0.f,0.f}) );
}

FFTW::~FFTW() {
  if ( plan != nullptr ) {
    fftwf_destroy_plan(plan);
    plan = nullptr;
  }
  if ( complexBuffer != nullptr ) {
    fftwf_free(complexBuffer);
    complexBuffer = nullptr;
  }
  if ( realBuffer != nullptr ) {
    fftwf_free(realBuffer);
    realBuffer = nullptr;
  }
}

