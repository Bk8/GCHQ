#pragma once

#include <boost/detail/endian.hpp>

namespace internal {
  
  /* endian conversion helper */
  template <typename T>
  T swap_endian(T u) {
#ifdef BOOST_LITTLE_ENDIAN
    return u;
#else
    union
    {
      T u;
      unsigned char u8[sizeof(T)];
    } source, dest;
    
    source.u = u;
    
    for (size_t k = 0; k < sizeof(T); k++)
      dest.u8[k] = source.u8[sizeof(T) - k - 1];
    
    return dest.u;
#endif
  }

}

