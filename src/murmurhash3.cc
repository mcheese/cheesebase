// Implementation of MurmurHash3 with slightly changed interface.

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.
//-----------------------------------------------------------------------------

#include "murmurhash3.h"

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)

#include <stdlib.h>

#define ROTL32(x, y) _rotl(x, y)
#define ROTL64(x, y) _rotl64(x, y)

#define BIG_CONSTANT(x) (x)

// Other compilers

#else // defined(_MSC_VER)

inline uint32_t rotl32(uint32_t x, int8_t r) {
  return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64(uint64_t x, int8_t r) {
  return (x << r) | (x >> (64 - r));
}

#define ROTL32(x, y) rotl32(x, y)
#define ROTL64(x, y) rotl64(x, y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

namespace cheesebase {

namespace {

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

uint32_t getblock(const uint32_t* p, int i) { return p[i]; }

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

uint32_t fmix(uint32_t h) {
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

//-----------------------------------------------------------------------------

uint32_t MurmurHash3_x86_32(const void* key, size_t len, uint32_t seed) {
  const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
  const int nblocks = static_cast<int>(len / 4);

  uint32_t h1 = seed;

  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t* blocks =
      reinterpret_cast<const uint32_t*>(data + nblocks * 4);

  for (int i = -nblocks; i; i++) {
    uint32_t k1 = getblock(blocks, i);

    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64;
  }

  //----------
  // tail

  const uint8_t* tail = reinterpret_cast<const uint8_t*>(data + nblocks * 4);

  uint32_t k1 = 0;

  switch (len & 3) {
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0];
    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix(h1);

  return h1;
}

} // anonymous namespace

uint32_t hashString(std::string str) {
  return MurmurHash3_x86_32(str.data(), str.size(), 0);
}

} // namespace cheesebase
