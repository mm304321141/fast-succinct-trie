#include <cstdio>
#include <typeinfo>
#include <vector>
#include <cstring>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>

template<typename ValueType> 
/**
 * Custom allocator for all the indexes except array index.
 */
class AllocatorTracker : public std::allocator<ValueType> {
public:
  typedef typename std::allocator<ValueType> BaseAllocator;
  typedef typename BaseAllocator::pointer pointer;
  typedef typename BaseAllocator::size_type size_type;

  int64_t *memory_size;

  AllocatorTracker() throw() : BaseAllocator() {}

  AllocatorTracker(int64_t* m_ptr) throw() : BaseAllocator() {
    memory_size = m_ptr;
  }
  AllocatorTracker(const AllocatorTracker& allocator) throw() : BaseAllocator(allocator) {
    memory_size = allocator.memory_size;
  }
  template <class U> AllocatorTracker(const AllocatorTracker<U>& allocator) throw(): BaseAllocator(allocator) {
    memory_size = allocator.memory_size;
  }

  ~AllocatorTracker() {}

  template<class U> struct rebind {
    typedef AllocatorTracker<U> other;
  };

  pointer allocate(size_type size) {
    pointer dataPtr = BaseAllocator::allocate(size);
    *memory_size += size * sizeof(ValueType);
    return dataPtr;
  }

  pointer allocate(size_type size, void* ptr) {
    pointer dataPtr = BaseAllocator::allocate(size, ptr);
    *memory_size += size * sizeof(ValueType);
    return dataPtr;
  }

  pointer allocate(size_type size, pointer ptr) {
    pointer dataPtr = BaseAllocator::allocate(size, ptr);
    *memory_size += size * sizeof(ValueType);
    return dataPtr;
  }

  void deallocate(pointer ptr, size_type size) throw() {
    BaseAllocator::deallocate(ptr, size);
    *memory_size -= size * sizeof(ValueType);
  }
};
