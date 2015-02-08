#pragma once
#include <stdint.h>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "fusion_map_serialization.hpp"

/* Wav file format */

typedef char tFourCC[4];
struct ChunkStruct {
  tFourCC id;
  uint32_t length;
};

BOOST_FUSION_ADAPT_STRUCT( ChunkStruct,
			   (tFourCC,id)
			   (uint32_t,length) )
BOOST_FUSION_ADD_SERIALIZER( ChunkStruct )

struct RiffChunkStruct : public ChunkStruct {
  tFourCC fileType;
};

BOOST_FUSION_ADAPT_STRUCT( RiffChunkStruct,
			   (tFourCC,id)
			   (uint32_t,length)
			   (tFourCC,fileType) )
BOOST_FUSION_ADD_SERIALIZER( RiffChunkStruct )


struct InfoChunkStruct : public ChunkStruct {
  uint16_t wFormatTag;
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

BOOST_FUSION_ADAPT_STRUCT( InfoChunkStruct,
			   (tFourCC,id)
			   (uint32_t,length)
			   (uint16_t,wFormatTag)
			   (uint16_t,nChannels)
			   (uint32_t,nSamplesPerSec)
			   (uint32_t,nAvgBytesPerSec)
			   (uint16_t,nBlockAlign)
			   (uint16_t,wBitsPerSample))
BOOST_FUSION_ADD_SERIALIZER( InfoChunkStruct )





