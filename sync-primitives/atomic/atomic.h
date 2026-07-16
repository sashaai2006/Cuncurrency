#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <stdint.h>

typedef int64_t AtomicValue;
typedef volatile int64_t AtomicStorage;

AtomicValue AtomicLoad(AtomicStorage* loc);

void AtomicStore(AtomicStorage* loc, AtomicValue value);

AtomicValue AtomicExchange(AtomicStorage* loc, AtomicValue value);

#endif  // __ATOMIC_H__