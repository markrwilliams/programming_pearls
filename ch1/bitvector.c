#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHUNK_TYPE uint64_t
#define CHUNK_SIZE sizeof(CHUNK_TYPE)
#define CHUNK_BITS (CHUNK_SIZE * 8)
#define MAX 10000000


struct bitarray_s {
  uint64_t max;
  uint64_t chunk_count;
  CHUNK_TYPE chunk[];
};

typedef struct bitarray_s bitarray_t;


void
bitarray_offset_remainder(uint64_t number, uint64_t *offset, CHUNK_TYPE *remainder)
{
  *offset = number / CHUNK_BITS;
  *remainder = number - (*offset * CHUNK_BITS);
}


bitarray_t *
bitarray_new(uint64_t max)
{
  assert(max > 0);

  bitarray_t *array;
  uint64_t chunk_count;
  CHUNK_TYPE remainder;

  bitarray_offset_remainder(max, &chunk_count, &remainder);

  ++chunk_count;

  if ((array = (bitarray_t *)calloc(chunk_count, sizeof(bitarray_t) + CHUNK_SIZE)) == NULL)
    return NULL;

  array->max = max;
  array->chunk_count = chunk_count;

  return array;
}


int8_t
bitarray_set(bitarray_t *array, uint64_t number)
{
  if (number > array->max)
    return 0;

  uint64_t offset;
  CHUNK_TYPE remainder;

  bitarray_offset_remainder(number, &offset, &remainder);
  array->chunk[offset] |= 1 << remainder;

  return 1;
}


int8_t
bitarray_contains(bitarray_t *array, uint64_t number)
{
  if (number > array->max)
    return 0;

  uint64_t offset;
  CHUNK_TYPE remainder, expected;

  bitarray_offset_remainder(number, &offset, &remainder);
  expected = 1 << remainder;
  return (array->chunk[offset] & expected) == expected;
}


void
bitarray_print_all(bitarray_t *array)
{
  CHUNK_TYPE expected;
  for (uint64_t offset = 0; offset < array->chunk_count; offset++) {
    for (uint64_t remainder = 0; remainder < CHUNK_BITS; remainder++) {
      expected = 1 << remainder;
      if ((array->chunk[offset] & expected) == expected)
        printf("%ld\n", CHUNK_BITS * offset + remainder);
    }
  }
}


int
main(int argc, char *argv[])
{
  bitarray_t *array = NULL;

  if ((array = bitarray_new(MAX)) == NULL)
    goto finished;

  uint32_t i, parsed_int;

  for(i = 0, parsed_int = 0; (i < MAX) && (scanf("%u", &parsed_int) == 1); i++)
    bitarray_set(array, parsed_int);

  if (errno) {
    perror("scanf");
    goto finished;
  }

  bitarray_print_all(array);

 finished:

  if (array != NULL)
    free(array);

  return 0;
}
