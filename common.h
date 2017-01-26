#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>


/* round columns to a multiple of this */
#define CONFIG_ROUND_COLS 0x8

/* mmap files larger than this */
#define CONFIG_LARGE_FILESIZE (256 * (1 << 20)) /* 256 megabytes */

/* microseconds to wait for the rest of what could be an escape sequence */
#define CONFIG_WAIT_ESCAPE (10000) /* 10 milliseconds */


typedef uint8_t byte;

void die(char const *s) __attribute__((noreturn)); /* hyx.c */
void pdie(char const *s) __attribute__((noreturn)); /* hyx.c */

static inline size_t min(size_t x, size_t y)
    { return x < y ? x : y; }
static inline size_t max(size_t x, size_t y)
    { return x > y ? x : y; }
static inline size_t absdiff(size_t x, size_t y)
    { return x > y ? x - y : y - x; }

/* FIXME This feels way too complicated. Perhaps we can simplify the
 * arithmetic by restructuring the cursor movement code in input.c? */

/* returns x + wz, where w is the largest integer such that x + wz <= min(x + yz, b - 1) */
static inline size_t sataddstep(size_t x, size_t y, size_t z, size_t b)
{
    assert(z); assert(z <= b);
    if (x < b)
        return x + min(y, (b - x - 1) / z) * z;
    return x - (x - b + z) / z * z;
}

/* returns x - wz, where w is the largest integer such that x - wz >= max(x - yz, a) */
static inline size_t satsubstep(size_t x, size_t y, size_t z, size_t a)
{
    assert(z);
    if (x >= a)
        return x - min(y, (x - a) / z) * z;
    return x + (a - x + z - 1) / z * z;
}

static inline size_t satadd(size_t x, size_t y, size_t b)
    { return sataddstep(x, y, 1, b); }
static inline size_t satsub(size_t x, size_t y, size_t a)
    { return satsubstep(x, y, 1, a); }
static inline size_t satinc(size_t x, size_t y, size_t b)
    { return sataddstep(x, 1, y, b); }
static inline size_t satdec(size_t x, size_t y, size_t a)
    { return satsubstep(x, 1, y, a); }

unsigned long bit_length(unsigned long n);

void *malloc_strict(size_t len);
void *realloc_strict(void *ptr, size_t len);

void *mmap_strict(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
void munmap_strict(void *addr, size_t len);

off_t lseek_strict(int fildes, off_t offset, int whence);

char *fgets_retry(char *s, int size, FILE *stream);

#endif
