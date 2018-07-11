
#include <new>
#include <dsim/memory.h>
#if 0
void *
operator new( std::size_t count ) throw(std::bad_alloc)
{
  return ds_heap_alloc( count );
}

void *
operator new( std::size_t count, const std::nothrow_t&t ) throw()
{
  return ds_heap_alloc( count );
}

void *
operator new[]( std::size_t count ) throw(std::bad_alloc)
{
  return ds_heap_alloc( count );
}

void *
operator new[]( std::size_t count, const std::nothrow_t&t ) throw()
{
  return ds_heap_alloc( count );
}

void
operator delete( void* pointer )
{
  ds_heap_free( pointer );
}

void
operator delete[]( void* pointer )
{
  ds_heap_free( pointer );
}

#endif
