#pragma once
#include <iostream>

class InWaveFileImpl;

class InWaveFile {
private:
  InWaveFileImpl * pImpl;
public:
  InWaveFile( std::istream & wav_file );
  ~InWaveFile();
  int channels() const;
  float sampleRate() const;
  int bitsPerSample() const;
  bool read( float * sampleBuffer );
};
