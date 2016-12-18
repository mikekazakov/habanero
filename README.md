Habanero
========
Open repo with some useful stuff I produce sometimes.
Mostly C++ based and oriented for Mac(MacOSX/OSX/macOS) working environment (some stuff might work on iOS-based OSes as well).
This code is C++14 with some intrusions of coming C++17 via std::experimental.
The library also assumes x64 build targets.
[Nimble Commander](http://magnumbytes.com/) uses this code extensively.

Habanero/CFDefaultsCPP.h
-----------
Routines to work with User Defaults (CFPreferences / NSUserDefaults) directly from C++. There're variants to get value directly, regardless of it's presence, or to get it via optional<> to check if it is actually present in defaults map.

Habanero/CFStackAllocator.h
-----------
It's stack-based allocator for CoreFoundation objects. Might be useful for fast creating and processing some small objects, like CFStringRef. Obviously, object allocated with CFStackAllocator should not leak outside the function it was created in,
since this will lead to crash. CFStackAllocator is much faster than stock CoreFoundation allocator, but should be used with care.

Habanero/CFString.h
-----------
Set of routines for CFStringRef<->std::string interop.
Also has a CFString C++ class, which makes owning of CFStringRef objects a bit easier.

Habanero/CommonPaths.h
-----------
Some routines to access persistent directory locations. Contained in CommonPaths:: namespace.

Habanero/dispatch_cpp.h
-----------
A set of functions providing modern C++ interface for libdispatch API (also known as gcd - Grand Cental Dispatch).
They use C++11 lambdas as a callbacks and std::chrono values, so you can write something like this:

dispatch_after( 10s, dispatch_get_main_queue(), []{ puts("ten seconds after..."); } );

Following functions are wrapped and are available with the same names via C++ overloading resolution:
  * dispatch_async
  * dispatch_sync
  * dispatch_apply
  * dispatch_after
  * dispatch_barrier_async
  * dispatch_barrier_sync
Also some useful additions include these:
  * dispatch_is_main_queue
  * dispatch_assert_main_queue
  * dispatch_assert_background_queue
  * dispatch_to_main_queue
  * dispatch_to_default
  * dispatch_to_background
  * dispatch_to_main_queue_after
  * dispatch_to_background_after
  * dispatch_or_run_in_main_queue

Habanero/DispatchGroup.h
-----------
High-level wrapper abstraction on top of GCD's dispatch_group_async(), with callback signals about group's load state and info about amount of currently running tasks. Like dispatch_cpp.h, DispatchGroup is compatible with C++ lambdas and function<>'s.

Habanero/Hash.h
-----------
Hash/checksum calculation facility, supporting Adler32/CRC32/MD2/MD4/MD5/SHA1_160/SHA2_224/SHA2_256/SHA2_384/SHA2_512.
Relies on zlib and CommonCrypto routines.

Habanero/mach_time.h
-----------
Provides std::chrono::nanoseconds machtime() function, which tells the current relative kernel time in safe form of std::chrono. Also has a tiny MachTimeBenchmark time-measuring facility.

Habanero/SerialQueue.h
-----------
High-level wrapper abstraction on top of GCD's dispatch_async() serial execution with following additions:
  * Callback signals about queue's load state.
  * IsStopped() concept, lets you to flag running tasks as being discarded. The IsStopped() flag is automatically cleared with queue becomes empty.
  * Queue's length probing.
  * Compatible with C++ lambdas and function<>'s.

Habanero/spinlock.h
-----------
Spinlock implementation based on C++11 atomics, conforming BasicLockable concept. When thread can't acquire the lock, it will lower it's priority via Mach's swtch_pri() syscall. Also provides useful LOCK_GUARD(lock_object){...} macro and call_locked(lock_object, [=]{....}) template function.

Habanero/tiny_string.h
-----------
tiny_string is a std::string implementation with a small sizeof - 8 bytes. It assumes the following:
  * platform with 64-bit pointers (for my case it is x86-64)
  * little endian
  * only char as a value type, not a template class
  * no allocators
  * nearly C++14 compatible

The reason why this class exists is a situation when you need to store a string in an object and most of the times it will be empty. With this case basically there are two options:
  * use straightforward std::string, which is 24 bytes long for example on clang's std::string implemetation. Wasting a lot of memory for nothing.
  * use C-style char* buffer - 8 bytes for pointer. Manually allocate and deallocate it, deal with copying/moving etc. Boring and error-prone.

So tiny_string can be used - for strings up to 6 characters it will use a built-in buffer and will use malloc/realloc for larger strings.





