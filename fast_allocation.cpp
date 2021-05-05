#define FAST_ALLOCATOR_MEMORY 4e8
#ifdef FAST_ALLOCATOR_MEMORY
int allocator_pos = 0;
char allocator_memory[(int)FAST_ALLOCATOR_MEMORY];
inline void* operator new(size_t n) {
  char* res = allocator_memory + allocator_pos;
  allocator_pos += n;
  assert(allocator_pos <=
         (int)FAST_ALLOCATOR_MEMORY);
  return (void*)res;
}
inline void operator delete(void*) noexcept {}
#endif
