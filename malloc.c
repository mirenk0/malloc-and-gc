#include "malloc.h"
#include <unistd.h>

#define align4(x) ((((x) - 1) >> 2 << 2) + 4)
#define BLOCK_SIZE 20

typedef struct s_block *t_block;

struct s_block {
  size_t size;
  t_block next;
  t_block prev;
  int free;
  void *ptr;
  char data[1];
};

void *base = NULL;

// Internal helpers
t_block find_block(t_block *last, size_t size);
t_block extend_heap(t_block last, size_t size);
void split_block(t_block b, size_t size);
t_block fusion(t_block b);
int valid_addr(void *p);
t_block get_block(void *p);
void copy_block(t_block src, t_block dst);

void *malloc(size_t size) {
  t_block b, last;
  size_t s = align4(size);
  if (base) {
    last = base;
    b = find_block(&last, s);
    if (b) {
      if ((b->size - s) >= (BLOCK_SIZE + 4))
        split_block(b, s);
      b->free = 0;
    } else {
      b = extend_heap(last, s);
      if (!b)
        return NULL;
    }
  } else {
    b = extend_heap(NULL, s);
    if (!b)
      return NULL;
    base = b;
  }
  return b->data;
}

void free(void *p) {
  if (valid_addr(p)) {
    t_block b = get_block(p);
    b->free = 1;
    if (b->prev && b->prev->free)
      b = fusion(b->prev);
    if (b->next)
      fusion(b);
    else {
      if (b->prev)
        b->prev->next = NULL;
      else
        base = NULL;
      brk(b);
    }
  }
}

void *calloc(size_t number, size_t size) {
  size_t *new;
  size_t s4, i;
  new = malloc(number * size);
  if (new) {
    s4 = align4(number * size) >> 2;
    for (i = 0; i < s4; i++)
      new[i] = 0;
  }
  return new;
}

void *realloc(void *p, size_t size) {
  if (!p)
    return malloc(size);
  if (!valid_addr(p))
    return NULL;
  size_t s = align4(size);
  t_block b = get_block(p);
  if (b->size >= s) {
    if (b->size - s >= (BLOCK_SIZE + 4))
      split_block(b, s);
  } else {
    if (b->next && b->next->free &&
        (b->size + BLOCK_SIZE + b->next->size) >= s) {
      fusion(b);
      if (b->size - s >= (BLOCK_SIZE + 4))
        split_block(b, s);
    } else {
      void *newp = malloc(s);
      if (!newp)
        return NULL;
      t_block newb = get_block(newp);
      copy_block(b, newb);
      free(p);
      return newp;
    }
  }
  return p;
}

void *reallocf(void *p, size_t size) {
  void *newp = realloc(p, size);
  if (!newp)
    free(p);
  return newp;
}

// Internal functions

t_block find_block(t_block *last, size_t size) {
  t_block b = base;
  while (b && !(b->free && b->size >= size)) {
    *last = b;
    b = b->next;
  }
  return b;
}

t_block extend_heap(t_block last, size_t size) {
  t_block b = sbrk(0);
  if ((intptr_t)sbrk(BLOCK_SIZE + size) == -1)
    return NULL;
  b->size = size;
  b->next = NULL;
  b->prev = last;
  b->free = 0;
  b->ptr = b->data;
  if (last)
    last->next = b;
  return b;
}

void split_block(t_block b, size_t size) {
  t_block newb = (t_block)(b->data + size);
  newb->size = b->size - size - BLOCK_SIZE;
  newb->next = b->next;
  newb->prev = b;
  newb->free = 1;
  newb->ptr = newb->data;
  b->size = size;
  b->next = newb;
  if (newb->next)
    newb->next->prev = newb;
}

t_block fusion(t_block b) {
  if (b->next && b->next->free) {
    b->size += BLOCK_SIZE + b->next->size;
    b->next = b->next->next;
    if (b->next)
      b->next->prev = b;
  }
  return b;
}

t_block get_block(void *p) { return (t_block)((char *)p - BLOCK_SIZE); }

int valid_addr(void *p) {
  if (base && p > base && p < sbrk(0)) {
    return (p == get_block(p)->ptr);
  }
  return 0;
}

void copy_block(t_block src, t_block dst) {
  int *sdata = src->ptr;
  int *ddata = dst->ptr;
  for (size_t i = 0; i * 4 < src->size && i * 4 < dst->size; i++)
    ddata[i] = sdata[i];
}
