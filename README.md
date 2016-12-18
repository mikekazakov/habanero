Habanero
========
Open repo with some useful stuff I produce sometimes.
Mostly C++ based and oriented for Mac(MacOSX/OSX/macOS) working environment (some stuff might work on iOS-based OSes as well).
This code is using C++14 with some intrusions of coming C++17 via std::experimental.
The library also assumes x64 build targets.

Habanero/CFStackAllocator.h
-----------
It's stack-based allocator for CoreFoundation objects. Might be useful for fast creating and processing some small objects, like CFStringRef. Obviously, object allocated with CFStackAllocator should not leak outside the function it was created in,
since this will lead to crash. CFStackAllocator is much faster than stock CoreFoundation allocator, but should be used with care.

Habanero/CFDefaultsCPP.h
-----------
Routines to work with User Defaults (CFPreferences / NSUserDefaults) directly from C++. There're variants to get value directly, regardless of it's presence, or to get it via optional<> to check if it is actually present in defaults map.

Habanero/CommonPaths.h
-----------
Some routines to access persistent directory locations. Contained in CommonPaths:: namespace.

cpp/tiny_string
-----------
tiny_string is a std::string implementation with a small sizeof - 8 bytes. It assumes the following:
  * platform with 64-bit pointers (for my case it is x86-64)
  * little endian
  * only char as a value type, not a template class
  * no allocators
  * nearly C++14 compatible

The reason why this class exists is a situation when you need to store a string in an object and most of a times it will be empty. With this case basically there are two options:
  * use straightforward std::string, which is 24 bytes long for example on clang's std::string implemetation. Wasting a lot of memory for nothing.
  * use C-style char* buffer - 8 bytes for pointer. Manually allocate and deallocate it, deal with copying/moving etc. Boring and error-prone.

So tiny_string can be used - for strings up to 6 characters it will use a built-in buffer and will use malloc/realloc for larger strings.


cpp/dispatch_cpp
-----------
A set of functions providing modern C++ interface for libdispatch API (also known as gcd - Grand Cental Dispatch).
They use C++11 lambdas as a callback, so you can write something like this:

dispatch_after( 10s, dispatch_get_main_queue(), []{ puts("ten seconds after..."); } );

Following functions are wrapped and are available with the same names via C++ overloading resolution:
  * dispatch_async
  * dispatch_sync
  * dispatch_apply
  * dispatch_after
  * dispatch_barrier_async
  * dispatch_barrier_sync



