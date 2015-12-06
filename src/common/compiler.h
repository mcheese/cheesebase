// Licensed under the Apache License 2.0 (see LICENSE file).

#pragma once

#ifdef _MSC_VER

#define CB_PACKED(...) __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop))

#else

#define CB_PACKED(...) __VA_ARGS__ __attribute__((__packed__))

#endif
