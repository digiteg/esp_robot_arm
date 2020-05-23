#ifndef OOP_EXT_
#define OOP_EXT_

void *operator new(size_t size) { return malloc(size); }

void *operator new[](size_t size) { return malloc(size); }

void operator delete(void *ptr) { free(ptr); }

void operator delete[](void *ptr) { free(ptr); }

#endif // OOP_EXT_