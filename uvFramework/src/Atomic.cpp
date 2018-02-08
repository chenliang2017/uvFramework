#include "Atomic.h"

#ifdef WIN32
#include <intrin.h>
#endif

long AtomicIncLong(volatile long *val)
{
#ifdef WIN32
	return _InterlockedIncrement(val);
#else
	return __sync_add_and_fetch(val, 1);
#endif
}

long AtomicDecLong(volatile long *val)
{
#ifdef WIN32
	return _InterlockedDecrement(val);
#else
	return __sync_sub_and_fetch(val, 1);
#endif
}

long AtomicSetLong(volatile long *ptr, long val)
{
#ifdef WIN32
	return (long)_InterlockedExchange((volatile long*)ptr, (long)val);
#else
	return __sync_lock_test_and_set(ptr, val);
#endif
}

long AtomicLoadLong(const volatile long *ptr)
{
#ifdef WIN32
	return (long)_InterlockedOr((volatile long*)ptr, 0);
#else
	return __atomic_load_n(ptr, __ATOMIC_SEQ_CST);
#endif
}

bool AtomicCompareSwapLong(volatile long *val, long old_val, long new_val)
{
#ifdef WIN32
	return _InterlockedCompareExchange(val, new_val, old_val) == old_val;
#else
	return __sync_bool_compare_and_swap(val, old_val, new_val);
#endif
}

bool AtomicSetBool(volatile bool *ptr, bool val)
{
#ifdef WIN32
	return !!_InterlockedExchange8((volatile char*)ptr, (char)val);
#else
	return __sync_lock_test_and_set(ptr, val);
#endif
}

bool AtomicLoadBool(const volatile bool *ptr)
{
#ifdef WIN32
	return !!_InterlockedOr8((volatile char*)ptr, 0);
#else
	return __atomic_load_n(ptr, __ATOMIC_SEQ_CST);
#endif
}
