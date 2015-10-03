// Licensed under the Apache License 2.0 (see LICENSE file).

// Common types, constants and functions.

#pragma once

#include <cstdint>

using byte = unsigned char;

namespace cheesebase {

// size of a memory page
const size_t k_page_size_power{ 14 };
const size_t k_page_size{ 1u << k_page_size_power };

constexpr uint64_t page_nr(const uint64_t addr) noexcept
{
  return addr >> k_page_size_power;
};

constexpr uint64_t page_offset(const uint64_t addr) noexcept
{
  return addr & static_cast<uint64_t>(k_page_size - 1);
};

}
