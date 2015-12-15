// Licensed under the Apache License 2.0 (see LICENSE file).

#pragma once

#include "common.h"
#include "compiler.h"
#include <gsl.h>

namespace cheesebase {

enum class BlockType {
  pg = 'P', // 1 page (4k)
  t1 = '1', // 1/2 page (2k)
  t2 = '2', // 1/4 page (1k)
  t3 = '3', // 1/8 page (512)
  t4 = '4'  // 1/16 page (256)
};

static size_t toBlockSize(BlockType t) {
  switch (t) {
  case BlockType::pg: return k_page_size;
  case BlockType::t1: return k_page_size / 2;
  case BlockType::t2: return k_page_size / 4;
  case BlockType::t3: return k_page_size / 8;
  case BlockType::t4: return k_page_size / 16;
  default: throw ConsistencyError("Invalid block type");
  }
}

CB_PACKED(struct DskBlockHdr {
  DskBlockHdr() = default;
  DskBlockHdr(BlockType type, Addr next) {
    auto type_value = static_cast<uint64_t>(type);
    Expects(type_value <= 0xff);
    Expects(next < ((uint64_t)1 << 56));
    data = (type_value << 56) + (next & (((uint64_t)1 << 56) - 1));
  };

  PageNr next() const noexcept { return (data & (((uint64_t)1 << 56) - 1)); }

  BlockType type() const noexcept {
    return gsl::narrow_cast<BlockType>(data >> 56);
  }

  uint64_t data;
});

const uint64_t magic = *((uint64_t const*)"CHSBSE01");

CB_PACKED(struct DskDatabaseHdr {
  uint64_t magic;
  uint64_t end_of_file;
  uint64_t free_alloc_pg;
  uint64_t free_alloc_t1;
  uint64_t free_alloc_t2;
  uint64_t free_alloc_t3;
  uint64_t free_alloc_t4;
});
static_assert(sizeof(DskDatabaseHdr) <= k_page_size / 2,
              "Database header should be smaller than half of the page size");

CB_PACKED(struct DskKey {
  DskKey(uint32_t h, uint16_t i) : hash(h), index(i) {};
  uint32_t hash;
  uint16_t index;

  bool operator!=(const DskKey& o) {
    return this->hash != o.hash || this->index != o.index;
  }
  bool operator==(const DskKey& o) {
    return !(*this != o);
  }
});
static_assert(sizeof(DskKey) == 6, "Invalid disk key size");

CB_PACKED(using DskKeyCacheSize = uint16_t);
static_assert(sizeof(DskKeyCacheSize) == 2, "Invalid disk key cache size size");

CB_PACKED(struct DskValueHdr {
  uint8_t magic_byte;
  uint8_t type;
});
static_assert(sizeof(DskValueHdr) == 2, "Invalid disk value header size");

CB_PACKED(struct DskPair {
  DskKey key;
  DskValueHdr value;
});
static_assert(sizeof(DskPair) == 8, "invalid disk pair size");

} // namespace cheesebase
