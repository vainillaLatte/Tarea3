#ifndef Heap_h
#define Heap_h
#define MAXHEAP 100
#define Max 31


typedef struct nodo
{
  void* data;
  int priority;
} heapElem;

typedef struct Heap
{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;


Heap* createHeap();
void* heap_top(Heap* pq);
void heap_push(Heap* pq, void* data, int priority);
void heap_pop(Heap* pq);
void aumentarCapacidad(Heap* pq);
void heapify(heapElem* arr, int n, int i);
void swap(heapElem* a, heapElem* b);
#endif /* HashMap_h */