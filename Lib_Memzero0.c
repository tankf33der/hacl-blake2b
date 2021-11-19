#if __has_include("config.h")
#include "config.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#if (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__)
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#endif

#ifdef __FreeBSD__
#include <strings.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>

#include "Lib_Memzero0.h"
#include "kremlin/internal/target.h"


typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint32_t u32;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint64_t u64;
static u32 load32_le_p(u8 *s)
{
    return (u32)s[0]
        | ((u32)s[1] <<  8)
        | ((u32)s[2] << 16)
        | ((u32)s[3] << 24);
}

static u64 load64_le_p(u8 *s)
{
    return load32_le_p(s) | ((u64)load32_le_p(s+4) << 32);
}
static void store32_le_p(u8 out[4], u32 in)
{
    out[0] =  in        & 0xff;
    out[1] = (in >>  8) & 0xff;
    out[2] = (in >> 16) & 0xff;
    out[3] = (in >> 24) & 0xff;
}

static void store64_le_p(u8 out[8], u64 in)
{
    store32_le_p(out    , (u32)in );
    store32_le_p(out + 4, in >> 32);
}



/* The F* formalization talks about the number of elements in the array. The C
   implementation wants a number of bytes in the array. KreMLin is aware of this
   and inserts a sizeof multiplication. */
void Lib_Memzero0_memzero(void *dst, uint64_t len) {
  /* This is safe: kremlin checks at run-time (if needed) that all object sizes
     fit within a size_t, so the size we receive has been checked at
     allocation-time, possibly via KRML_CHECK_SIZE, to fit in a size_t. */
  size_t len_ = (size_t) len;

  #ifdef _WIN32
    SecureZeroMemory(dst, len);
  #elif defined(__APPLE__) && defined(__MACH__)
    memset_s(dst, len_, 0, len_);
  #elif (defined(__linux__) && !defined(LINUX_NO_EXPLICIT_BZERO)) || defined(__FreeBSD__)
    explicit_bzero(dst, len_);
  #elif defined(__NetBSD__)
    explicit_memset(dst, 0, len_);
  #else
    /* Default implementation for platforms with no particular support. */
    #warning "Your platform does not support any safe implementation of memzero -- consider a pull request!"
    volatile unsigned char *volatile dst_ = (volatile unsigned char *volatile) dst;
    size_t i = 0U;
    while (i < len)
      dst_[i++] = 0U;
  #endif
}
