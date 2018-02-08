#ifndef _DAS_ATOMIC_H_
#define _DAS_ATOMIC_H_

#include <stdio.h>

long AtomicIncLong(volatile long *val);
long AtomicDecLong(volatile long *val);
long AtomicSetLong(volatile long *ptr, long val);
long AtomicLoadLong(const volatile long *ptr);
bool AtomicCompareSwapLong(volatile long *val, long old_val, long new_val);
bool AtomicSetBool(volatile bool *ptr, bool val);
bool AtomicLoadBool(const volatile bool *ptr);

#endif
