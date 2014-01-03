#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 10000000

int
comparer(const void *v_a, const void *v_b)
{
  const uint32_t *a = v_a, *b = v_b;
  return *a == *b ? 0 : (*a < *b ? -1 : 1);
}


int
main(int argc, char *argv[])
{
  uint32_t *incoming = calloc(MAX, sizeof(uint32_t)), i;

  if (!incoming)
    goto finished;

  for(i = 0; (i < MAX) && (scanf("%u", &incoming[i]) == 1); i++);

  if (errno) {
    perror("scanf");
    goto finished;
  }

  qsort(incoming, i, sizeof(uint32_t), comparer);

  for(int32_t j = 0; j < i; j++)
    printf("%d\n", incoming[j]);

 finished:
  if (incoming)
    free(incoming);

  return 0;
}
