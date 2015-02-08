#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <map>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/include/map_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/io.hpp>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/convert.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/detail/endian.hpp>

#include "littleendian_binary_iarchive.hpp"
#include "wav.hpp"

#include "InWaveFile.hpp"

typedef struct {
  uint32_t chunkPosition;
  uint32_t chunkLength;
} tChunkInfo;

typedef unsigned char pcm24_t_array[3];

struct pcm24_t {
  pcm24_t_array bytes;
};

BOOST_FUSION_ADAPT_STRUCT( pcm24_t,
			   (pcm24_t_array,bytes) )
BOOST_FUSION_ADD_SERIALIZER( pcm24_t )

template <typename T>
float SampleRawRead( littleendian_binary_iarchive & ar ) {
  T data;
  ar & data;
  if ( data >= static_cast<T>(0) ) {
    return static_cast<float>(data)/static_cast<float>(std::numeric_limits<T>::max());
  } else {
    return -1.f * (static_cast<float>(data)/static_cast<float>(std::numeric_limits<T>::min()));
  }
}

template <>
float SampleRawRead<pcm24_t>( littleendian_binary_iarchive & ar ) {
  pcm24_t data;
  ar & data;

#ifdef BOOST_LITTLE_ENDIAN  
  uint32_t helper = ( data.bytes[2] << 16 ) | ( data.bytes[1] << 8 ) | ( data.bytes[0] );
#else
  uint32_t helper = ( data.bytes[0] << 16 ) | ( data.bytes[1] << 8 ) | ( data.bytes[2] );
#endif
  if ( ( helper & ( 1 << 23 ) ) == 0 ) {
    return static_cast<float>(helper)/static_cast<float>(( 1 << 23 ) - 1);
  } else {
    helper = ( 1 << 23 ) - ( helper & ( ( 1 << 23 ) - 1 ) );
    return -1.f * (static_cast<float>(helper)/static_cast<float>(1 << 23));
  }
}

class InWaveFileImpl {
private:
  std::istream & mFile;
  littleendian_binary_iarchive mAr;
  struct InfoChunkStruct mInfo;
  unsigned int mNumSamples, mCurrentSample;
public:
  InWaveFileImpl( std::istream & in_file ) : mFile(in_file), mAr(mFile) {
    RiffChunkStruct riff;
    mAr & riff;
    
    if ( ( std::string(riff.id, 4) != "RIFF" ) || ( std::string( riff.fileType, 4 ) != "WAVE" ) ) {
      throw std::runtime_error( "Unknown wav file type" );
    }
    
    std::map<std::string, tChunkInfo> chunk_map;
    {
      uint32_t pos = 12;
      while ( pos < (riff.length+12-8) ) {
	mFile.seekg(pos);
	ChunkStruct chunk;
	mAr & chunk;
	tChunkInfo info;
	/* include riff header */
	info.chunkPosition = pos;
	info.chunkLength = chunk.length;
	chunk_map[std::string(chunk.id,4)] = info;
	pos += chunk.length + 8;
      }
    }
    
    {
      std::map<std::string, tChunkInfo>::const_iterator it;
      if ( ( it = chunk_map.find( std::string("fmt ") ) ) != chunk_map.end() ) {
	if ( it->second.chunkLength < 16 ) {
	  throw std::runtime_error( "Wav file format error" );
	}
	mFile.seekg(it->second.chunkPosition);
	mAr & mInfo;
	if ( mInfo.wFormatTag != 1 ) {
	  throw std::runtime_error( "Wav file format error: I only understand uncompressed wav files" );
	}
	if ( ( mInfo.wBitsPerSample != 8 ) && ( mInfo.wBitsPerSample != 16 ) && ( mInfo.wBitsPerSample != 24 ) && ( mInfo.wBitsPerSample != 32 ) ) {
	  std::string error;
	  {
	    std::stringstream ss;
	    ss << "Wav file format error: " << mInfo.wBitsPerSample << "-bits detected: I only understand 8-, 16-, 32-bit audio files.";
	    error = ss.str();
	  }
	  throw std::runtime_error( error.c_str() );
	}
      } else {
	throw std::runtime_error( "Wav file format error: Unable to find info chunk" );
      }
    }
    
    {
      std::map<std::string, tChunkInfo>::const_iterator it;
      if ( ( it = chunk_map.find( std::string("data") ) ) != chunk_map.end() ) {
	mNumSamples = it->second.chunkLength / ( mInfo.nChannels * ( mInfo.wBitsPerSample / 8 ) );
	mCurrentSample = 0;
	mFile.seekg(it->second.chunkPosition+8);
      } else {
	throw std::runtime_error( "Wav file format error: Unable to find info chunk" );
      }
    } 
  }
  ~InWaveFileImpl() {}

  int channels() const {
    return mInfo.nChannels;
  }

  float sampleRate() const {
    return mInfo.nSamplesPerSec;
  }

  int bitsPerSample() const {
    return mInfo.wBitsPerSample;
  }

  bool read( float * sampleBuffer ) {
    if ( mCurrentSample < mNumSamples ) {
      mCurrentSample++;
      for ( unsigned int i=0; i<mInfo.nChannels; ++i ) {
	float sample;
	switch ( mInfo.wBitsPerSample ) {
	case 8:
	  sample = SampleRawRead<int8_t>(mAr);
	  break;
	case 16:
	  sample = SampleRawRead<int16_t>(mAr);
	  break;
	case 24:
	  sample = SampleRawRead<pcm24_t>(mAr);
	  break;
	case 32:
	  sample = SampleRawRead<int32_t>(mAr);
	  break;
	default:
	  throw std::runtime_error("Ugghh!");
	}
	sampleBuffer[i] = sample;
      }
    }
    return ( mCurrentSample < mNumSamples );
  }
};

InWaveFile::InWaveFile( std::istream & wav_file ) 
  : pImpl( new InWaveFileImpl(wav_file) ) {}

InWaveFile::~InWaveFile() {
  delete pImpl;
}

int InWaveFile::channels() const {
  return pImpl->channels();
}

float InWaveFile::sampleRate() const {
  return pImpl->sampleRate();
}

int InWaveFile::bitsPerSample() const {
  return pImpl->bitsPerSample();
}

bool InWaveFile::read( float * sampleBuffer ) {
  return pImpl->read(sampleBuffer);
}
