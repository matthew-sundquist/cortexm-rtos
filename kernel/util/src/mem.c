
#include <stddef.h> 
#include "mem.h"

void *memcpy(void *dest, const void *src, size_t n)
{
  unsigned char *d = dest;
  const unsigned char *s = src;

  while (n--)
  {
    *d++ = *s++;
  }

  return dest;
}

void *memset(void *ptr, int value, size_t n)
{
    unsigned char *p = ptr;

    while (n--)
    {
        *p++ = (unsigned char)value;
    }

    return ptr;
}
