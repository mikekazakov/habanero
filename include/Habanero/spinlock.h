/* Copyright (c) 2016 Michael G. Kazakov
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
#pragma once

#include <mach/mach.h>
#include <atomic>
#include <mutex>

class spinlock
{
    std::atomic_flag __flag = ATOMIC_FLAG_INIT ;
public:
    inline void lock() noexcept {
        while( __flag.test_and_set(std::memory_order_acquire) ) {
            swtch_pri(0); // talking to Mach directly
        }
    }
    inline void unlock() noexcept {
        __flag.clear(std::memory_order_release);
    }
};

#define __LOCK_GUARD_TOKENPASTE(x, y) x ## y
#define __LOCK_GUARD_TOKENPASTE2(x, y) __LOCK_GUARD_TOKENPASTE(x, y)
#define LOCK_GUARD(lock_object) bool __LOCK_GUARD_TOKENPASTE2(__lock_guard_go_, __LINE__) = true; \
    for(std::lock_guard<decltype(lock_object)> __LOCK_GUARD_TOKENPASTE2(__lock_guard_, __LINE__)(lock_object); \
        __LOCK_GUARD_TOKENPASTE2(__lock_guard_go_, __LINE__); \
        __LOCK_GUARD_TOKENPASTE2(__lock_guard_go_, __LINE__) = false \
        )

template <typename _Lock, typename _Callable>
auto call_locked( _Lock &_lock, _Callable _callable )
{
    std::lock_guard<_Lock> guard(_lock);
    return _callable();
}
