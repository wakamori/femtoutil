#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lispo.h"

void *allocate(long unsigned int size)
{
	void *ret;
	if(heap_counter < HEAP_SIZE / 2) {
		if(heap_counter + size >= HEAP_SIZE / 2)
			GC();
		ret = heap[heap_counter];
		heap_counter += size;
		return ret;
	}
	else {
		if(heap_counter + size >= HEAP_SIZE)
			GC();
		ret = heap[heap_counter];
		heap_counter += size;
		return ret;
	}
}

void GC(void)
{
}
