#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>


/* round columns to a multiple of this */
#define CONFIG_ROUND_COLS 0x8

/* mmap files larger than this */
#define CONFIG_LARGE_FILESIZE (256 * (1 << 20)) /* 256 megabytes */


typedef uint8_t byte;

void die(char const *s); /* hyx.c */
void pdie(char const *s); /* hyx.c */

static inline size_t min(size_t x, size_t y)
    { return x < y ? x : y; }
static inline size_t max(size_t x, size_t y)
    { return x > y ? x : y; }
static inline size_t absdiff(size_t x, size_t y)
    { return x > y ? x - y : y - x; }

static inline size_t satadd(size_t x, size_t y, size_t b)
    { assert(b >= 1); return min(b - 1, x + y); }
static inline size_t satsub(size_t x, size_t y, size_t a)
    { return x < y || x - y < a ? a : x - y; }
static inline size_t satinc(size_t x, size_t y, size_t b)
    { return x + y >= b ? x : x + y; }
static inline size_t satdec(size_t x, size_t y, size_t a)
    { return x < y || x - y < a ? x : x - y; }

unsigned long bit_length(unsigned long n);

void *malloc_strict(size_t len);
void *realloc_strict(void *ptr, size_t len);

void *mmap_strict(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
void munmap_strict(void *addr, size_t len);

off_t lseek_strict(int fildes, off_t offset, int whence);

#endif
