#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <stdint.h>

typedef int64_t AtomicValue;
typedef volatile int64_t AtomicStorage;

AtomicValue AtomicLoad(AtomicStorage* loc);

void AtomicStore(AtomicStorage* loc, AtomicValue value);

AtomicValue AtomicExchange(AtomicStorage* loc, AtomicValue value);

// Атомарно: *loc += delta. Возвращает СТАРОЕ значение (как std::atomic::fetch_add).
AtomicValue AtomicFetchAdd(AtomicStorage* loc, AtomicValue delta);

#endif  // __ATOMIC_H__